#include "Server.h"

Server::Server(SOCKET socket)
{
	serverSocket = socket;
	supress = false;
	echo = false;
	memset(sendBuffer, 0, sizeof(sendBuffer));
	sendBuffer[sendIndex++] = IAC;
	sendBuffer[sendIndex++] = WILL;
	sendBuffer[sendIndex++] = ECHO;
	sendBuffer[sendIndex++] = IAC;
	sendBuffer[sendIndex++] = WILL;
	sendBuffer[sendIndex++] = SUPRESS;
	sendMessage(sendBuffer, sendIndex++);
	supress = true;
	echo = true;
}


Server::~Server()
{
	closesocket(serverSocket);
}

int Server::sendMessage(char *message, int length)
{
	int ret = send(serverSocket, message, length, 0);
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

int Server::recvMessage(char *message, int length)
{
	int ret;
	char *ptr = message;
	int totalLength = 0;
	while (1) {
		ret = recv(serverSocket, ptr, length, 0);
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
	return 1;
}

int Server::handle()
{
	while (1) {
		unsigned char rec[MAX_BUFFER_SIZE];
		char display[MAX_BUFFER_SIZE];
		int displayIndex = 0;
		memset(display, 0, sizeof(display));
		int ret = recvMessage((char *)rec, MAX_BUFFER_SIZE);
		if (ret < 0) {
			return ret;
		}
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
						if (rec[i + 2] == ECHO || rec[i + 2] == SUPRESS) {
							if (rec[i + 1] == WILL || rec[i + 1] == DO) {
								if (echo == false && rec[i + 2] == ECHO || supress == false && rec[i + 2] == SUPRESS) {
									sendBuffer[sendIndex++] = IAC;
									sendBuffer[sendIndex++] = rec[i + 1] == WILL ? DO: WILL;
									sendBuffer[sendIndex++] = rec[i + 2];
								}
							}

						}
						else {
							if (rec[i + 1] == WILL || rec[i + 1] == DO || rec[i + 1] == WONT || rec[i + 1] == DONT) {
								sendBuffer[sendIndex++] = IAC;
								switch (rec[i + 1])
								{
								WILL:
									sendBuffer[sendIndex++] = DONT;
									break;
								WONT:
									sendBuffer[sendIndex++] = DONT;
									break;
								DO:
									sendBuffer[sendIndex++] = WONT;
									break;
								DONT:
									sendBuffer[sendIndex++] = WONT;
									break;
								}
								sendBuffer[sendIndex++] = rec[i + 2];
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
								i -= 2;
							}
						}

					}
					else {
						display[displayIndex++] = rec[i];
						i -= 2;
					}
					i += 2;
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
				strcpy(sendBuffer+sendIndex, display);
				sendIndex += strlen(display) + 1;
			}
			if (sendIndex != 0) {
				sendMessage(sendBuffer, sendIndex);
			}
			else {
				char x[40] = "Welcome My TELNET Server!";
				sendMessage(x, strlen(x) + 1);
			}

		}

		//end while
	}

}
