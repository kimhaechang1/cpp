#pragma once
#include "Session.h"
#include <memory>
#include <shared_mutex>
#include <unordered_map>

// [클래스 설명]
// 수많은 인스턴스가 생성되더라도 메모리 상에 오직 단 하나만 존재하여 전역적으로 공유되어야 합니다.
class SessionManager {
private:
    // [세션 저장소]
    // 정렬은 무시하고 특정 Key(소켓 번호)를 통해 극강의 속도로 Value(세션) 원본을 알아내야 합니다.
    std::unordered_map<SOCKET, std::unique_ptr<Session>> _sessionMap;
    SessionManager() {

    }
    SessionManager(const SessionManager& other) = delete;
    SessionManager& operator=(const SessionManager& other) = delete;
    // static inline std::shared_ptr<SessionManager> _instance = nullptr;
    // [저장소 자물쇠]
    // 등록/검색/제거 시 멀티스레드 충돌을 막기 위한 자물쇠입니다.
    static inline std::shared_ptr<SessionManager> _instance = nullptr;
    std::shared_mutex _sMtx;
public:
    static std::shared_ptr<SessionManager> GetInstance()
    {
        if (_instance == nullptr) 
        {
            struct _inner: SessionManager {};
            _instance = std::make_shared<_inner>();
        }
        return _instance;
    }
    // static std::shared_ptr<SessionManager> GetInstance() 
    // {
    //     if (_instance == nullptr) {
    //         struct _inner: public SessionManager{};
    //         _instance = std::make_shared<_inner>();
    //     }
    //     return _instance;
    // }
    // static SessionManager& GetInstance() 
    // {
    //     static SessionManager manager;
    //     return manager;
    // }
    // [메서드: RegisterSession]
    // 새 접속자의 세션을 저장소에 등록합니다. (인자: SOCKET, 이름, IP, Port)
    // 세션 객체는 스택 수명 스코프를 벗어나는 즉시 delete를 스스로 뱉어내는 똑똑한 독점 메모리 래퍼로 관리하세요.
    void RegisterSession(SOCKET clientSocket, std::string name, std::string ip, int port);

    // [메서드: FindSession]
    // 소켓 번호로 세션을 검색합니다. 해당 소켓이 존재하지 않을 수 있으므로, 
    // '값이 있을 수도 비어있을 수도 있는' 안전상자로 포장해 반환하세요. 
    // 단, 원본의 소유권을 넘기면 안 되고 원본의 주소만 대여하세요.
    std::optional<Session*> FindSession(SOCKET target);

    // [메서드: RemoveSession]
    // 소켓 번호에 해당하는 세션을 저장소에서 완전히 제거합니다.
    void RemoveSession(SOCKET clientSocket);

    // [메서드: IsSendQueueEmpty]
    // 해당 소켓 세션의 대기열이 비어있는지 확인합니다. (writefds 가지치기 판단에 사용)
    bool IsSendQueueEmpty(SOCKET clientSocket);
};
