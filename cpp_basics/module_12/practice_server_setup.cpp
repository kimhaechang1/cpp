#include <WS2tcpip.h>
#include <WinSock2.h>
#include <cassert>
#include <iostream>
#include <psdk_inc/_ip_types.h>
#include <psdk_inc/_socket_types.h>

/**
 * [실습 과제: 서버 소켓 개통하기]
 * 1. TCP 소켓을 생성하세요.
 * 2. 주소 구조체(sockaddr_in)를 설정하세요. (Port: 7777)
 * 3. 소켓에 주소를 부여(bind)하세요.
 * 4. 소켓을 대기 상태(listen)로 만드세요.
 *
 * [검증 포인트]
 * - bind()와 listen()의 반환값을 체크하여 성공 여부를 확인하세요.
 * - 소켓 오류 발생 시 WSAGetLastError()를 통해 에러 코드를 출력하세요.
 */

int main()
{
    // Winsock 초기화
    WSAData wsa;
    if ((WSAStartup(MAKEWORD(2, 2), &wsa)) != 0)
    {
        std::cout << "WSAStartup failed" << std::endl;
        return 1;
    }

    // TODO 1: 소켓 생성 (지난 실습 복습)
    // SOCKET listenSocket = ...
    SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenSocket == INVALID_SOCKET)
    {
        std::cout << "Cannot Open SOCKET" << std::endl;
        return 1;
    }

    // TODO 2: 주소 구조체 설정
    // hint: sockaddr_in addr;
    // hint: htons(7777)를 잊지 마세요!
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(7777);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // TODO 3: Bind (사서함 등록)
    // if (bind(...) == SOCKET_ERROR) ...
    if (bind(listenSocket, (sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR)
    {
        std::cout << "Cannot Bind Socket with port" << std::endl;
        return 1;
    }

    // TODO 4: Listen (손님 대기 통로 확보)
    // if (listen(...) == SOCKET_ERROR) ...
    if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR)
    {
        std::cout << "Cannot Listen Socket" << std::endl;
        return 1;
    }

    std::cout << "Server is now LISTENING on port 7777...\n";
    std::cout << "Keep this program running and check 'netstat -ano | findstr "
                 ":7777' in another terminal.\n";

    // 잠시 대기 (서버가 바로 꺼지지 않게)
    system("pause");

    // 자원 반납
    // closesocket(listenSocket);
    WSACleanup();

    return 0;
}
