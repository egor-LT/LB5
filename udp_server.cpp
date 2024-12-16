
#include <winsock2.h>
#include <iostream>

#pragma comment(lib, "Ws2_32.lib")

int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET udpSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (udpSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed.\n";
        WSACleanup();
        return -1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(udpSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed.\n";
        closesocket(udpSocket);
        WSACleanup();
        return -1;
    }

    std::cout << "UDP server is running on port 8080...\n";

    char buffer[1024];
    sockaddr_in clientAddr;
    int clientSize = sizeof(clientAddr);

    while (true) {
        int bytesReceived = recvfrom(udpSocket, buffer, sizeof(buffer), 0, (sockaddr*)&clientAddr, &clientSize);
        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';
            std::cout << "Received: " << buffer << "\n";

            sendto(udpSocket, buffer, bytesReceived, 0, (sockaddr*)&clientAddr, clientSize); 
        }
    }

    closesocket(udpSocket);
    WSACleanup();
    return 0;
}
