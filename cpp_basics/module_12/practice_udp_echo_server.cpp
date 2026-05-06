#include <WS2tcpip.h>
#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

/*
 * [실습]: UDP Echo Server 만들기
 *
 * [요구사항]
 * 1. 9000번 포트로 UDP 소켓을 바인드하세요.
 * 2. 루프를 돌며 클라이언트로부터 데이터를 받으세요 (recvfrom).
 * 3. 데이터를 보낸 클라이언트의 IP와 Port 정보를 출력하세요.
 * 4. 받은 데이터를 그대로 클라이언트에게 다시 돌려주세요 (sendto).
 * 5. "exit" 메시지를 받으면 서버를 종료하세요.
 */

int main()
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        return 1;

    // 1. 소켓 생성 (UDP)
    SOCKET serverSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (serverSock == INVALID_SOCKET)
        return 1;

    // 2. 서버 주소 설정 및 Bind
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(9000);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSock, (sockaddr *)&serverAddr, sizeof(serverAddr)) ==
        SOCKET_ERROR)
    {
        std::cerr << "Bind failed\n";
        return 1;
    }

    std::cout << "UDP Echo Server started on port 9000...\n";

    char recvBuf[1024];
    sockaddr_in clientAddr;
    int clientAddrLen = sizeof(clientAddr);

    while (true)
    {
        // [TODO]: recvfrom을 사용하여 데이터를 받으세요.
        // int len = recvfrom(...);

        /* 힌트:
        int len = recvfrom(serverSock, recvBuf, sizeof(recvBuf) - 1, 0,
                           (sockaddr*)&clientAddr, &clientAddrLen);
        */

        // [구현 시작]
        int len = recvfrom(serverSock, recvBuf, sizeof(recvBuf) - 1, 0,
                           (sockaddr *)&clientAddr, &clientAddrLen);
        // [구현 끝]
        clientAddrLen = sizeof(clientAddr);

        if (len <= 0)
            break;

        recvBuf[len] = '\0';

        // [TODO]: 클라이언트의 IP 주소를 문자열로 변환하여 출력하세요
        // (inet_ntop 사용).
        char clientIp[INET_ADDRSTRLEN];
        // [구현 시작]
        inet_ntop(AF_INET, &clientAddr.sin_addr, clientIp, sizeof(clientIp));

        // [구현 끝]

        std::cout << "[Received from " << clientIp << ":"
                  << ntohs(clientAddr.sin_port) << "]: " << recvBuf << "\n";

        // [TODO]: 받은 데이터를 그대로 다시 보내세요 (sendto).
        // [구현 시작]
        sendto(serverSock, recvBuf, len, 0, (sockaddr *)&clientAddr,
               clientAddrLen);

        // [구현 끝]

        if (std::string(recvBuf) == "exit")
            break;
    }

    closesocket(serverSock);
    WSACleanup();
    return 0;
}
