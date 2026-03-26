#include <iostream>
#include <vector>
#include <string>
#include <ranges>
#include <algorithm>
#include <format>
#include <cassert>

// [Q] 데이터의 원본들을 1차원으로 순차적, 연속적으로 보관하는 가변 길이를 가져야 합니다.
// [Q] 서로 연관된 데이터(이름, 공격력, 상태 등)를 하나의 이름 아래 묶어 새로운 타입을 만드세요.
struct Monster {
    std::string name;
    int power;
    bool isBoss;
};

// ===========================================================================
// [TASK] Java Stream과 유사한 파이프라인(|)을 사용하여 몬스터 리스트를 가공하세요.
// 1. 공격력이 50 이상인 몬스터만 필터링 (std::views::filter)
// 2. 필터링된 몬스터의 이름을 "[BOSS] 이름" 형태로 변환 (std::views::transform)
// ===========================================================================

int main() {
    std::vector<Monster> fieldMonsters = {
        {"Slime", 10, false},
        {"Orc", 55, false},
        {"Dragon", 999, true},
        {"Goblin", 30, false},
        {"Skeleton", 60, false}
    };

    auto bossQuestTargets = fieldMonsters
                    | std::views::filter([&](Monster m) {return m.power >= 50; })
                    | std::views::transform([&](Monster m) { return std::format("[BOSS] {}", m.name); });

    // [Q] 기존 데이터 원본을 건드리지 않고, 특정 기호를 사용해 데이터의 흐름을 연결하는 방식을 쓰세요.
    // TODO: 파이프라인(ranges::view) 구현
    // auto bossQuestTargets = fieldMonsters | ...

    // [Q] 컬렉션의 크기에 구애받지 않고 처음부터 끝까지 모든 원소를 안전하게 훑는 순회 방식을 쓰세요.
    std::cout << "[Boss Quest Targets List]" << std::endl;
    int count = 0;

    /* TODO:
    for (auto const& name : bossQuestTargets) {
        std::cout << name << std::endl;
        count++;
    }
    */
    for (auto const& name : bossQuestTargets) {
        std::cout << name << std::endl;
        count++;
    }

    // TDD 검증용
     assert(count == 3); // Orc, Dragon, Skeleton 이 필터링되어야 함

    return 0;
}
