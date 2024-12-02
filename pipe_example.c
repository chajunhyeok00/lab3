#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main() {
    int pipe1[2]; // 부모에서 자식으로 데이터 전송하는 파이프
    int pipe2[2]; // 자식에서 부모로 데이터 전송하는 파이프
    pid_t pid;
    char write_msg[] = "Hello from parent!";
    char read_msg[100];

    // 파이프 생성
    if (pipe(pipe1) == -1) {
        perror("pipe1 creation failed");
        exit(1);
    }
    if (pipe(pipe2) == -1) {
        perror("pipe2 creation failed");
        exit(1);
    }

    // 자식 프로세스 생성
    pid = fork();
    if (pid < 0) {
        perror("fork failed");
        exit(1);
    }

    if (pid == 0) { // 자식 프로세스
        close(pipe1[1]); // 부모에게 데이터를 쓰는 끝 닫기
        close(pipe2[0]); // 부모에서 데이터를 읽는 끝 닫기

        // 부모로부터 데이터 읽기
        read(pipe1[0], read_msg, sizeof(read_msg));
        printf("Child received: %s\n", read_msg);

        // 부모에게 데이터 쓰기
        char reply[] = "Hello from child!";
        write(pipe2[1], reply, strlen(reply) + 1);

        close(pipe1[0]);
        close(pipe2[1]);
        exit(0);
    } else { // 부모 프로세스
        close(pipe1[0]); // 자식에게 데이터를 읽는 끝 닫기
        close(pipe2[1]); // 자식에서 데이터를 쓰는 끝 닫기

        // 자식에게 데이터 쓰기
        write(pipe1[1], write_msg, strlen(write_msg) + 1);

        // 자식으로부터 데이터 읽기
        read(pipe2[0], read_msg, sizeof(read_msg));
        printf("Parent received: %s\n", read_msg);

        close(pipe1[1]);
        close(pipe2[0]);
        wait(NULL); // 자식 프로세스 종료 대기
    }

    return 0;
}

