#pragma once
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <tchar.h>

class UDPServer {
public:
	bool recieveData();
	bool sendData(sockaddr_in& clientsAddress, char* buffer);
	void closeServer();
	bool startServer();
private:
	SOCKET serverSocket;
	int port{ 55555 };
	int  queue{ 1 };
	WSADATA wsaData;
	bool startWsa();
	bool createServerSocket();
	bool bindSocket();
};