// MyServer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

using namespace std;

#define CONNECT_MAX_NUMBER 5

int _tmain(int argc, _TCHAR* argv[])
{
	SOCKET serverSocket;
	WORD myVersionRequest = MAKEWORD(1, 1);
	WSADATA wsaData;

	int length, ret;
	int err = WSAStartup(myVersionRequest, &wsaData);
	if (err != 0) {
		cout << "WSAStartup() failed!" << endl;
		return 0;
	}
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == INVALID_SOCKET) {
		WSACleanup();
		cout << "Socket() failed" << endl;
		return 0;
	}

	SOCKADDR_IN serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(23);
	serverAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	
	//bind
	ret = bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
	if (ret == SOCKET_ERROR) {
		closesocket(serverSocket);
		WSACleanup();
		cout << "Bind() failed! code: " << WSAGetLastError() << endl;
		return 0;
	}
	
	ret = listen(serverSocket, CONNECT_MAX_NUMBER); 
	if (ret == SOCKET_ERROR) {
		closesocket(serverSocket);
		WSACleanup();
		cout << "Listen() failed! code: " << WSAGetLastError() << endl;
		return 0;
	}

	cout << "Server created! Wait for clients..." << endl;

	SOCKADDR_IN clientAddr;
	length = sizeof(clientAddr);
	SOCKET serverSocketC = accept(serverSocket, (struct sockaddr *)&clientAddr, &length);
	if (serverSocketC == INVALID_SOCKET) {
		closesocket(serverSocket);
		WSACleanup();
		cout << "Accept() failed! code: " << WSAGetLastError() << endl;
		return 0;
	}

	cout << "Client Accepted: " << inet_ntoa(clientAddr.sin_addr) << " " << ntohs(clientAddr.sin_port) << endl;

	char x[100];
	int dataLength = 100;
	char *ptr = x;
	while (dataLength > 0) {
		ret = recv(serverSocketC, ptr, 100, 0);
		if (ret == SOCKET_ERROR) {
			cout << "Recv() failed!" << endl;
			break;
		}

		if (ret == 0) {
			cout << "Connection Closed!" << endl;
			break;
		}

		dataLength -= ret;
		ptr += ret;
	}
	if (dataLength == 0) {
		cout << "All data received!" << endl;
		cout << x << endl;
	}

	closesocket(serverSocketC);
	closesocket(serverSocket);
	WSACleanup();

	getchar();
	return 0;
}

