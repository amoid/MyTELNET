// MyClient.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Client.h"

int _tmain(int argc, _TCHAR* argv[])
{
	Client client = Client("127.0.0.1");
	char x[100] = "12312312";
	client.sendMessage(x, 100);

	getchar();
	return 0;
}

