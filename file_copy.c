#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

#define SHM_SIZE 1024  // 공유 메모리 크기
#define KEY_SHM 1234   // 공유 메모리 키
#define KEY_SEM 5678   // 세마포어 키

// 세마포어 동작 함수
void sem_wait(int semid, int sem_num) {
    struct sembuf op = {sem_num, -1, 0};  // 감소 연산
    semop(semid, &op, 1);
}

void sem_signal(int semid, int sem_num) {
    struct sembuf op = {sem_num, 1, 0};  // 증가 연산
    semop(semid, &op, 1);
}

int main(int argc, char *argv[]) {
    // 1. 명령어 인자 확인
    if (argc != 3) {
        printf("Usage: %s <source file> <destination file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char *src_file = argv[1];
    char *dst_file = argv[2];

    // 2. 공유 메모리 생성
    int shmid = shmget(KEY_SHM, SHM_SIZE, 0666 | IPC_CREAT);
    if (shmid == -1) {
        perror("shmget failed");
        return EXIT_FAILURE;
    }

    // 3. 세마포어 생성
    int semid = semget(KEY_SEM, 2, 0666 | IPC_CREAT);
    if (semid == -1) {
        perror("semget failed");
        return EXIT_FAILURE;
    }

    // 세마포어 초기화
    semctl(semid, 0, SETVAL, 1);  // write_sem
    semctl(semid, 1, SETVAL, 0);  // read_sem

    // 4. 공유 메모리 연결
    char *shm_ptr = (char *)shmat(shmid, NULL, 0);
    if (shm_ptr == (char *)-1) {
        perror("shmat failed");
        return EXIT_FAILURE;
    }

    // 5. 프로세스 분기
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        return EXIT_FAILURE;
    }

    if (pid == 0) { // 자식 프로세스
        int dst_fd = open(dst_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (dst_fd == -1) {
            perror("open failed (destination file)");
            return EXIT_FAILURE;
        }

        while (1) {
            sem_wait(semid, 1);  // 읽기 세마포어 대기
            if (strncmp(shm_ptr, "EOF", 3) == 0) break;
            write(dst_fd, shm_ptr, strlen(shm_ptr));
            sem_signal(semid, 0);  // 쓰기 세마포어 신호
        }

        close(dst_fd);
        exit(EXIT_SUCCESS);

    } else { // 부모 프로세스
        int src_fd = open(src_file, O_RDONLY);
        if (src_fd == -1) {
            perror("open failed (source file)");
            return EXIT_FAILURE;
        }

        ssize_t bytes_read;
        while ((bytes_read = read(src_fd, shm_ptr, SHM_SIZE - 1)) > 0) {
            shm_ptr[bytes_read] = '\0';  // Null-terminate data
            sem_signal(semid, 1);  // 읽기 세마포어 신호
            sem_wait(semid, 0);  // 쓰기 세마포어 대기
        }

        strcpy(shm_ptr, "EOF");  // EOF 전송
        sem_signal(semid, 1);

        close(src_fd);
        wait(NULL);
    }

    // 6. 자원 정리
    shmdt(shm_ptr);
    shmctl(shmid, IPC_RMID, NULL);
    semctl(semid, 0, IPC_RMID);
    semctl(semid, 1, IPC_RMID);

    return 0;
}

