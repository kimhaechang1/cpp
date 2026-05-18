#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main() {
    system("chcp 65001 > nul");
    WSADATA wsa; WSAStartup(MAKEWORD(2,2), &wsa);
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8081);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

    cout << "[느림보 클라이언트] 서버에 접속을 시도합니다...\n";
    if (connect(sock, (sockaddr*)&addr, sizeof(addr)) == 0) {
        cout << "[느림보 클라이언트] 서버 접속 성공!\n";
        cout << "[느림보 클라이언트] 저는 데이터를 바로 읽지 않고 5초간 버티겠습니다.\n";
        cout << "[느림보 클라이언트] (이때 서버의 송신 버퍼가 꽉 차게 됩니다)\n";
        
        Sleep(5000); // 5초 동안 recv 안함 -> 서버 측 OS 버퍼 꽉 참
        
        cout << "\n[느림보 클라이언트] 5초 지남! 이제 소화제(recv)를 먹어볼까요?\n";
        char buf[8192];
        int bytes = recv(sock, buf, sizeof(buf), 0); // 한 움큼 읽어서 서버 쪽에 빈 공간을 만들어줌
        
        cout << "[느림보 클라이언트] 꺼억- " << bytes << " 바이트를 읽어 서버 버퍼에 여유를 만들어 주었습니다.\n";
        
        Sleep(2000); // 서버가 writefds를 띄우고 남은 데이터를 보낼 시간을 줌
    } else {
        cout << "접속 실패\n";
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}
