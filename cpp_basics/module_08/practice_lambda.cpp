#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include<algorithm>

// [주의] std::sort, std::count_if 등 강력한 병기들을 쓰기 위해 필요한 헤더를 직접 추가하세요.

// =========================================================================
// [실무 프로젝트 상황: 인벤토리 아이템 정렬 및 필터링]
// 
// 수천 개의 데이터가 쌓인 인벤토리를 처리할 때, 매번 "for문 + if문"으로 직접
// 로직을 짜는 것은 버그를 유발하고 코드도 지저분해집니다.
// C++은 <algorithm> 헤더 안에 광속으로 검증된 순회 도구들을 마련해 두었습니다.
// 여러분은 이 강력한 알고리즘 차량에, "어떻게 움직여라"라는 '뇌(로직)'만
// 람다 함수로 쏙 빼서 던져주시면 됩니다!
// =========================================================================

struct Item {
    int id;
    int grade;   // 1: 일반, 2: 희귀, 3: 서사, 4: 전설
    int cost;    // 상점 판매가
};

int main() {
    std::cout << "--- Lambda & Algorithm Practice ---\n";

    // 데이터의 원본들을 1차원으로 순차적, 연속적으로 보관하는 가변 길이 컨테이너
    std::vector<Item> inventory = {
        {101, 1, 50},
        {102, 3, 300},
        {103, 2, 120},
        {104, 4, 1000},
        {105, 3, 250},
        {106, 1, 10}
    };


    // -------------------------------------------------------------------------
    // [요구사항 1: 필터링 (Capture 활용)]
    // 외부 데이터(기획팀이 실시간으로 넘겨주는 컷-오프 변수)를 로직이 사용할 수 있어야 합니다.
    // -------------------------------------------------------------------------
    int targetGradeThreshold = 3; // "3등급 이상인 템만 찾아줘!"
    int highGradeCount = 0;       // 조건을 만족하는 아이템 개수 누적

    // TODO: <algorithm>의 개수를 세는 함수(std::count_if)와 람다 함수를 결합하여, 
    //       grade가 targetGradeThreshold 이상인 무기 개수를 찾아 highGradeCount에 저장하세요.
    // [조건] 함수를 외부에 억지로 빼지 말고, 그 즉시 현장에서 이름 없는 1회용 로직 덩어리를 만드세요.
    // [조건] 람다의 캡처 기능을 통해 외부 지역변수인 'targetGradeThreshold'를 내부로 빨아들이세요.
    // [작성 영역]
    highGradeCount = std::count_if(inventory.begin(), inventory.end(), [&](Item item) {
        return item.grade >= targetGradeThreshold;
    });


    // -------------------------------------------------------------------------
    // [요구사항 2: 커스텀 정렬 (다중 조건)]
    // -------------------------------------------------------------------------
    // TODO: <algorithm>의 고속 줄 세우기 함수(std::sort)와 람다 함수를 결합하여 inventory를 정렬하세요.
    // [조건] 함수를 외부에 억지로 빼지 말고 람다를 사용하세요. 
    // [조건 1순위] Item의 grade(등급)가 높은 순서대로(내림차순) 정렬합니다.
    // [조건 2순위] 만약 grade(등급)가 서로 똑같다면, cost(판매가)가 낮은 순서대로(오름차순) 정렬합니다.
    // [작성 영역]
    std::sort(inventory.begin(), inventory.end(), [&](Item a, Item b) {
        if (a.grade == b.grade) {
            return a.cost < b.cost;
        }
        // sort 할때 왼쪽이 먼저오는걸 결정하는것
        // 즉 두개를 비교했을때 왼쪽이 더 큰것이라면 왼쪽으로 >
        // 오른쪽이 더 큰것이라면 왼쪽이 <
        return a.grade > b.grade;
    });


    // ================== TDD 검증 구역 ==================
    std::cout << "\n[TDD Test Result]\n";

    // 1. 카운트 테스트
    if (highGradeCount == 3) {
        std::cout << "SUCCESS: 3등급(서사) 이상 아이템 개수 (3개) 도출 완료!\n";
    }
    else {
        std::cout << "FAIL: 필터링 로직이 틀렸습니다. (Expected: 3, Actual: " << highGradeCount << ")\n";
        assert(false);
    }

    // 2. 정렬 테스트
    if (inventory[0].id == 104 && inventory[1].id == 105 && inventory.back().id == 101) {
        std::cout << "SUCCESS: 등급 내림차순 및 가격 오름차순 최우선 정렬 완료!\n";
    }
    else {
        std::cout << "FAIL: 커스텀 정렬 로직이 틀렸습니다.\n";
        std::cout << "[최상위 템] 현재: id " << inventory[0].id << ", grade " << inventory[0].grade << ", cost " << inventory[0].cost << "\n";
        std::cout << "[최하위 템] 현재: id " << inventory.back().id << ", grade " << inventory.back().grade << ", cost " << inventory.back().cost << "\n";
        assert(false);
    }

    return 0;
}
