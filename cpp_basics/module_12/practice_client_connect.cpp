#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>

/**
 * [실습: 클라이언트 접속 시도]
 * 1. 서버 주소(127.0.0.1:7777) 정보를 설정합니다.
 * 2. connect()를 호출하여 서버에 접속을 요청합니다.
 */

int main() {
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // TODO: 서버 주소 설정 (127.0.0.1, Port 7777)
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(7777);
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

    std::cout << "Connecting to server...\n";

    // --- [오늘의 핵심: connect] ---
    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cout << "Connection Failed! Error: " << WSAGetLastError() << "\n";
        return -1;
    }

    std::cout << "Successfully connected to server!\n";

    closesocket(clientSocket);
    WSACleanup();

    return 0;
}
