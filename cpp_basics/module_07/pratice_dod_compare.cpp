#include <iostream>
#include <vector>
#include <chrono>
#include <string>

// 1. AoS (Array of Structures) - 일반적인 OOP 방식
struct MonsterAoS {
    int id;
    float hp;
    char name[128]; // 캐시 라인을 오염시키기 위한 '무거운' 데이터
    float posX, posY, posZ;
};

// 2. SoA (Structure of Arrays) - 데이터 지향 방식
struct MonsterSoA {
    std::vector<int> ids;
    std::vector<float> hps; // HP만 따로 모음
    std::vector<float> posXs;
    std::vector<float> posYs;
};

const int MONSTER_COUNT = 1000000;

int main() {
    // [AoS 준비]
    std::vector<MonsterAoS> aos_monsters(MONSTER_COUNT);
    for (int i = 0; i < MONSTER_COUNT; ++i) aos_monsters[i].hp = 100.0f;

    // [SoA 준비]
    MonsterSoA soa_monsters;
    soa_monsters.hps.resize(MONSTER_COUNT, 100.0f);

    // --- AoS 성능 측정 ---
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < MONSTER_COUNT; ++i) {
        aos_monsters[i].hp += 1.0f; // 덩어리 안에서 HP만 골라서 수정
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> aos_duration = end - start;

    // --- SoA 성능 측정 ---
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < MONSTER_COUNT; ++i) {
        soa_monsters.hps[i] += 1.0f; // HP만 모인 배열을 순차적으로 수정
    }
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> soa_duration = end - start;

    std::cout << "AoS (OOP) Time: " << aos_duration.count() << " ms" << std::endl;
    std::cout << "SoA (DOD) Time: " << soa_duration.count() << " ms" << std::endl;
    std::cout << "성능 차이: " << aos_duration.count() / soa_duration.count() << " 배" << std::endl;

    return 0;
}
