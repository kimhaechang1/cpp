#include <iostream>
#include <string>
#include <vector>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

/*
 * [실습 목표]: UDP의 데이터 경계(Boundary) 확인
 * 
 * 1. 클라이언트는 10바이트 데이터를 3번 연속으로 sendto 합니다.
 * 2. 서버는 100바이트 크기의 큰 버퍼로 딱 한 번 recvfrom을 시도합니다.
 * 3. 결과: 서버가 30바이트를 한 번에 받는지, 아니면 첫 번째 10바이트만 받는지 확인합니다.
 */

int main() {
    // 1. Winsock 초기화
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        return 1;
    }

    // 2. UDP 소켓 생성 (SOCK_DGRAM)
    SOCKET udpSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (udpSocket == INVALID_SOCKET) {
        WSACleanup();
        return 1;
    }

    // 3. 서버 주소 설정 (자기 자신에게 보낼 예정)
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9000);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

    // 4. 서버 측: Bind (9000번 포트 점유)
    if (bind(udpSocket, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed\n";
        closesocket(udpSocket);
        WSACleanup();
        return 1;
    }

    // 5. 송신 (자기 자신에게 10바이트씩 3번 던지기)
    std::string msg1 = "Message 01"; // 10 bytes
    std::string msg2 = "Message 02"; // 10 bytes
    std::string msg3 = "Message 03"; // 10 bytes

    std::cout << "[Sender] Sending 3 messages (10 bytes each)...\n";
    sendto(udpSocket, msg1.c_str(), (int)msg1.length(), 0, (sockaddr*)&addr, sizeof(addr));
    sendto(udpSocket, msg2.c_str(), (int)msg2.length(), 0, (sockaddr*)&addr, sizeof(addr));
    sendto(udpSocket, msg3.c_str(), (int)msg3.length(), 0, (sockaddr*)&addr, sizeof(addr));

    // 6. 수신 테스트
    char buffer[100]; // 넉넉한 100바이트 버퍼
    sockaddr_in clientAddr;
    int addrLen = sizeof(clientAddr);

    std::cout << "[Receiver] Trying to recvfrom once with 100-byte buffer...\n";
    
    // 딱 한 번만 읽어봅니다.
    int receivedBytes = recvfrom(udpSocket, buffer, sizeof(buffer), 0, (sockaddr*)&clientAddr, &addrLen);

    if (receivedBytes > 0) {
        buffer[receivedBytes] = '\0';
        std::cout << "------------------------------------------\n";
        std::cout << "Received Bytes: " << receivedBytes << " bytes\n";
        std::cout << "Content: " << buffer << "\n";
        std::cout << "------------------------------------------\n";
        
        if (receivedBytes == 10) {
            std::cout << "[Result] Success! UDP preserved the boundary.\n";
            std::cout << "Even with 100-byte buffer, we only got the FIRST 10-byte datagram.\n";
        } else {
            std::cout << "[Result] Unexpected result: " << receivedBytes << " bytes received.\n";
        }
    }

    // 나머지 2개도 마저 읽어줘야 큐가 비워집니다.
    // (실습을 위해 생략하거나 루프를 돌릴 수 있습니다.)

    closesocket(udpSocket);
    WSACleanup();
    return 0;
}
