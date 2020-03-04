#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
void error_handling(char* message);

int main(int argc, char* argv[]) {
    int sock;
    char message[BUF_SIZE];
    int str_len, recv_len, recv_cnt;
    struct sockaddr_in serv_addr;
    int opCnt, i, j;

    if(argc != 3) {
        printf("Usage: %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1) {
        error_handling("socket() error");
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
        error_handling("connect() error");
    } else {
        puts("Connected.......");
    }

    while(1) {
        fputs("Operand count: ", stdout);
        fgets(message, BUF_SIZE, stdin);
        if(write(sock, message, strlen(message) == -1)) {
            error_handling("write() error");
        }
        
        opCnt = atoi(message);
        for(i = 0; i < opCnt; i++) {
            printf("Operand %d: ", i);
            fgets(message, BUF_SIZE, stdin);
            if(write(sock, message, strlen(message) == -1)) {
                error_handling("write() error");
            }
        }

        fputs("Operator: ", stdout);
        fgets(message, BUF_SIZE, stdin);
        if(write(sock, message, strlen(message) == -1)) {
            error_handling("write() error");
        }

        if(read(sock, &message, BUF_SIZE - 1) == -1) {
            error_handling("read() erorr");
        } else {
            message[BUF_SIZE] = 0;
            printf("Operation result: %s", message);
        }
    }

    close(sock);
    return 0;
}

void error_handling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}