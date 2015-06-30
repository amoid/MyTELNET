// MyServer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "server.h"
using namespace std;

#define CONNECT_MAX_NUMBER 5

DWORD WINAPI socketThread(LPP *lp);

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

	cout << "Please input the listening port: ";
	int port;
	cin >> port;

	SOCKADDR_IN serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
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

	while (1) {
		SOCKET serverSocketC = accept(serverSocket, (struct sockaddr *)&clientAddr, &length);
		if (serverSocketC == INVALID_SOCKET) {
			closesocket(serverSocket);
			WSACleanup();
			cout << "Accept() failed! code: " << WSAGetLastError() << endl;
			return 0;
		}

		LPP lp = { serverSocketC , clientAddr };

		HANDLE hThread1 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)socketThread, (LPVOID)&lp, 0, 0);

		cout << "Client Accepted: " << inet_ntoa(clientAddr.sin_addr) << " " << ntohs(clientAddr.sin_port) << endl;

	}
	
	closesocket(serverSocket);
	WSACleanup();

	getchar();
	return 0;
}

DWORD WINAPI socketThread(LPP *lp)
{
	SOCKET socket = lp->socket;
	SOCKADDR_IN addr = lp->addr;
	cout << "Thread Created." << endl;
	Server server = Server(socket, addr);

	int ret = server.handle();
	if (ret < 0) {
		cout << "Client Closed!" << endl;
	}
	/*
	Server server = Server(socket);
	char x[100] = "Welcome!";
	int length = 101;
	int res;
	
	server.sendMessage(x, length);

	while (1) {
		char x[100];
		int length = 101;
		server.recvMessage(x, length);
		server.sendMessage(x, length);
	}
	*/
//	GetCurrentThreadId();
	return ret;
}