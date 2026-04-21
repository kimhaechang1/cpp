#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>

/**
 * [실습: 서버 접속 대기]
 * 1. listen()까지 완료된 후, accept()를 호출하여 클라이언트를 기다립니다.
 * 2. 접속이 성공하면 클라이언트의 IP와 Port 정보를 출력하세요.
 */

int main() {
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(7777);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(listenSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(listenSocket, SOMAXCONN);

    std::cout << "Waiting for client connection...\n";

    // --- [오늘의 핵심: accept] ---
    sockaddr_in clientAddr{};
    int addrLen = sizeof(clientAddr);

    // 여기서 프로그램이 멈추고 손님을 기다립니다 (Blocking)
    SOCKET clientSocket = accept(listenSocket, (sockaddr*)&clientAddr, &addrLen);

    if (clientSocket != INVALID_SOCKET) {
        char clientIP[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, INET_ADDRSTRLEN);
        
        std::cout << "Connected! Client IP: " << clientIP << "\n";
        std::cout << "Client Communication Socket ID: " << clientSocket << "\n";
    }

    closesocket(clientSocket);
    closesocket(listenSocket);
    WSACleanup();

    return 0;
}
