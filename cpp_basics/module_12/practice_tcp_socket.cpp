#include <WinSock2.h>
#include <cassert>
#include <io.h>
#include <iostream>

/**
 * [실습 과제]
 * 1. 운영체제로부터 TCP 통신용 소켓 핸들을 할당받는 함수를 작성하세요.
 * 2. 생성된 소켓이 유효한지 검증하는 로직을 포함하세요.
 * 3. 사용이 끝난 소켓은 반드시 운영체제에 반납(Close)해야 합니다.
 *
 * [제한 사항]
 * - IPv4 주소 체계를 사용하세요.
 * - 연결 지향형 스트림 타입을 사용하세요.
 * - TCP 프로토콜을 명시하세요.
 */

// TODO: 아래 함수들을 직접 구현하세요. (선언부도 직접 작성해야 합니다.)
// 힌트:
// - 성공 시 SOCKET 핸들을 반환하고 실패 시 INVALID_SOCKET을 반환하는 함수
// - 소켓을 닫고 리소스를 정리하는 함수
SOCKET OpenTCPSocket()
{
    return socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
}

void CloseTCPSocket(SOCKET socket)
{
    closesocket(socket);
}

int main()
{
    // [1] Winsock 초기화 (실습 편의를 위해 직접 작성)
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        std::cout << "Winsock Init Failed\n";
        return -1;
    }

    std::cout << "--- Socket Creation Test ---\n";

    // [2] 실습: 여기에 작성한 함수를 호출하여 소켓을 생성하세요.
    // ---------------------------------------------------------
    SOCKET mySocket = OpenTCPSocket();
    // ---------------------------------------------------------

    // [3] 검증 코드 (테스트 코드 수정 금지)
    assert(mySocket != INVALID_SOCKET);
    std::cout << "Success: Socket created with handle [" << mySocket << "]\n";
    std::cout << "Handle Memory Size: " << sizeof(mySocket) << "bytes\n";

    // [4] 자원 반납
    CloseTCPSocket(mySocket);

    WSACleanup();
    std::cout << "--- Test Finished ---\n";

    return 0;
}
