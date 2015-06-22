// MyClient.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Client.h"

int _tmain(int argc, _TCHAR* argv[])
{
	Client client = Client("127.0.0.1");
	char x[1024];
	int length;
	length = 1024;
	int ret = 1;
	while (ret > 0) {
		ret = client.handle();
	}
	while (1) {
		scanf("%s", x);
		ret = client.sendMessage(x, strlen(x));
		ret = client.handle();
	}
	//client.recvMessage(x, length);
	/*
	cout << x << endl;
	while (1) {
		memset(x, 0, 100);
		scanf("%s", x);
		client.sendMessage(x, 100);
		length = 100;
		client.recvMessage(x, length);
		cout << x << endl;
	}
	*/
	getchar();
	return 0;
}

