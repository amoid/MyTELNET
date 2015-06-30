#include "stdafx.h"
#include "define.h"

using namespace std;
#pragma once

class Client
{
private:
	SOCKET clientSocket;
	char sendBuffer[MAX_BUFFER_SIZE];
	int sendIndex;
	int logMessage(char *message);
public:
	int sendMessage(char *message, int length);
	int recvMessage(char *message, int length);

	Client(char server[], int port);
	int handle();


	
	~Client();
};

