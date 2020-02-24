#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>

void ErrorHandling(char* message);

int main(int argc, char* argv[]) {
	WSADATA wsaData;
	SOCKET servSock, clntSock;
	SOCKADDR_IN servAddr, clntAddr;

	int szClntAddr;
	char message[] = "Hello World!";
	if (argc != 2) {
		printf("Usage: %s <port>\n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		ErrorHandling("WSAStartup() error");
	}

	servSock = socket(PF_INET, SOCK_STREAM, 0);
	if (servSock == INVALID_SOCKET) {
		ErrorHandling("socket() error");
	}

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(atoi(argv[1]));

	if (bind(servSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) {
		ErrorHandling("bind() error");
	}

	if (listen(servSock, 5) == SOCKET_ERROR) {
		ErrorHandling("listen() error");
	}

	szClntAddr = sizeof(clntAddr);
	clntSock = accept(servSock, (SOCKADDR*)&clntAddr, &szClntAddr);
	if (clntSock == INVALID_SOCKET) {
		ErrorHandling("accept() error");
	}

	send(clntSock, message, sizeof(message), 0);
	closesocket(clntSock);
	closesocket(servSock);
	WSACleanup();
	return 0;
}

void ErrorHandling(char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}