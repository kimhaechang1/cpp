#include <WS2tcpip.h>
#include <WinSock2.h>
#include <iostream>
#include <string>

/**
 * [실습: 토크 클라이언트 구현]
 * 1. 서버에 connect() 합니다.
 * 2. 사용자로부터 문자열을 입력받아 서버에 send() 합니다.
 * 3. 서버가 돌려준 에코 메시지를 recv()로 받아서 출력합니다.
 */

int main()
{
    WSAData wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        std::cout << "Cannot startup WSA" << std::endl;
        return 1;
    }

    SOCKET clientSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSock == INVALID_SOCKET)
    {
        std::cout << "Cannot create Client Socket" << std::endl;
        return 1;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(7777);
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

    std::cout << "Connecting to Server (127.0.0.1:7777)..." << std::endl;

    if (connect(clientSock, (sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
    {
        std::cout << "Connect Failed! Error: " << WSAGetLastError() << std::endl;
        return 1;
    }

    // --- [대화 시작] ---
    std::string msg = "Hello Server! This is a message from Client.";
    
    // 1. 서버로 메시지 전송
    int sentLen = send(clientSock, msg.c_str(), (int)msg.length(), 0);
    if (sentLen > 0)
    {
        std::cout << "Sent Message: " << msg << std::endl;
    }

    // 2. 서버의 응답(Echo) 대기
    char recvBuf[1024];
    int recvLen = recv(clientSock, recvBuf, sizeof(recvBuf) - 1, 0);

    if (recvLen > 0)
    {
        recvBuf[recvLen] = '\0'; // 문자열 끝 처리
        std::cout << "Server Response: " << recvBuf << std::endl;
    }

    std::cout << "Closing Connection..." << std::endl;

    closesocket(clientSock);
    WSACleanup();

    return 0;
}
