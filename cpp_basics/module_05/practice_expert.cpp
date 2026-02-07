#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>
#include <iostream>
#include <string>

#ifdef _DEBUG
#define new new( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#endif

// [실습 설명]
// Module 05 보충: 메모리 안전성 (Safe Delete)
// 이중 포인터를 활용하여 메모리 해제와 nullptr 처리를 한번에 하는 함수를 만듭니다.

struct Monster {
    std::string name;
    int hp;
};

// -------------------------------------------------------------
// [Safe Delete]
// -------------------------------------------------------------
// 포인터를 받아서 delete 하고, 그 포인터를 nullptr로 만드는 것까지 책임지는 함수
void SafeDelete(Monster** ppMob) {
    // TODO 1: ppMob 자체가 유효한지 체크
    if (ppMob == nullptr) return;

    // TODO 2: 실제 포인터(*ppMob)가 유효한지(이미 지워진 건 아닌지) 체크
    if (*ppMob == nullptr) return;

    // TODO 3: 메모리 해제
    delete* ppMob;

    // TODO 4: 포인터에 nullptr 대입 (안전장치)
    // *ppMob = ...
    *ppMob = nullptr;
}

int main() {
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    std::cout << "=== Module 05 Expert Practice (Safety) ===\n";

    // 몬스터 생성 (Heap)
    Monster* myMob = new Monster{ "Dragon", 9999 };

    std::cout << "Monster Created at: " << myMob << "\n";

    // SafeDelete 호출
    // 우리가 만든 함수가 '진짜 포인터 변수'를 수정해야 하므로 주소를 넘깁니다.
    SafeDelete(&myMob);

    // 결과 확인
    if (myMob == nullptr) {
        std::cout << ">> Success: myMob is nullptr now.\n";
    }
    else {
        std::cout << ">> Warning: myMob is Dangling Pointer! Address: " << myMob << "\n";
    }

    // 중복 해제 시도 (SafeDelete가 안전하다면 크래시가 안 나야 함)
    SafeDelete(&myMob);
    std::cout << ">> Double Free Test Passed (No Crash).\n";

    return 0;
}
