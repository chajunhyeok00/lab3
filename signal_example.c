#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

// 시그널 핸들러 함수
void handle_signal(int signal) {
    if (signal == SIGINT) {
        printf("\nReceived SIGINT (Ctrl+C). Exiting safely...\n");
        exit(0);
    }
}

int main() {
    // SIGINT 시그널을 처리하기 위해 핸들러 등록
    signal(SIGINT, handle_signal);

    printf("Program running. Press Ctrl+C to exit.\n");

    // 무한 루프에서 대기
    while (1) {
        printf("Waiting for signal...\n");
        sleep(2);  // 2초마다 출력
    }

    return 0;
}

