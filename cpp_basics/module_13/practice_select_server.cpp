#include <_timeval.h>
#include <cstddef>
#include <cstring>
#include <ctime>
#include <iostream>
#include <vector>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

/*
[실습: Select 기반 다중 접속 에코 서버 구현]

1. Winsock 초기화 및 Listen Socket 생성 코드는 기본 제공됩니다.
2. 클라이언트들의 소켓을 관리할 동적 배열을 선언하세요.
   - [힌트] 데이터의 원본들을 1차원으로 순차적, 연속적으로 보관하는 가변 길이 컨테이너를 쓰세요.

3. 무한 루프(while) 안에서 다음 흐름을 코드로 번역해보세요.
   가. fd_set 구조체를 선언하고, 관찰 목록을 완전히 비웁니다. (초기화)
   나. Listen Socket을 관찰 목록에 추가합니다.
   다. 현재 배열에 보관된 모든 Client Socket들을 순회하며 관찰 목록에 추가합니다.
   라. select() 함수를 호출하여 수신(Read) 이벤트를 기다립니다. (타임아웃은 nullptr로 무한대기)

   마. select()가 리턴되면, 가장 먼저 Listen Socket이 살아남았는지(이벤트 발생) 확인합니다.
       - 살아남았다면: 새 클라이언트를 accept() 하고, 뱉어낸 소켓을 동적 배열에 보관합니다.

   바. 그 다음, 배열에 보관된 클라이언트 소켓들을 순회하며 살아남은 놈(데이터 보냄)이 있는지 확인합니다.
       - 살아남았다면: recv()를 호출하여 데이터를 읽습니다.
       - 만약 recv() 리턴값이 0 이하라면(연결 종료 또는 에러): closesocket() 후 배열에서 제거합니다.
       - 데이터를 잘 읽었다면: 받은 데이터를 그대로 다시 send() 해줍니다. (Echo)

⚠️ 블랭크 슬레이트 규칙:
- `FD_ZERO`, `FD_SET`, `FD_ISSET` 등의 매크로와 `vector`의 메서드를 직접 조합해보세요.
- 컴파일이 실패해도 좋으니 일단 구조를 잡아보는 것이 핵심입니다!
*/

int main()
{
    system("chcp 65001 > nul"); // 터미널 인코딩 강제 UTF-8 변경
    // 1. Winsock 초기화
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        return 1;

    // 2. Listen 소켓 생성 및 바인딩
    SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(listenSocket, (SOCKADDR *)&serverAddr, sizeof(serverAddr));
    listen(listenSocket, SOMAXCONN);

    std::cout << "[서버] 8080 포트에서 대기 중... (Select 모델)\n";

    // TODO 1: 클라이언트 소켓들을 보관할 컨테이너 선언
    vector<SOCKET> clients;
    fd_set readfds;
    fd_set exceptfds;
    while (true)
    {
        // TODO 2: fd_set 초기화 및 세팅
        FD_ZERO(&readfds);
        FD_ZERO(&exceptfds);
        FD_SET(listenSocket, &readfds);

        timeval val(10);

        // TODO 3: select() 호출
        for (int i = 0; i < clients.size(); i++)
        {
            FD_SET(clients[i], &readfds);
        }
        select(1, &readfds, NULL, &exceptfds, nullptr);

        // TODO 4: Listen 소켓 접속 이벤트 처리
        if (FD_ISSET(listenSocket, &readfds))
        {
            sockaddr clientAddr;
            int clientAddrLen = sizeof(clientAddr);
            SOCKET cSock = accept(listenSocket, &clientAddr, &clientAddrLen);
            if (cSock != SOCKET_ERROR)
            {
                clients.push_back(cSock);
            }
        }

        // TODO 5: Client 소켓 통신(recv/send) 및 접속 종료 이벤트 처리
        for (int i = 0; i < clients.size(); i++)
        {
            if (FD_ISSET(clients[i], &readfds))
            {
                char recBuf[1024];
                // 마지막에 '\0' 넣어줘야함
                int recvBytes = recv(clients[i], recBuf, sizeof(recBuf) - 1, 0);
                if (recvBytes <= 0)
                {
                    // 만약에 ISSET 으로 감지한 것이 0이하 값 즉 종료신호가 온것이라면 연결을 끊는다.
                    closesocket(clients[i]);
                    clients.erase(clients.begin() + i);
                    i--;
                }
                else
                {
                    recBuf[recvBytes] = '\0';
                    std::cout << "[수신] " << recBuf << std::endl;
                    send(clients[i], recBuf, recvBytes, 0);
                }
            }
        }
    }

    // 종료 정리
    closesocket(listenSocket);
    WSACleanup();
    return 0;
}
