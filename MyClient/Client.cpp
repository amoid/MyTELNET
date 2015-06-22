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

int Client::handle()
{
	unsigned char rec[MAX_BUFFER_SIZE];
	char display[MAX_BUFFER_SIZE];
	int displayIndex = 0;
	memset(display, 0, sizeof(display));
	int ret = recvMessage((char *)rec, MAX_BUFFER_SIZE);
#ifdef _DEBUG_MODE
	printf("---------\nRECEIVE:\n");
	for (int i = 0; i < ret; i++) {
		printf("%hhu  ", rec[i]);
	}
	printf("\n");
#endif
	if (ret > 0) {
		memset(sendBuffer, 0, sizeof(sendBuffer));
		sendIndex = 0;
		for (int i = 0; i < ret; i++) {
			if (rec[i] == IAC) {
				if (ret > i + 2) {
					if (rec[i + 1] == DO) {
						if (rec[i + 2] == ECHO || rec[i + 2] == SUPRESS) {
							sendBuffer[sendIndex++] = IAC;
							sendBuffer[sendIndex++] = WILL;
							sendBuffer[sendIndex++] = rec[i + 2];
						}
						else {
							sendBuffer[sendIndex++] = IAC;
							sendBuffer[sendIndex++] = WONT;
							sendBuffer[sendIndex++] = rec[i + 2];
						}
					}
					else if (rec[i + 1] == WILL) {
						if (rec[i + 2] == ECHO || rec[i + 2] == SUPRESS) {
							sendBuffer[sendIndex++] = IAC;
							sendBuffer[sendIndex++] = DO;
							sendBuffer[sendIndex++] = rec[i + 2];
						}
						else {
							sendBuffer[sendIndex++] = IAC;
							sendBuffer[sendIndex++] = DONT;
							sendBuffer[sendIndex++] = rec[i + 2];
						}
					}
					else if (rec[i + 1] == WONT) {
						if (rec[i + 2] == ECHO || rec[i + 2] == SUPRESS) {
							sendBuffer[sendIndex++] = IAC;
							sendBuffer[sendIndex++] = DONT;
							sendBuffer[sendIndex++] = rec[i + 2];
						}
						else {
							sendBuffer[sendIndex++] = IAC;
							sendBuffer[sendIndex++] = DONT;
							sendBuffer[sendIndex++] = rec[i + 2];
						}
					}
					else if (rec[i + 1] == DONT) {
						if (rec[i + 2] == ECHO || rec[i + 2] == SUPRESS) {
							sendBuffer[sendIndex++] = IAC;
							sendBuffer[sendIndex++] = WONT;
							sendBuffer[sendIndex++] = rec[i + 2];
						}
						else {
							sendBuffer[sendIndex++] = IAC;
							sendBuffer[sendIndex++] = WONT;
							sendBuffer[sendIndex++] = rec[i + 2];
						}
					}
					else if (rec[i + 1] == SB) {
						int j = i;
						while (j < ret - 1) {
							if (rec[j] == IAC && rec[j + 1] == SE) {
								break;
							}
							j++;
						}
						if (j == ret - 1) {
							//not found
							display[displayIndex++] = rec[i];
						}
						else {
							sendBuffer[sendIndex++] = IAC;
							sendBuffer[sendIndex++] = SB;
							sendBuffer[sendIndex++] = rec[i + 2];
							sendBuffer[sendIndex++] = 0;
							sendBuffer[sendIndex++] = IAC;
							sendBuffer[sendIndex++] = SE;
							i = j;
						}
					}
					else {
						display[displayIndex++] = rec[i];
						i-=2;
					}
				}
				else {
					display[displayIndex++] = rec[i];
					i-=2;
				}
				i+=2;
			}
			else {
				display[displayIndex++] = rec[i];
			}

		}
#ifdef _DEBUG_MODE
		printf("TO SEND: \n");
		for (int i = 0; i < sendIndex; i++) {
			printf("%hhu  ", sendBuffer[i]);
		}
		printf("\n");
		printf("DISPLAY:\n");
#endif
		if (displayIndex != 0) {
			cout << display << endl;
		}

		if (sendIndex != 0) {
			sendMessage(sendBuffer, sendIndex);
		}
		
		return sendIndex == 0 ? 0 : 1;
	}
	return 0;
}

int Client::sendMessage(char *message, int length)
{
	int ret = send(clientSocket, message, length, 0);
	if (ret == SOCKET_ERROR) {
		cout << "Send() failed! Code:" << WSAGetLastError() << endl;
	}
	else {
#ifdef _DEBUG_MODE
		cout << "Send Success!" << endl;
#endif
	}
	return ret;
}

int Client::recvMessage(char *message, int length)
{
	int ret;
	char *ptr = message;
	int totalLength = 0;
	while (1) {
		ret = recv(clientSocket, ptr, length, 0);
		if (ret == SOCKET_ERROR) {
			cout << "Recv() failed!" << endl;
			return SOCKET_ERROR;
		}
		else if (ret == 0) {
			cout << "Connection Closed!" << endl;
			return 0;
		}
		else if (ret != length) {
			return ret;
		}
		else {
			ptr += ret;
			totalLength += ret;
			// to do
			cout << "Out Of length in recv()" << endl;
		}
	}
	
	/*
	char *ptr = message;
	int dataLength = length;
	int ret;
	while (dataLength > 0) {
		ret = recv(clientSocket, ptr, length, 0);
		if (ret == SOCKET_ERROR) {
			cout << "Recv() failed!" << endl;
			return ret;
		}

		if (ret == 0) {
			cout << "Connection Closed!" << endl;
			return 0;
		}
		for (int i = 0; i < ret; i++)
		{
			//printf("%hhu ", ptr[i]);
		}
		dataLength -= ret;
		ptr += ret;
		cout << ret << endl;
		cout << dataLength << endl;
	}
	if (dataLength == 0) {
		cout << message << endl;
	}
	*/
	return 1;
}

Client::~Client()
{
	closesocket(clientSocket);
	WSACleanup();
}
