#include "Client.h"

Client::Client(char server[])
{
	WORD myVersionRequest;
	WSADATA wsaData;
	myVersionRequest = MAKEWORD(1, 1);
	int ret;
	ret = WSAStartup(myVersionRequest, &wsaData);
	if (ret != 0) {
		cout << "Socket Opened Error!" << endl;
		return;
	}
	clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (clientSocket == INVALID_SOCKET) {
		cout << "Socket() failed!" << endl;
		WSACleanup();
		return;
	}

	SOCKADDR_IN addr;
	addr.sin_addr.S_un.S_addr = inet_addr(server);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(23);

	ret = connect(clientSocket, (sockaddr *)&addr, sizeof(addr));
	if (ret == SOCKET_ERROR) {
		cout << "Connect() failed! Code: " << WSAGetLastError() << endl;
		closesocket(clientSocket);
		WSACleanup();
		return;
	}

}

int Client::sendMessage(char message[], int length)
{
	int ret = send(clientSocket, message, length, 0);
	if (ret == SOCKET_ERROR) {
		cout << "Send() failed! Code:" << WSAGetLastError() << endl;
	}
	else {
		cout << "Send Success!" << endl;
	}
	return ret;
}

int Client::recvMessage()
{
	char receiveBuf[100];
	int ret = recv(clientSocket, receiveBuf, 101, 0);
	return ret;
}

Client::~Client()
{
	closesocket(clientSocket);
	WSACleanup();
}
