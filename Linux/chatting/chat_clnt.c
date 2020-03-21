#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/socket.h>

#define BUF_SIZE 100
#define NAME_SIZE 20

void* sendMsg(void* arg);
void* recvMsg(void* arg);
void errorHandling(char* msg);

char usr[NAME_SIZE] = "[DEFAULT]";
char msg[BUF_SIZE];

int main(int argc, char* argv[]) {
    int sock;
    struct sockaddr_in servAddr;
    pthread_t sendTid, recvTid;
    void* threadReturn;

    if(argc != 4) {
        printf("Usage: %s <IP> <PORT> <USER_NAME>\n", argv[0]);
        exit(1);
    }

    sprintf(usr, "[%s]", argv[3]);
    sock = socket(PF_INET, SOCK_STREAM, 0);

    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(argv[1]);
    servAddr.sin_port = htons(atoi(argv[2]));

    if(connect(sock, (struct sockaddr*)&servAddr, sizeof(servAddr)) == -1) {
        errorHandling("connect() error");
    }

    pthread_create(&sendTid, NULL, sendMsg, (void*)&sock);
    pthread_create(&recvTid, NULL, recvMsg, (void*)&sock);
    pthread_join(sendTid, &threadReturn);
    pthread_join(recvTid, &threadReturn);

    close(sock);

    return 0;
}

void* sendMsg(void* arg) {
    int sock = *(int*)arg;
    char chatMsg[NAME_SIZE + BUF_SIZE];

    while(1) {
        fgets(msg, BUF_SIZE, stdin);
        if(!strcmp(chatMsg, "q\n") || !strcmp(chatMsg, "Q\n")) {
            close(sock);
            exit(0);
        }

        sprintf(chatMsg, "%s %s", usr, msg);
        write(sock, chatMsg, strlen(chatMsg));
    }

    return NULL;
}

void* recvMsg(void* arg) {
    int sock = *(int*)arg;
    char chatMsg[NAME_SIZE + BUF_SIZE];
    int chatLen;

    while(1) {
        chatLen = read(sock, chatMsg, NAME_SIZE + BUF_SIZE - 1);
        if(chatLen == -1) {
            return (void*)-1;
        }
        
        chatMsg[chatLen] = 0;
        fputs(chatMsg, stdout);
    }

    return NULL;
}

void errorHandling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}