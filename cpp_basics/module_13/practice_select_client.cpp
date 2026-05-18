#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main() {
    system("chcp 65001 > nul"); // 터미널 인코딩 강제 UTF-8 변경
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

    if (connect(sock, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        cout << "[클라이언트] 서버 접속 실패\n";
        return 1;
    }

    cout << "[클라이언트] 서버(8080)에 접속 성공! 메시지를 입력하세요 (종료: q)\n";

    while (true) {
        cout << "> ";
        string msg;
        getline(cin, msg);
        
        if (msg == "q" || msg == "Q") break;
        if (msg.empty()) continue;

        send(sock, msg.c_str(), msg.length(), 0);

        char buf[1024];
        int bytes = recv(sock, buf, sizeof(buf) - 1, 0);
        if (bytes <= 0) {
            cout << "[클라이언트] 서버와 연결이 끊어졌습니다.\n";
            break;
        }
        buf[bytes] = '\0';
        cout << "[서버로부터 메아리] " << buf << "\n";
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}
