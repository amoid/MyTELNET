#include "stdafx.h"
using namespace std;
#pragma once

class Client
{
private:
	SOCKET clientSocket;

public:
	Client(char server[]);
	int sendMessage(char message[], int length);
	int recvMessage();
	~Client();
};

