#include "SessionManager.h"
#include <memory>
#include <mutex>
#include <shared_mutex>

// SessionManager의 멤버 함수들을 구현하세요.
void SessionManager::RegisterSession(SOCKET clientSocket, std::string name, std::string ip, int port) {
    std::unique_lock<std::shared_mutex> lock(this->_sMtx);
    auto newSession = std::make_unique<Session>(clientSocket, name, ip, port);
    this->_sessionMap.emplace(clientSocket, std::move(newSession));
}

std::optional<Session*> SessionManager::FindSession(SOCKET target){
    std::shared_lock<std::shared_mutex> lock(this->_sMtx);
    if (this->_sessionMap.contains(target)) {
        return this->_sessionMap[target].get();
    }
    return std::nullopt;
}

void SessionManager::RemoveSession(SOCKET target) {
    std::unique_lock<std::shared_mutex> lock(this->_sMtx);
    // 기존의 단순 erase 에 target 을 넣는 경우에는 내부 해시테이블 노드를 탐색하는 시간이 걸린다.
    // 성능적으로 미세하게 우세하기 때문에 find 를 통해 찾을 대상의 반복자를 얻고
    // 반복자가 끝을 가리키고 있지 않다면 erase 의 인자로 넣어 삭제할 대상을 찾는 오버헤드를 줄인다.
    auto it = this->_sessionMap.find(target);
    if (it != this->_sessionMap.end()) {
        it->second.reset();
        this->_sessionMap.erase(it);
    }
}

bool SessionManager::IsSendQueueEmpty(SOCKET target) 
{
    std::shared_lock<std::shared_mutex> lock(this->_sMtx);
    auto it = this->_sessionMap.find(target);
    if (it != this->_sessionMap.end()) {
        return it->second->IsSendQueueEmpty();
    }
    return true;
}

