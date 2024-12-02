#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid;

    printf("Parent process started. PID: %d\n", getpid());

    // 프로세스 생성
    pid = fork();

    if (pid < 0) {
        // fork() 실패
        perror("Fork failed");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // 자식 프로세스 코드
        printf("Child process started. PID: %d, Parent PID: %d\n", getpid(), getppid());

        // exec() 함수로 다른 프로그램 실행
        execlp("/bin/ls", "ls", "-l", NULL);

        // exec() 실패 시
        perror("execlp failed");
        exit(EXIT_FAILURE);
    } else {
        // 부모 프로세스 코드
        printf("Parent process waiting for child to finish.\n");

        int status;
        wait(&status);  // 자식 프로세스 종료 대기

        if (WIFEXITED(status)) {
            printf("Child exited with status: %d\n", WEXITSTATUS(status));
        } else {
            printf("Child terminated abnormally.\n");
        }

        printf("Parent process exiting.\n");
    }

    return 0;
}

