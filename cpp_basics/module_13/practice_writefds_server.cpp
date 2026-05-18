#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main() {
    system("chcp 65001 > nul");
    WSADATA wsa; WSAStartup(MAKEWORD(2,2), &wsa);
    SOCKET listenSock = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8081);
    addr.sin_addr.s_addr = INADDR_ANY;
    bind(listenSock, (sockaddr*)&addr, sizeof(addr));
    listen(listenSock, 1);
    
    cout << "[서버] 8081 포트에서 대기중 (클라이언트가 접속하면 폭탄 드랍 예정)...\n";
    SOCKET clientSock = accept(listenSock, nullptr, nullptr);
    cout << "[서버] 클라이언트 접속 완료!\n\n";

    // 1. 송신 버퍼 크기 확인
    int sndBufSize = 0;
    int optLen = sizeof(sndBufSize);
    getsockopt(clientSock, SOL_SOCKET, SO_SNDBUF, (char*)&sndBufSize, &optLen);
    cout << "[서버] 현재 OS 송신 버퍼 크기: " << sndBufSize << " 바이트\n";

    // 2. 넌블로킹 모드 전환!
    // 블로킹 모드에서는 버퍼가 꽉 차면 스레드가 멈추지만, 넌블로킹에서는 에러를 뿜습니다.
    u_long mode = 1;
    ioctlsocket(clientSock, FIONBIO, &mode);
    cout << "[서버] 소켓을 논블로킹(Non-blocking) 모드로 강제 전환했습니다.\n\n";

    // 3. 버퍼가 꽉 찰 때까지 냅다 쏘기 (클라이언트가 안 읽어가면 꽉 참)
    char dummyData[4096];
    memset(dummyData, 'A', sizeof(dummyData)); // A로 가득 찬 4KB 블록
    
    int totalSent = 0;
    while(true) {
        int ret = send(clientSock, dummyData, sizeof(dummyData), 0);
        if (ret == SOCKET_ERROR) {
            if (WSAGetLastError() == WSAEWOULDBLOCK) {
                cout << "\n\n[서버] 🚨 헉! OS 송신 버퍼가 꽉 찼습니다! (WSAEWOULDBLOCK 발생)\n";
                cout << "[서버] 통로가 막히기 전까지 보낸 총 데이터: " << totalSent << " 바이트\n\n";
                break;
            } else {
                cout << "알 수 없는 에러: " << WSAGetLastError() << "\n";
                return 1;
            }
        }
        totalSent += ret;
        cout << "."; // 진행상황 표시
    }

    // 4. writefds에 넣고 빈 공간이 생길 때까지 대기
    cout << "[서버] 이제 writefds에 소켓을 등록하고, OS가 \"빈 공간 생겼어!\" 할 때까지 select()로 대기합니다...\n";
    fd_set writefds;
    FD_ZERO(&writefds);
    FD_SET(clientSock, &writefds);

    // select 대기 (클라이언트가 5초 뒤에 데이터를 읽어가면 빈 공간이 생기고 리턴됨)
    int selRet = select(0, nullptr, &writefds, nullptr, nullptr);
    if (selRet > 0) {
        if (FD_ISSET(clientSock, &writefds)) {
            cout << "\n[서버] 🎉 select() 리턴! (writefds 감지)\n";
            cout << "[서버] 클라이언트가 데이터를 소화해서 OS 송신 버퍼에 드디어 빈 공간이 생겼습니다!\n";
            
            // 다시 보내보기
            int ret = send(clientSock, dummyData, sizeof(dummyData), 0);
            if (ret > 0) {
                cout << "[서버] 대기 후 데이터 " << ret << " 바이트 추가 전송 완료!\n";
            }
        }
    }

    closesocket(clientSock);
    closesocket(listenSock);
    WSACleanup();
    return 0;
}
