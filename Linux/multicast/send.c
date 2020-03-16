#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define TTL 64 // Time to Live(경로상의 router의 최대개수를 나타냄)
#define BUF_SIZE 30

void error_handling(char* message);

int main(int argc, char* argv[]) {
    int send_sock; // file descriptor of the sending socket
    struct sockaddr_in sock_addr;
    int time_live = TTL;
    FILE* fp;
    char buf[BUF_SIZE];

    if(argc != 3) {
        printf("Usage: %s <GIP> <PORT>\n", argv[0]);
        exit(1);
    }

    send_sock = socket(PF_INET, SOCK_DGRAM, 0);
    memset(&sock_addr, 0, sizeof(sock_addr));
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_addr.s_addr = inet_addr(argv[1]);
    sock_addr.sin_port = htons(atoi(argv[2]));

    setsockopt(send_sock, IPPROTO_IP, IP_MULTICAST_TTL, (void*)&time_live, sizeof(time_live));
    if((fp = fopen("multicast_news.txt", "r")) == NULL) {
        error_handling("fopen() error");
    }

    while(!feof(fp)) {
        fgets(buf, BUF_SIZE, fp);
        sendto(send_sock, buf, strlen(buf), 0, (struct sockaddr*)&sock_addr, sizeof(sock_addr));
        sleep(2);
    }

    close(fp);
    close(send_sock);

    return 0;
}

void error_handling(char* message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}