#include "updserver.h"

bool UDPServer::startWsa() {
    int wsaErr;
    WORD wVersionRequested = MAKEWORD(2, 2);
    wsaErr = WSAStartup(wVersionRequested, &wsaData);
    if (wsaErr != 0) {
        std::cout << "The Winsock dll was not found" << std::endl;
        return false;
    }
    else {
        std::cout << "This Winsock dll was found" << std::endl;
        std::cout << "The status: " << wsaData.szSystemStatus << std::endl;
        return true;
    }
}
bool UDPServer::createServerSocket() {
    serverSocket = INVALID_SOCKET;
    serverSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    u_long mode = 1;  // 1 to enable non-blocking socket
    ioctlsocket(serverSocket, FIONBIO, &mode);
    if (serverSocket == INVALID_SOCKET) {
        std::cout << "Error createServerSocket" << std::endl;
        WSACleanup();
        return false;
    }
    else {
        std::cout << "socket is OK!" << std::endl;
        return true;
    }
}
bool UDPServer::bindSocket() {
    sockaddr_in service{};
    service.sin_family = AF_INET;
    InetPton(AF_INET, _T("127.0.0.1"), &service.sin_addr.s_addr);
    service.sin_port = htons(port);

    if (bind(serverSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR) {
        std::cout << "Error in bindSocket()" << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return false;
    }
    else {
        std::cout << "The socket was binded ok!" << std::endl;
        return true;
    }
}

bool UDPServer::recieveData() {
    char buffer[512] = "";
    sockaddr_in clientAddress{};
    int recvbuflen = 512;
    int clientAddress_length = (int)sizeof(clientAddress);

    int bytes_received = recvfrom(serverSocket, buffer, recvbuflen, 0, (struct sockaddr*)&clientAddress, &clientAddress_length);
    if (WSAGetLastError() != WSAEWOULDBLOCK) {
        if (bytes_received < 0) {
            std::cout << "Error recieving datagram from client:" << std::endl;
            WSACleanup();
            return false;
        }
        else {
            std::cout << "Recieved: " << buffer << std::endl;
            std::cout << "Recieved: " << bytes_received << std::endl;
            return true;
        }
    }
}

bool UDPServer::sendData(sockaddr_in& clientsAddress, char* buffer) {

    int bytesSent = sendto(serverSocket, (const char*)buffer, strlen(buffer), 0, (struct sockaddr*)&clientsAddress, sizeof(clientsAddress));
    if (bytesSent == -1) {
        std::cout << "Error sending data to server: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return false;
    }
    else {
        std::cout << "Data sent: " << buffer << std::endl;
        std::cout << "Bytes sent: " << bytesSent << std::endl;
        return true;
    }
}

void UDPServer::closeServer() {
    closesocket(serverSocket);
    WSACleanup();
}

bool UDPServer::startServer() {
    if(!startWsa())
        return false;
    if(!createServerSocket())
        return false;
    if(!bindSocket())
        return false;
    return true;
}
