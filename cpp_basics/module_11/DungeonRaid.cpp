#include "DungeonRaid.h"
#include <chrono>
// [Review] <format>, <iostream>은 DungeonRaid.h에서 이미 include 하거나 미사용이므로 제거
#include <thread>

// ---------------------------------------------------------
// [DungeonRaid.cpp]
//
// 1. DungeonRaid 클래스의 정적(static) 멤버 변수를 여기서 정의하고 0으로
// 초기화하세요.
// 2. 클래스의 각 메서드와 ExecuteRaid 함수를 구현하세요.
// 3. ExecuteRaid 내부에서는 std::this_thread::sleep_for(50ms) 정도를 사용하여
//    전투 시뮬레이션을 구현하고, 루프 마다 stop_token을 체크하세요.
// ---------------------------------------------------------
using namespace std::chrono_literals;
// (static 변수 정의)
int DungeonRaid::totalGlobalKills = 0;
// (구현부 - 학습자가 작성)
void ExecuteRaid(std::stop_token stoken,
                 std::span<const std::unique_ptr<Monster>> mPtrs,
                 DungeonRaid &raid)
{
    if (stoken.stop_requested()) {
        return;
    }
    for (const auto &ptr : mPtrs) {
        if (stoken.stop_requested()) {
            return;
        }
        raid.RecordKill();
        std::this_thread::sleep_for(50ms);
    }
}