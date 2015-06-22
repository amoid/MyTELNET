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
public:
	Server(SOCKET socket);
	~Server();
	int sendMessage(char *message, int length);
	int recvMessage(char *message, int length);
	int handle();
};

