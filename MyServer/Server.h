#pragma once
#include "stdafx.h"
using namespace std;

class Server
{
private:
	BOOL supress;
	BOOL echo;
	SOCKET serverSocket;
	char sendBuffer[MAX_BUFFER_SIZE];
	char sendIndex = 0;
	SOCKADDR_IN addr;
public:
	Server(SOCKET socket, SOCKADDR_IN addr);
	~Server();
	int sendMessage(char *message, int length);
	int recvMessage(char *message, int length);
	int logMessage(char *message);
	int handle();
};

