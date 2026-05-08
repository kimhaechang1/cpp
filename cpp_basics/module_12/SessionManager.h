#pragma once
#include "GameMessage.h"
#include <functional>
#include <map>
#include <shared_mutex>
#include <string>
#include <utility>
#include <vector>

// [암시적 가이드]
// 접속한 플레이어를 특정 Key를 통해 극강의 속도로 등록/조회/제거합니다.
// 병렬 실행되는 작업자들이 동시에 접근해도 데이터가 오염되지 않아야 합니다.
//
// 세션 필터링: 함수를 외부에 억지로 빼지 말고, 그 즉시 현장에서 이름 없는 1회용
// 로직 덩어리를 만들어 필터링 조건으로 던져 넣으세요. 메시지 라우팅: 메시지
// 종류에 따라 전송 프로토콜을 결정합니다. 한정된 카테고리 각각에 대해 빠짐없이
// 분기 처리하세요.
/* SessionManager 클래스를 구현하세요. */

class SessionManager
{
  private:
    // [코드 평가 피드백] (자료구조 선택의 우아함)
    // (문제점) `std::map`은 내부가 트리 구조라 탐색에 O(log N)의 시간이 걸립니다.
    // (개선 방향) 세션 조회가 1초에 수십만 번 일어나는 서버 환경에서는 탐색 속도가 완벽한 O(1)인 해시 테이블 기반의 **`std::unordered_map`**을 사용하는 것이 정석입니다. "극강의 속도"를 달성하려면 반드시 변경해야 합니다.
    std::map<int, std::pair<std::string, int>> userSessionMap;
    std::shared_mutex
        _lock; // mutex 와의 차이는 읽기용 쓰기용 구멍이 나눠진 자물쇠이다.

  public:
    ProtocolType RouteMessage(MessageType type);
    void RegisterSession(const int id, const std::string name, const int hp);
    
    // [코드 평가 피드백] (힙 메모리 할당 방지)
    // (문제점) 단순 이름을 조회하는 용도임에도 `std::string` 객체를 통째로 반환하고 있습니다. 
    // (왜 발생하는가?) `std::string`은 반환될 때 문자열 데이터를 복사하기 위해 새로운 메모리 공간(Heap Allocation)을 운영체제에 요청합니다. 만약 GUI 리스트를 갱신하느라 이 함수를 초당 수만 번 호출한다면, 힙 메모리를 수만 번 할당/해제해야 하므로 가비지 컬렉션(Garbage Collection/Allocator) 오버헤드로 서버가 멈칫거리거나(Lag) 엄청난 지연이 발생합니다.
    // (수정 방법) C++17의 핵심 기능인 `std::string_view`를 활용하세요. 이는 원본 문자열을 복사하지 않고 원본의 주소(포인터)와 길이만을 묶어서 8~16바이트 크기로 아주 가볍게 반환하는 '유리창' 같은 객체입니다. 
    // 수정 예시: `std::optional<std::string_view> GetSessionName(const int id);` 로 리턴 타입 변경.
    std::optional<std::string> GetSessionName(const int id);
    void RemoveSession(const int id);
    std::vector<int> FilterSessions(std::function<bool(int, int)> fn);
};
