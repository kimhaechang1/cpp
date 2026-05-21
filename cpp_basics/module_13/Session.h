#pragma once
#include <mutex>
#include <optional>
#include <queue>
#include <string>
#include <string_view>
#include <winsock2.h>

// 이 파일에서는 유저(클라이언트) 한 명의 연결 상태와 데이터를 관리하는 Session 클래스를 설계합니다.

class Session
{
    // [멤버 변수 (소켓)]
    // 이 세션이 통신하는 대상의 OS 핸들(식별자)을 보관합니다.
    SOCKET _clientSock;

    // [멤버 변수 (IP/Port)]
    // 접속 시 상대방이 알려준 네트워크 주소 정보를 기억합니다. (M12 sockaddr_in 관련 문자열/정수 보관)
    int _port;
    std::string _ip;

    // [멤버 변수 (이름)]
    // 플레이어의 닉네임을 보관합니다.
    std::string _name;

    // [멤버 변수 (HP)]
    // 플레이어의 현재 생존 수치를 보관합니다. (기본값 100)
    int _hp;

    // [멤버 변수 (SendQueue)]
    // 이 유저에게 전송할 응답들을 선입선출(FIFO) 순서로 줄세우는 대기열입니다. (문자열 보관)
    std::queue<std::string> _sendQueue;

    // [멤버 변수 (Lock)]
    // 여러 스레드가 동시에 이 대기열에 접근할 때 충돌을 방지할 자물쇠입니다.
    std::mutex _mtx;

public:
    // [생성자]
    // 소켓, 이름, IP, Port를 받아 초기화하세요. HP는 100으로 시작합니다.
    Session(SOCKET clientSocket, std::string name, std::string ip, int port)
        : _clientSock(clientSocket), _name{name}, _ip{ip}, _port{port}, _hp{100}
    {
    }

    // [메서드: PushToSendQueue]
    // 대기열의 맨 뒤에 응답 데이터를 안전하게 추가합니다.
    void PushToSendQueue(std::string responseData)
    {
        std::lock_guard<std::mutex> lock(this->_mtx);
        this->_sendQueue.push(responseData);
    }

    // [메서드: PopFromSendQueue]
    // 대기열의 맨 앞에서 데이터를 꺼냅니다. 대기열이 비어있을 수도 있으므로,
    // '값이 있을 수도 비어있을 수도 있는' 안전상자로 포장해 반환하세요.
    std::optional<std::string> PopFromSendQueue()
    {
        if (this->_sendQueue.empty()) 
        {
            return std::nullopt;
        }
        auto rs = this->_sendQueue.front();
        this->_sendQueue.pop();
        return rs;
    }

    // [메서드: GetHp, SetHp]
    // 체력 관련 기본 getter/setter
    const int GetHp() const {
        return this->_hp;
    }
    void SetHp(int hp) {
        this->_hp = hp;
    }

    // [메서드: GetName]
    // 플레이어 이름 반환
    const std::string_view GetName() const {
        return this->_name;
    }
    
    bool IsSendQueueEmpty() {
        return this->_sendQueue.empty();
    }
};
