#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>

#define MSG_KEY 1234   // 메시지 큐의 키 값
#define MAX_TEXT 512   // 최대 텍스트 길이

// 메시지 구조체
struct message {
    long mtype;
    char mtext[MAX_TEXT];
};

int main() {
    int msgid;
    struct message msg;

    // 메시지 큐 생성
    msgid = msgget(MSG_KEY, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget failed");
        exit(1);
    }

    printf("Server ready to receive messages...\n");

    while (1) {
        // 메시지 수신
        if (msgrcv(msgid, &msg, sizeof(msg.mtext), 1, 0) == -1) {
            perror("msgrcv failed");
            exit(1);
        }

        printf("Server received: %s\n", msg.mtext);

        // 응답 메시지 전송
        msg.mtype = 2;  // 응답 메시지 타입
        snprintf(msg.mtext, MAX_TEXT, "Message received: %s", msg.mtext);

        if (msgsnd(msgid, &msg, sizeof(msg.mtext), 0) == -1) {
            perror("msgsnd failed");
            exit(1);
        }
    }

    return 0;
}

