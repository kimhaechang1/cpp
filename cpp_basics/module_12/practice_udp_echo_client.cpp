#include <WS2tcpip.h>
#include <iostream>
#include <minwindef.h>
#include <psdk_inc/_ip_types.h>
#include <psdk_inc/_socket_types.h>
#include <winsock2.h>

int main()
{
    WSAData wsaData;
    int wsaStartupRs = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (wsaStartupRs != 0)
    {
        WSACleanup();
        return 1;
    }

    SOCKET udpClient = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (udpClient == SOCKET_ERROR)
    {
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(9000);
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
    std::string msg = "Hello this is UDP TEST";

    int sendRs = sendto(udpClient, msg.c_str(), msg.length(), 0,
                        (sockaddr *)&serverAddr, sizeof(serverAddr));

    std::cout << "Client Sent: " << msg << std::endl;
    sockaddr_in fromAddr{};
    int fromAddrLen = sizeof(fromAddr);
    char recvBuf[1024];

    int recvLen = recvfrom(udpClient, recvBuf, sizeof(recvBuf), 0,
                           (sockaddr *)&fromAddr, &fromAddrLen);
    char fromIp[INET_ADDRSTRLEN];
    int fromIpLen = sizeof(fromIp);
    inet_ntop(AF_INET, &fromAddr.sin_addr, fromIp, fromIpLen);
    if (recvLen > 0)
    {
        recvBuf[recvLen] = '\0';
        std::cout << "Client Received: " << recvBuf << " from Addr: " << fromIp
                  << std::endl;
    }

    closesocket(udpClient);
    WSACleanup();
}