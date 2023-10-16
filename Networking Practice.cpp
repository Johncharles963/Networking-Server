#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <tchar.h>

bool startWsa(WSADATA& wsaData);
bool createServerSocket(SOCKET& serverSocket);
bool bindSocket(SOCKET& socket, int& port);
bool startListen(SOCKET& socket, int& queue);
bool socketAccept(SOCKET& serverSocket, SOCKET& acceptSocket);

int main()
{
    std::cout << "Hello World!\n";
    SOCKET serverSocket, acceptSocket;
    int port{ 55555 };
    int  queue{ 1 };
    WSADATA wsaData;
    startWsa(wsaData);
    createServerSocket(serverSocket);
    bindSocket(serverSocket, port);
    startListen(serverSocket, queue);
    socketAccept(serverSocket, acceptSocket);
    system("pause");
    WSACleanup();
    return 0;
}

bool startWsa(WSADATA &wsaData) {
    int wsaErr;
    WORD wVersionRequested = MAKEWORD(2, 2);
    wsaErr = WSAStartup(wVersionRequested, &wsaData);
    if (wsaErr != 0) {
        std::cout << "The Winsock dll was not found" << std::endl;
        exit(EXIT_FAILURE);
    }
    else {
        std::cout << "This Winsock dll was found" << std::endl;
        std::cout << "The status: " << wsaData.szSystemStatus << std::endl;
        return true;
    }
}
bool createServerSocket(SOCKET &serverSocket) {
    serverSocket = INVALID_SOCKET;
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) {
        std::cout << "Error createServerSocket" << std::endl;
        WSACleanup;
        exit(EXIT_FAILURE);
    }
    else {
        std::cout << "socket is OK!" << std::endl;
        return true;
    }
}
bool bindSocket(SOCKET &socket, int &port) {
    sockaddr_in service;
    service.sin_family = AF_INET; 
    InetPton(AF_INET, _T("127.0.0.1"), &service.sin_addr.s_addr);
    service.sin_port = htons(port);

    if (bind(socket,(SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR) {
        std::cout << "Error in bindSocket()" << WSAGetLastError() << std::endl;
        closesocket(socket);
        WSACleanup;
        exit(EXIT_FAILURE);
    }
    else {
        std::cout << "The socket was binded ok!" << std::endl;
        return true;
    }
}

bool startListen(SOCKET &socket, int &queue) {
    if (listen(socket, queue) == SOCKET_ERROR) {
        std::cout << "There was an error in startListen()" << WSAGetLastError() << std::endl;
    }
    else {
        std::cout << "The socket is waiting for connections..."<< std::endl;
        return true;
    }
}
bool socketAccept(SOCKET& serverSocket, SOCKET& acceptSocket) {
    acceptSocket = accept(serverSocket, NULL, NULL);
    if (acceptSocket == INVALID_SOCKET) {
        std::cout << "socketAccept() failed" << WSAGetLastError() << std::endl;
        WSACleanup();
    }
    else {
        std::cout << "Accepted connection" << std::endl;
        return true;
    }
}

