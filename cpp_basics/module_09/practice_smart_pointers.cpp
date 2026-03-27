#include <iostream>
#include <memory>
#include <string>
#include <cassert>

// ==============================================================================
// [개념 복습] 스마트 포인터 실습 — practice_smart_pointers.cpp
// 이 파일의 모든 연습은 new / delete 를 직접 쓰지 않고 구현해야 합니다!
// ==============================================================================


// ==============================================================================
// 연습문제 1: unique_ptr 기본 생성 및 접근
// [요구사항]
// 1. 함수 이름: SpawnBoss
// 2. 기능: "Dragon"이라는 이름과 체력 9999를 가진 Monster를 힙에 생성하여
//          이 동적 객체에 대한 "독점 소유권"을 가진 스마트 래퍼를 반환하세요.
// 3. [Blank Slate]: 반환 타입을 직접 추론하세요.
//    힌트: 스택 수명 스코프를 벗어나는 즉시 delete를 스스로 뱉어내는
//          똑똑한 "독점 메모리 래퍼"를 사용하세요.
// ==============================================================================

struct Monster {
    std::string name;
    int hp;
    Monster(std::string n, int h) : name{std::move(n)}, hp{h} {}
    ~Monster() { std::cout << "[소멸] " << name << " 처치 완료!\n"; }
};

// 코드 작성
std::unique_ptr<Monster> SpawnBoss() {
    return std::make_unique<Monster>("Dragon", 9999);
}

// ==============================================================================
// 연습문제 2: shared_ptr 참조 카운팅
// [요구사항]
// 1. 함수 이름: ShareMonster
// 2. 파라미터: Monster를 가리키는 "공유 소유권" 래퍼를 받습니다.
// 3. 기능: 받은 공유 래퍼를 내부에서 지역 변수로 하나 더 복사(공유)한 뒤,
//          복사 직후의 참조 카운트(use_count)를 반환하세요.
// 4. [Blank Slate]: 반환 타입과 파라미터 타입을 직접 추론하세요.
// ==============================================================================

// 코드 작성
int ShareMonster(std::shared_ptr<Monster>& sharedMonster){
    std::shared_ptr<Monster> copiedMonster = sharedMonster;
    return copiedMonster.use_count();
}

// ==============================================================================
// 연습문제 3: unique_ptr 소유권 이전 (Move)
// [요구사항]
// 1. main() 안에서 직접 구현하세요. (함수 분리 불필요)
// 2. unique_ptr<Monster> ownerA 에 Slime(hp:100)을 생성합니다.
// 3. ownerA의 소유권을 ownerB로 이전합니다.
// 4. 이전 후 ownerA가 nullptr인지 assert로 검증합니다.
// 5. ownerB를 통해 Slime에 정상 접근되는지 assert로 검증합니다.
// ==============================================================================


// ---------------- C++ 테스트 코드 (Main) ----------------
// 여기 아래의 테스트(Main) 코드는 절대 수정하지 마세요!
// --------------------------------------------------------
int main() {
    std::cout << "========= 스마트 포인터 훈련 시작 =========\n";

    // [테스트 1: SpawnBoss]
    {
        auto boss = SpawnBoss();
        assert(boss != nullptr && "SpawnBoss 실패: nullptr 반환");
        assert(boss->name == "Dragon" && "SpawnBoss 실패: 이름 불일치");
        assert(boss->hp == 9999 && "SpawnBoss 실패: HP 불일치");
        std::cout << "[PASS] 테스트 1: unique_ptr 생성 완료\n";
    }  // ← 이 스코프 끝에서 Dragon 소멸자가 자동 호출되어야 합니다!

    std::cout << "\n";

    // [테스트 2: ShareMonster]
    {
        auto sharedMonster = std::make_shared<Monster>("Slime", 100);
        assert(sharedMonster.use_count() == 1 && "초기 카운트는 1이어야 합니다.");

        long countAfterShare = ShareMonster(sharedMonster);
        
        assert(countAfterShare == 2 && "ShareMonster 실패: 공유 중 카운트는 2여야 합니다.");

        // ShareMonster 함수가 끝나고 나면 카운트가 다시 1로 돌아와야 함
        assert(sharedMonster.use_count() == 1 && "함수 종료 후 카운트가 1로 복귀해야 합니다.");
        std::cout << "[PASS] 테스트 2: shared_ptr 참조 카운팅 완료\n";
    }

    std::cout << "\n";

    // [테스트 3: 소유권 이전]
    {
        // 코드 작성 (ownerA → ownerB 이전 및 assert 검증)
        std::unique_ptr<Monster> ownerA = std::make_unique<Monster>("Slime", 100);
        std::unique_ptr<Monster> ownerB = std::move(ownerA);
        assert(ownerA == nullptr);
        assert(ownerB->name == "Slime");
        std::cout << "[PASS] 테스트 3: unique_ptr 소유권 이전 완료\n";
    }

    std::cout << "\n========= 모든 스마트 포인터 훈련 완료! =========\n";
    return 0;
}
