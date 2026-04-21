#include <WS2tcpip.h>
#include <WinSock2.h>
#include <iostream>
#include <psdk_inc/_socket_types.h>
#include <string>

/**
 * [실습: 에코 서버 구현]
 * 1. accept()로 클라이언트를 맞이합니다.
 * 2. 클라이언트가 보낸 데이터를 recv()로 받습니다.
 * 3. 받은 데이터 앞에 "[Echo] "를 붙여서 다시 send()로 보냅니다.
 */
const static inline u_short SERVER_PORT = 7777;

int main()
{
    WSAData wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        std::cout << "Cannot startup WSA" << std::endl;
        return 1;
    }
    SOCKET serverSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSock == SOCKET_ERROR)
    {
        std::cout << "Cannot Align Server Socket" << std::endl;
        return 1;
    }
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    sockaddr_in clientAddr{};

    if (bind(serverSock, (sockaddr *)&serverAddr, sizeof(serverAddr)) ==
        SOCKET_ERROR)
    {
        std::cout << "Cannot bind port with ServerSocket" << std::endl;
        return 1;
    }
    if (listen(serverSock, SOMAXCONN) == SOCKET_ERROR)
    {
        std::cout << "Cannot listen ServerSocket" << std::endl;
        return 1;
    }
    int addrSize = sizeof(clientAddr);
    SOCKET clientSock = accept(serverSock, (sockaddr *)&clientAddr, &addrSize);
    if (clientSock == SOCKET_ERROR)
    {
        std::cout << "Cannot accept client Socket" << std::endl;
        return 1;
    }
    char clientIpBuf[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &clientAddr.sin_addr, clientIpBuf, INET_ADDRSTRLEN);
    std::cout << "Success connected Client, ip=" << clientIpBuf
              << ", port=" << ntohs(clientAddr.sin_port) << std::endl;

    char recvBuf[1024];
    // 핵심 수정: recv의 반환값(실제 받은 길이)을 len에 다시 담아야 합니다!
    int len = recv(clientSock, recvBuf, sizeof(recvBuf), 0);
    
    if (len > 0)
    {
        std::cout << "Received Data! Echoing back..." << std::endl;
        send(clientSock, recvBuf, len, 0); // 받은 만큼만 돌려줌
    }

    closesocket(clientSock);
    closesocket(serverSock);

    if (WSACleanup() != 0)
    {
        std::cout << "Cannot Close WSA" << std::endl;
    }
    return 0;
}
