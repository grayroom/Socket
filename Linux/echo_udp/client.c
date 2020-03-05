#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
void error_handling(char* message);

int main(int argc, char* argv[]) {
    int clnt_sock;
    char message[BUF_SIZE];
    int str_len;
    socklen_t read_adr_sz;

    struct sockaddr_in write_adr, read_adr;
    if(argc != 3) {
        printf("Usage: %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    clnt_sock = socket(PF_INET, SOCK_DGRAM, 0);
    if(clnt_sock == -1) {
        error_handling("socket() error");
    }

    memset(&write_adr, 0, sizeof(write_adr));
    write_adr.sin_family = AF_INET;
    write_adr.sin_addr.s_addr = inet_addr(argv[1]);
    write_adr.sin_port = htons(atoi(argv[2]));

    while(1) {
        fputs("Insert message(q to quit): ", stdout);
        fgets(message, sizeof(message), stdin);
        if(!strcmp(message, "q\n") || !strcmp(message, "Q\n")) {
            break;
        }

        sendto(clnt_sock, message, strlen(message), 0, (struct sockaddr*)&write_adr, sizeof(write_adr));
        read_adr_sz = sizeof(read_adr);
        str_len = recvfrom(clnt_sock, message, BUF_SIZE, 0, (struct sockaddr*)&read_adr, &read_adr_sz);
        message[str_len] =  0;
        printf("Message from server: %s", message);
    }

    close(clnt_sock);
    return 0;
}

void error_handling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}