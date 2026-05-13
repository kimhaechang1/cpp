#include "SessionManager.h"
// [코드 평가 피드백] (헤더 관리)
// 이 파일에 있는 `<mutex>`, `<shared_mutex>`, `<vector>` 등은 이미 `SessionManager.h`에서 인클루드 한 것들입니다.
// 구현부(.cpp)에서는 헤더에서 포함하지 않은 정말 필요한 라이브러리만 인클루드해야 코드의 종속성이 우아해집니다.
#include <functional>
#include <mutex>
#include <optional>
#include <shared_mutex>
#include <utility>
#include <vector>

// 이 파일에 SessionManager의 구현을 작성하세요.
ProtocolType SessionManager::RouteMessage(MessageType type)
{
    if (type == MessageType::Login || type == MessageType::LootReport)
    {
        return ProtocolType::TCP;
    }
    return ProtocolType::UDP;
}
void SessionManager::RegisterSession(const int id, const std::string name, const int hp)
{
    std::lock_guard<std::shared_mutex> guard(_lock);
    // lock_guard 를 씌우면 락 사용자가 unlock 을 까먹더라도 작동을 도와준다.
    // 마치 스마트포인터 처럼 작동한다고 생각하면 된다.

    // [코드 평가 피드백] (불필요한 임시 객체 생성 및 복사 비용 방지)
    // (문제점) 맵에 넣기 위해 `{id, std::make_pair(name, hp)}`로 임시 객체를 생성하여 `insert`하고 있습니다.
    // (왜 발생하는가?) `insert` 함수는 이미 완성된 덩어리(Key-Value 쌍 객체)를 매개변수로 받습니다. 즉, 밖에서 임시
    // 객체(`make_pair`)를 한 번 만들고, 맵의 내부 트리(또는 해시) 노드 메모리로 그 임시 객체의 데이터를 다시
    // '복사'하거나 '이동'시키는 두 번의 오버헤드가 일어납니다. (수정 방법) C++17의 권장 문법인 `try_emplace`를
    // 사용하세요. 키(id)만 먼저 검색하고, 만약 없다면 인자들(`name`, `hp`)을 전달해 맵 내부의 진짜 노드 안에서 객체를
    // '직접 조립(In-place Construction)'하므로 밖에서 임시 객체를 만들고 복사하는 비용이 0이 됩니다. 수정 예시:
    // `this->userSessionMap.try_emplace(id, name, hp);`
    // this->userSessionMap.insert({id, std::make_pair(name, hp)});
    // 즉, 기존방식에서는 make_pair 로 pair rvalue 만들고 내부적인 id 에 대응되는 해시노드에 복사하거나 이동시키기에,
    // 생성 후 이동 혹은 복사 라는 2과정이 생긴다. 그것이 아니라 내부에 id 에 대응되는 위치에 생성만 한다면
    // 효율적일것이다. 그기능이 vector 에서는 emplace_back 이었다면, unordered_map 에서는 emplace 와  try_emplace() 가
    // 있다.
    // try_emplace 를 하게되면 key 에 해당하는 value 가 없을때에만 insertion 된다.
    this->userSessionMap.try_emplace(id, name, hp);
}

std::optional<std::string_view> SessionManager::GetSessionName(const int id)
{
    std::shared_lock<std::shared_mutex> shared(_lock);
    if (auto it = this->userSessionMap.find(id); it != this->userSessionMap.end())
    {
        return it->second.first; // 이미 찾은 노드의 주소(it)에서 곧바로 빼옵니다.
    }
    return std::nullopt;
}
void SessionManager::RemoveSession(const int id)
{
    std::lock_guard<std::shared_mutex> guard(_lock);
    this->userSessionMap.erase(id);
}
std::vector<int> SessionManager::FilterSessions(std::function<bool(int, int)> fn)
{
    std::shared_lock<std::shared_mutex> shared(_lock);
    std::vector<int> result;
    // [코드 평가 피드백] (벡터 할당 오버헤드)
    // (문제점) 반복문을 돌면서 조건에 맞을 때마다 `push_back`을 하면 내부적으로 메모리를 새로 파고 이사하는 작업이
    // 일어날 수 있습니다. (개선 방향) `result.reserve(userSessionMap.size() / 2);` 처럼 적당한 크기를 미리 예상해서
    // 예약해두면, 동적 메모리 재할당 오버헤드를 아예 없앨 수 있습니다.
    result.reserve(userSessionMap.size());
    for (const auto &[id, info] : this->userSessionMap)
    {
        if (fn(id, info.second))
        {
            result.push_back(id);
        }
    }
    return result;
}