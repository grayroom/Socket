#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>

#define BUF_SIZE 1024
void ErrorHandling(char* message);

int main(int argc, char* argv[]) {
	WSADATA wsaData;
	SOCKET servSock, clntSock;
	SOCKADDR_IN servAddr, clntAddr;
	
	char message[BUF_SIZE];
	int strLen, i;
	int clntAddrSize;
	
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

	/*
	size = sizeof(servAddr);
	WSAStringToAddress(strAddr, AF_INET, NULL, (SOCKADDR*)&servAddr, &size)
	*/
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

	clntAddrSize = sizeof(clntAddr);
	
	for(i = 0; i < 5; i++) {
		clntSock = accept(servSock, (SOCKADDR*)&clntAddr, &clntAddrSize);
		if(clntSock == -1) {
			ErrorHandling("accept() error");
		} else {
			printf("Connected client %d \n", i + 1);
		}

		while((strLen = recv(clntSock, message, BUF_SIZE, 0)) != 0) {
			send(clntSock, message, strLen, 0);
		}

		closesocket(clntSock);
	}

	closesocket(servSock);
	WSACleanup();
	return 0;
}

void ErrorHandling(char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}