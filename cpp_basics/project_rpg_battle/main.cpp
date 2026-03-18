#include <iostream>
#include <vector>
#include <string>
#include <array>
#include <span>
#include <cassert>
#include <crtdbg.h> // M5: 메모리 누수 탐지

/**
 * [FINAL TIER EXAM: OOP RPG Battle System]
 * 
 * 범위: Module 01 ~ Module 07 전체
 * 목표: 아래 비어있는 선언부와 구현부를 채워 main()의 Assert 테스트를 모두 통과하세요.
 * 주의: 주석에 적힌 '목적'과 '이름' 외의 구체적 타입(ex: int, 포인터 등)은 직접 유추해야 합니다.
 *       M1(enum/struct)부터 M7(다형성/캐스팅)까지의 모든 지식이 요구됩니다.
 */

// =========================================================================
// [M1] 기본 데이터 규격 설계
// =========================================================================

// [1] 원소를 나타내는 열거형 (enum class)
// 이름: Element (값: Fire, Water, Earth)


// [2] 2D 좌표를 나타내는 구조체
// 이름: Position (멤버: float x, float y)


// =========================================================================
// [M7] 인터페이스 설계 (다중 상속용)
// =========================================================================

class BaseUnit; // 전방 선언

// [3] 공격 인터페이스
// 이름: IAttacker
// 기능: 순수 가상 함수 Attack(BaseUnit* target)


// [4] 치유 인터페이스
// 이름: IHealer
// 기능: 순수 가상 함수 Heal(BaseUnit* target)



// =========================================================================
// [M6, M7] 최상위 뼈대 설계
// =========================================================================

// [5] 모든 유닛의 최상위 추상 클래스
// 이름: BaseUnit
// - protected 멤버: 문자열 이름(name), 체력(hp), 최대체력(maxHp), 위치(pos), 원소(element)
// - 생성자: 이름, 체력, 원소, 위치를 받아 초기화. maxHp는 hp와 동일하게 설정.
// - 소멸자: "Unit [이름] Destroyed" 출력 (가상 소멸자 필수, 메모리 누수 방지)
// - 가상 함수(TakeDamage): 데미지(정수)를 받아 hp 감소 (0 미만으로 떨어지지 않음)
// - 순수 가상 함수(Action): 매개변수 없음
// - Getter: GetName(), IsAlive() -> 체력이 0보다 큰지 반환 (const 함수로 작성)



// =========================================================================
// [M7] 구체 클래스 구현
// =========================================================================

// [6] 전사 클래스 (BaseUnit, IAttacker 상속)
// 이름: Warrior
// - 생성자: 부모 생성자 호출하여 초기화
// - Action(): "[이름] readies sword." 출력
// - Attack(target): target의 TakeDamage(20) 호출 및 "[이름] attacks [대상 이름]!" 출력


// [7] 사제 클래스 (BaseUnit, IHealer 상속)
// 이름: Cleric
// - 생성자: 부모 생성자 호출하여 초기화
// - Action(): "[이름] prays." 출력
// - Heal(target): 이 게임에서는 임시로 무조건 target의 TakeDamage(-20)을 호출하여 회복시킴 
//                 (단, BaseUnit의 TakeDamage를 오버라이딩하거나 수정해서, hp가 maxHp를 넘지 않게 처리할 것)
//                 출력: "[이름] heals [대상 이름]!"



// =========================================================================
// [M3, M4] 함수와 포인터, 메모리 뷰(span) 활용
// =========================================================================

// [8] 전역 생존자 카운트 함수 (std::span 활용)
// 인자로 BaseUnit 포인터들의 span을 받아, IsAlive()가 true인 유닛의 수를 반환.
int CountAliveUnits(std::span<BaseUnit*> units) {
    // TODO: 구현
    return 0;
}


// =========================================================================
// [M5, M6] 전장 매니저 클래스 (동적 할당 및 RAII)
// =========================================================================

// [9] 전장 매니저
// 이름: BattleField
// - 멤버: std::vector<BaseUnit*> units
// - 소멸자: 남은 모든 동적 할당 유닛들을 delete하여 메모리 누수 방지
// - 함수(AddUnit): BaseUnit 포인터를 vector에 추가
// - 함수(GetUnits): 내부 vector의 span을 반환 (span<BaseUnit*> 리턴)
// - 함수(SimulateCombat): 
//   1. 모든 유닛의 Action() 실행
//   2. IAttacker인 유닛(dynamic_cast 활용)은 살아있는 적 중 가장 첫 번째 유닛을 공격
//      (자신은 공격하지 않음)



// =========================================================================
// 메인 테스트 슈트 (수정 금지)
// =========================================================================
int main() {
    // M5: 메모리 누수 탐지 활성화
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    std::cout << "--- TIER EXAM: OOP RPG BATTLE SYSTEM ---\n" << std::endl;

    {
        BattleField field;

        // M1, M5: 객체 동적 할당 및 열거형/구조체 활용
        Warrior* w1 = new Warrior("Arthur", 100, Element::Fire, Position{0.0f, 0.0f});
        Cleric* c1 = new Cleric("Merlin", 60, Element::Water, Position{5.0f, 5.0f});
        Warrior* orc = new Warrior("Orc", 80, Element::Earth, Position{10.0f, 10.0f});

        field.AddUnit(w1);
        field.AddUnit(c1);
        field.AddUnit(orc);

        // M4: span 테스트
        std::span<BaseUnit*> allUnits = field.GetUnits();
        assert(CountAliveUnits(allUnits) == 3 && "초기 생존자는 3명이어야 합니다.");
        std::cout << "[PASS] Initialization & Span Count\n";

        // M7: 다형성 및 Action 테스트
        std::cout << "\n[Turn 1: Simulation]" << std::endl;
        field.SimulateCombat(); 
        
        // 검증: Arthur가 Orc를 때렸으므로 Orc의 체력은 60, 
        // Orc가 Arthur를 때렸으므로 Arthur 체력은 80이어야 함 (단순 구현이라 가정)
        // (구현에 따라 타겟이 달라질 수 있지만 작동만 하면 OK)

        // M7: 인터페이스 캐스팅 (dynamic_cast) 테스트
        std::cout << "\n[Special: Manual Heal]" << std::endl;
        IHealer* healerPtr = dynamic_cast<IHealer*>(c1);
        assert(healerPtr != nullptr && "Cleric은 IHealer로 캐스팅되어야 합니다.");
        healerPtr->Heal(w1); // Arthur 회복

        IAttacker* fakeAttacker = dynamic_cast<IAttacker*>(c1);
        assert(fakeAttacker == nullptr && "Cleric은 IAttacker가 아니어야 합니다.");
        std::cout << "[PASS] Dynamic Casting (RTTI)\n";

        // M5/M6: RAII 소멸 테스트
        std::cout << "\n[Field Destruction]" << std::endl;
    } // <- 여기서 BattleField 소멸자가 불리며 Arthur, Merlin, Orc 순서관계없이 delete 되어야 함.

    std::cout << "\n--- All Tests Finished (Check output for memory leaks) ---\n";
    return 0;
}
