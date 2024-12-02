#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

// 직접 구현한 system 함수
int my_system(const char *command) {
    if (command == NULL) {
        fprintf(stderr, "No command provided.\n");
        return -1;
    }

    pid_t pid = fork();  // 자식 프로세스 생성
    if (pid < 0) {
        perror("Fork failed");
        return -1;
    }

    if (pid == 0) {
        // 자식 프로세스: 쉘 명령 실행
        execl("/bin/sh", "sh", "-c", command, (char *)NULL);
        perror("execl failed"); // execl 실패 시 메시지 출력
        exit(EXIT_FAILURE);
    } else {
        // 부모 프로세스: 자식 종료 대기
        int status;
        wait(&status);
        return WEXITSTATUS(status);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <command>\n", argv[0]);
        return 1;
    }

    // 명령어를 하나의 문자열로 합치기
    char command[1024] = "";
    for (int i = 1; i < argc; i++) {
        strcat(command, argv[i]);
        if (i < argc - 1) strcat(command, " ");  // 명령어 사이에 공백 추가
    }

    printf("Running command: %s\n", command);

    // 직접 구현한 my_system 호출
    int result = my_system(command);

    printf("Command exited with status: %d\n", result);
    return 0;
}

