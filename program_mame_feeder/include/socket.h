#pragma once

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "8000"

#define ERROR_CHAR "-"

using namespace std;

class Socket
{
private:
	WSADATA wsaData;
	SOCKET ConnectSocket = INVALID_SOCKET;
	struct addrinfo *result = NULL,
					*ptr = NULL,
					hints;
	char recvbuf[DEFAULT_BUFLEN] = {0};
	int iResult;
	int recvbuflen = DEFAULT_BUFLEN;
	string string_buff;

public:
	bool Init(string server);
	bool Close();
	string WaitForNewData();
};