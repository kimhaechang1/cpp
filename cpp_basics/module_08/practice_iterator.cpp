#include <iostream>
#include <string>
#include <cassert>
#include<vector>
#include<format>
#include<map>

// [주의] 정답 자료구조(타입) 헤더는 직접 #include 하세요.

// =========================================================================
// [실무 프로젝트 상황: 반복자(Iterator)를 이용한 범용 순회 로직 작성]
// 
// 현업의 C++ 게임 서버에서는 데이터 컨테이너의 내부 구조 결함(캐시 미스 등)을
// 보완하기 위해, 개발 도중 자료구조를 다른 것으로(예: map -> unordered_map, list -> vector)
// 전면 교체하는 일이 아주 빈번하게 일어납니다.
//
// 이때 뿔뿔이 흩어진 메모리들을 안전하고 일관된 단일 방식으로 훑기 위해서
// 강제되는 문법이 바로 '보편적 순회용 추상화 포인터' 입니다.
// 아래 두 가지 서로 완전히 다른 구조를 가진 데이터를 하나의 통일된 방식으로 순회해보세요.
// =========================================================================

// -------------------------------------------------------------------------
// [요구사항 1: 인벤토리 아이템 출력]
//
// 함수 이름: PrintAllItems
// 매개변수: [데이터의 원본들을 1차원으로 순차적, 연속적으로 보관하는 가변 길이를 가져야 합니다. (int형 아이템 ID 보관)]
//           (단, 함수 호출이 끝난 후에도 외부에 있는 원본 데이터가 수정된 결과를 온전히 보존해야 합니다. / 성능을 위해 복사를 피하세요.)
// 반환값: 없음(void)
// 
// 기능:
// - 자료구조의 겉모습이 배열인지 트리인지 무관하게, 내부 요소를 가리키고 전진할 수 있는 
//   보편적 순회용 추상화 포인터를 사용하세요.
// - 첫 번째 원소부터 마지막 원소 직전(끝 방어벽)까지 순회하며 아이템 ID를 출력합니다.
// - (힌트: 반드시 "전위 증가"를 사용하여 성능을 챙기세요)
// -------------------------------------------------------------------------
// TODO: PrintAllItems 함수를 구현하세요.
void PrintAllItems(std::vector<int>& itemIds) {
    for (auto it = itemIds.begin(); it != itemIds.end(); ++it) {
        std::cout << std::format("ID={}", *it) << std::endl;
    }
}

// -------------------------------------------------------------------------
// [요구사항 2: 플레이어 스탯 일괄 업그레이드]
//
// 함수 이름: UpgradeAllStats
// 매개변수: [항상 정렬된 상태를 자동 유지하며 특정 Key(std::string)로 Value(int)를 찾습니다.]
//           (단, 원본을 직접 수정해야 하므로 안전한 접근 방식을 취하세요.)
// 반환값: 없음(void)
//
// 기능:
// - 보편적 순회용 추상화 포인터를 사용하여 모든 스탯 구간을 순회합니다.
// - 순회하면서 내부 요소의 'Value(값)'에 10씩 더해줍니다.
// - (힌트: 이 자료구조의 반복자는 Key와 Value를 한 켤레로 가지고 다닙니다. 화살표 연산자를 떠올려보세요.)
// -------------------------------------------------------------------------
// TODO: UpgradeAllStats 함수를 구현하세요.
void UpgradeAllStats(std::map<std::string, int>& statMap) {
    for (auto it = statMap.begin(); it != statMap.end(); ++it) {
        (*it).second += 10;
    }
}


int main() {
    std::cout << "--- Iterator Practice Start ---\n";

    // 1. 인벤토리 테스트
    // TODO: '데이터의 원본들을 1차원으로 순차적, 연속적으로 보관하는 가변 길이를 가지는 컨테이너'를 생성하고
    //       1001, 1002, 1003 이라는 아이템 ID를 순서대로 삽입하세요.
    // [변수 생성 영역]
    std::vector<int> container;
    container.push_back(1001);
    container.push_back(1002);
    container.push_back(1003);

    std::cout << "[Inventory Items]\n";
    // TODO: PrintAllItems 함수를 호출하세요.
    PrintAllItems(container);

    // 2. 플레이어 스탯 테스트
    // TODO: '항상 정렬된 상태를 자동 유지하며 특정 Key로 Value를 찾는 컨테이너'를 생성하고
    //       "STR" -> 15, "DEX" -> 12, "INT" -> 8 형태의 초기 데이터를 보관하세요.
    // [변수 생성 영역]
    std::map<std::string, int> stats;
    stats["STR"] = 15;
    stats["DEX"] = 12;
    stats["INT"] = 8;

    // 스탯 업그레이드 전/후 검증
    int beforeStr = stats["STR"]; // TODO: 컨테이너에서 "STR"의 값을 찾아 대입하세요.

    // TODO: UpgradeAllStats 함수를 호출하세요.
    UpgradeAllStats(stats);

    int afterStr = stats["STR"]; // TODO: 컨테이너에서 "STR"의 값을 다시 찾아 대입하세요.

    // TDD 검증
    std::cout << "\n[TDD Test Result]\n";
    if (afterStr == beforeStr + 10) {
        std::cout << "SUCCESS: 스탯이 올바르게 10 증가했습니다! (STR: " << afterStr << ")\n";
    }
    else {
        std::cout << "FAIL: 로직이 틀렸습니다. (Expected: " << beforeStr + 10 << ", Actual: " << afterStr << ")\n";
        assert(false);
    }

    return 0;
}
