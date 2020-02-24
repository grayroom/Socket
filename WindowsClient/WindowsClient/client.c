#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>

void ErrorHandling(char* message);

int main(int argc, char* argv[]) {
	WSADATA wsaData;
	SOCKET sock;
	SOCKADDR_IN servAddr;

	char message[30];
	int strLen = 0;
	int idx = 0, readLen = 0;

	if (argc != 3) {
		printf("Usage: %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		ErrorHandling("WSAStartup() error");
	}

	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		ErrorHandling("socket() error");
	}

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(argv[1]);
	servAddr.sin_port = htons(atoi(argv[2]));

	if (connect(sock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) {
		ErrorHandling("connect() error");
	}

	while (readLen = recv(sock, &message[idx++], 1, 0)) {
		if (readLen == -1) {
			ErrorHandling("read() error");
		}

		strLen += readLen;
	}

	printf("Message from server: %s \n", message);
	printf("Function read call count: %d \n", strLen);

	closesocket(sock);
	WSACleanup();
	return 0;
}

void ErrorHandling(char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}