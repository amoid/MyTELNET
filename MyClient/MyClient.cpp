// MyClient.cpp : �������̨Ӧ�ó������ڵ㡣
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

