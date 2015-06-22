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
public:
	int sendMessage(char *message, int length);
	int recvMessage(char *message, int length);

	Client(char server[]);
	int handle();


	
	~Client();
};

