#pragma comment(lib, "ws2_32.lib")
#include "ThreadPool.h"
#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <winsock2.h>

// =====================================================================
// [동기 계층: Worker Thread] - 실제 무거운 게임 로직을 처리하는 곳
// =====================================================================
using namespace std::chrono_literals;

void ProcessGameLogic(SOCKET clientSocket, std::string packet)
{
    std::cout << "[Worker Thread " << std::this_thread::get_id() << "] 작업 시작: " << packet << std::endl;

    // 1. [실습] 무거운 로직(예: DB 저장)을 시뮬레이션 하기 위해
    //    현재 스레드를 0.5초(500ms) 동안 재우세요. (std::this_thread::sleep_for 사용)
    std::this_thread::sleep_for(500ms);

    // 2. [실습] 처리가 끝났다는 응답을 클라이언트에게 그대로 전송(Echo)하세요.
    //    (주의: 여기서는 편의상 워커 스레드가 직접 send를 호출하지만,
    //     실제 상용 서버에서는 송신 큐에 넣고 메인 스레드가 writefds로 처리해야 안전합니다.)
    std::string response = "[DB Saved] " + packet;
    auto responsePayload = response.c_str();
    send(clientSocket, responsePayload, response.length(), 0);

    std::cout << "[Worker Thread " << std::this_thread::get_id() << "] 작업 완료 및 전송 성공\n";
}

int main()
{
    system("chcp 65001 > nul"); // 터미널 인코딩 강제 UTF-8 변경
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    // Non-blocking 모드 전환
    u_long mode = 1;
    ioctlsocket(serverSocket, FIONBIO, &mode);

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(serverSocket, (sockaddr *)&serverAddr, sizeof(serverAddr));
    listen(serverSocket, SOMAXCONN);

    std::cout << "하이브리드 서버 시작 (Port: 8080)\n";

    // 4명의 일꾼을 가진 스레드 풀 생성 (공장 가동!)
    ThreadPool pool(4);

    fd_set masterSet;
    FD_ZERO(&masterSet);
    FD_SET(serverSocket, &masterSet);

    while (true)
    {
        fd_set copySet = masterSet;
        timeval timeout{};
        timeout.tv_sec = 0;
        timeout.tv_usec = 1000; // 1ms

        // =====================================================================
        // [비동기 계층: Main Thread] - I/O 이벤트만 감지 (관제탑)
        // =====================================================================
        int activeCount = select(0, &copySet, nullptr, nullptr, &timeout);
        if (activeCount > 0)
        {
            for (u_int i = 0; i < copySet.fd_count; ++i)
            {
                SOCKET activeSocket = copySet.fd_array[i];

                if (activeSocket == serverSocket)
                {
                    // 신규 연결 수락
                    sockaddr_in clientAddr{};
                    int clientLen = sizeof(clientAddr);
                    SOCKET clientSocket = accept(serverSocket, (sockaddr *)&clientAddr, &clientLen);
                    if (clientSocket != INVALID_SOCKET)
                    {
                        std::cout << "[Main Thread] 클라이언트 접속: " << clientSocket << std::endl;
                        FD_SET(clientSocket, &masterSet);
                    }
                }
                else
                {
                    // 데이터 수신
                    char buffer[256] = {0};
                    int bytesReceived = recv(activeSocket, buffer, sizeof(buffer) - 1, 0);

                    if (bytesReceived > 0)
                    {
                        std::string packet(buffer);
                        std::cout << "[Main Thread] 데이터 수신 완료. 공장으로 전달합니다.\n";

                        // 3. [실습] 메인 스레드는 조립된 'packet'과 'activeSocket' 정보를
                        //    스레드 풀(pool)의 작업 큐에 던져넣고(EnqueueJob) 즉시 루프를 빠져나와야 합니다.
                        //    힌트: 람다 함수(Lambda)를 만들어 ProcessGameLogic 을 호출하도록 던지세요.
                        buffer[bytesReceived] = '\0';
                        // 주의, 다른 스레드에서 동작하는것이기 때문에 주소만 넘겨받아 처리할때는 조심해야한다.
                        // 특히, 지금 참조하려는 지역변수가 종료될 가능성이 존재하는 블럭 내에 있다면 댕글링 포인터로
                        // 터진다.
                        pool.EnqueueJob([activeSocket, buffer]() { ProcessGameLogic(activeSocket, buffer); });
                    }
                    else if (bytesReceived == 0 ||
                             (bytesReceived == SOCKET_ERROR && WSAGetLastError() != WSAEWOULDBLOCK))
                    {
                        std::cout << "[Main Thread] 클라이언트 종료: " << activeSocket << std::endl;
                        closesocket(activeSocket);
                        FD_CLR(activeSocket, &masterSet);
                    }
                }
            }
        }
    }

    closesocket(serverSocket);
    WSACleanup();
    return 0;
}
