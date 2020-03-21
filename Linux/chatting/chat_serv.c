#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/socket.h>

#define BUF_SIZE 100
#define MAX_CLNT 256

void echoChat(char* msg, int len);
void* clientService(void* arg);
void errorHandling(char* eMsg);

int clntCnt = 0;
int clntSocks[MAX_CLNT];
pthread_mutex_t mutx;

int main(int argc, char* argv[]) {
    int servSock, clntSock;
    struct sockaddr_in servAddr, clntAddr;
    int clntAdrLen;

    pthread_t tId;

    if (argc != 2) {
        printf("Usage: %s <port>", argv[0]);
        exit(1);
    }

    pthread_mutex_init(&mutx, NULL);
    servSock = socket(PF_INET, SOCK_STREAM, 0);

    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(atoi(argv[1]));

    if(bind(servSock, (struct sockaddr*)&servAddr, sizeof(servAddr)) == -1) {
        errorHandling("bind() error");
    }
    if(listen(servSock, 5) == -1) {
        errorHandling("listen() error");
    }

    while(1) {
        clntAdrLen = sizeof(clntAddr);
        clntSock = accept(servSock, (struct sockaddr*)&clntAddr, &clntAdrLen);

        pthread_mutex_lock(&mutx);
        clntSocks[clntCnt++] = clntSock;
        pthread_mutex_unlock(&mutx);

        pthread_create(&tId, NULL, clientService, (void*)&clntSock);
        pthread_detach(tId);
        printf("Connected client IP: %s \n", inet_ntoa(clntAddr.sin_addr));
    }

    close(servSock);

    return 0;
}

void* clientService(void* arg) {
    int clntSock = *((int*)arg);
    int msgLen = 0, i;
    char msg[BUF_SIZE];

    while((msgLen = read(clntSock, msg, sizeof(msg))) != 0) {
        echoChat(msg, msgLen);
    }

    pthread_mutex_lock(&mutx);
    for(i = 0; i < clntCnt; i++) {
        if(clntSock == clntSocks[i]) {
            while(i++ < clntCnt - 1) {
                clntSocks[i] = clntSocks[i + 1];
            }
            break;
        }
    }

    clntCnt--;
    pthread_mutex_unlock(&mutx);
    close(clntSock);

    return NULL;
}

void echoChat(char* msg, int len) {
    int i;

    pthread_mutex_lock(&mutx);
    for(i = 0; i < clntCnt; i++) {
        write(clntSocks[i], msg, len);
    }
    pthread_mutex_unlock(&mutx);
}

void errorHandling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}