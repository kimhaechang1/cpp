#include <iostream>
#include <WinSock2.h>
#include <iomanip>
#include <cstdint>

/**
 * [엔디언 변환 검증 실습]
 * - htons: 16비트 (2바이트) 변환
 * - htonl: 32비트 (4바이트) 변환
 */

int main() {
    // Winsock 초기화 (이 함수들을 쓰려면 필수입니다)
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    // 1. 16비트 포트 번호 테스트 (7777 = 0x1E61)
    uint16_t port = 7777; 
    uint16_t networkPort = htons(port);

    // 2. 32비트 IP 주소 형태 테스트 (0x12345678)
    uint32_t val = 0x12345678;
    uint32_t networkVal = htonl(val);

    std::cout << "========================================\n";
    std::cout << "          Endianness Check              \n";
    std::cout << "========================================\n";

    // 16비트 결과 출력
    std::cout << "[16-bit (htons)]\n";
    std::cout << "Host    : 0x" << std::hex << std::setw(4) << std::setfill('0') << port << "\n";
    std::cout << "Network : 0x" << std::hex << std::setw(4) << std::setfill('0') << networkPort << "\n\n";

    // 32비트 결과 출력
    std::cout << "[32-bit (htonl)]\n";
    std::cout << "Host    : 0x" << std::hex << std::setw(8) << std::setfill('0') << val << "\n";
    std::cout << "Network : 0x" << std::hex << std::setw(8) << std::setfill('0') << networkVal << "\n";

    std::cout << "========================================\n";
    if (port != networkPort) {
        std::cout << "Result: Your CPU is Little-Endian.\n";
        std::cout << "Bytes were swapped for Network (Big-Endian)!\n";
    }

    WSACleanup();
    return 0;
}
