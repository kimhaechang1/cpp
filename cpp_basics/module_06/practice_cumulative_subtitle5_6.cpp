#include <iostream>
#include <cassert>
#include <span>
#include <memory>
#include <vector>
// [여기에 나머지 필요한 헤더 파일을 포함하세요]


// ====== [요구사항 1: 전투 매직 시스템의 기반 속성 정의] ======
// 1. 이름이 MagicType 인 열거체를 선언하세요. 원소는 Fire, Ice, Lightning, None 네 가지가 있습니다.
// 2. 입력된 매직 속성의 기본 위력을 반환하는 GetBasePower 함수를 작성하되,
//    '실행 시점'이 아니라 '컴파일 시점'에 계산이 완료되도록 강제하십시오.
//    (위력: Fire = 10, Ice = 5, Lightning = 15, 이외 = 0)
enum MagicType {
    Fire, Ice, Lightning, None
};
constexpr int GetBasePower(MagicType type) {
    if (type == MagicType::Fire) {
        return 10;
    }
    else if (type == MagicType::Ice) {
        return 5;
    }
    else if (type == MagicType::Lightning) {
        return 15;
    }
    else {
        return 0;
    }
}
// ====== [요구사항 2: 발사체 객체 설계] ======
// 1. 이름이 Projectile 인 객체를 생성합니다.
// 2. 발사체는 식별자 번호(정수형 ID), 부여된 속성(MagicType), 마력 수치(정수) 세 가지 정보를 소유합니다.
// 3. 보안과 메모리 혼선을 막기 위해 이 발사체는 타 객체로 '대입' 또는 '복사생성' 되는 것이 물리적으로 불가능해야 합니다.
// 4. 발사체가 메모리에서 해제될 때 터미널 창에 "Projectile [해당ID] 소멸" 이라고 메시지를 출력하세요.
// 5. 발사체가 신규 생성될 때마다 발급되는 식별자 번호(ID)는 1부터 시작하여 생성 순서에 따라 즉시 자동으로 1씩 증가해야 합니다.
class Projectile {
private:
    static inline int autoID = 1;

public:
    // C++11 모던 방식: 컴파일러에게 "이 함수들을 생성하지 마!" 라고 명시적으로 지시
    Projectile(const Projectile& other) = delete;
    Projectile& operator=(const Projectile& other) = delete;
    int ID;
    MagicType type;
    int magicPower;

    Projectile(MagicType type, int mp) : type(type), magicPower(mp){
        ID = autoID;
        autoID++;
    }
    ~Projectile() {
        std::cout << "Projectile [" << this->ID << "] 소멸" << std::endl;
    }
};

// ====== [요구사항 3: 전투 매니저 설계] ======
// 1. 이름이 CombatManager 인 객체를 생성합니다. 이 매니저는 다수의 Projectile 객체를 내부에서 리스트 형태로 소유/관리합니다.
// 2. 전투 매니저 내부에 저장되는 발사체들은 반드시 '동적 할당 변수'로 생성되어야 합니다.
// 3. 단, CombatManager 객체가 소멸될 때 내부에서 보관 중이던 발사체들도 어떠한 메모리 누수도 없이 '자동으로 완전 파괴'되도록 설계하십시오. (수동 delete 금지 기준 만족)
// 4. CombatManager 객체끼리 덧셈(+) 기호로 연산할 경우, 두 매니저가 소유하고 있는 총 발사체 개수의 총합이 int형으로 반환되어야 합니다.
// 5. CreateProjectiles 라는 함수를 구성하여, 여러 개의 마력 수치들이 기록된 원본 외부 배열에서 '특정 범위 구간'만을 일괄적으로 읽어와 한 번에 여러 발사체를 등록할 수 있게 하십시오. 
//   - 이때, 읽어올 구간의 시작 포인터와 길이(크기)를 따로따로 쪼개서 넘기는 전통적인 방식은 허용 불가하며, 단일 개념으로 배열의 시야(View) 범위를 표현하는 모던 타입을 도입하십시오. (부여할 속성인 MagicType 도 함께 전달받아 발사체를 찍어냅니다.)
class CombatManager {
private:
    std::vector<std::unique_ptr<Projectile>> projectiles;

public:
    int operator+(CombatManager& other) {
        return this->projectiles.size() + other.projectiles.size();
    }

    void CreateProjectiles(std::span<int> mps, const MagicType type) {
        for (int mp : mps) {
            // C++14 모던 방식: new를 직접 쓰지 않고 더욱 안전한 make_unique 활용
            this->projectiles.push_back(std::make_unique<Projectile>(type, mp));
        }
    }
    int CalculateTotalDamage(const MagicType& type) {
        int sum = 0;
        for (int i = 0; i < projectiles.size(); i++) {
            if (type == projectiles[i]->type) {
                sum += projectiles[i]->magicPower * GetBasePower(projectiles[i]->type);
            }
        }
        return sum;
    }
};


// ====== [요구사항 4: 데미지 연산 로직] ======
// 1. CombatManager 객체 내부에 CalculateTotalDamage 라는 멤버 함수를 작성하세요.
// 2. 자신이 보관 중인 모든 발사체 중, 외부 매개변수로 넘어온 '특정 목표 속성(MagicType)'과 일치하는 발사체들만을 찾아내어 이들의 위력 총합을 구해 반환합니다.
//    (발사체 하나의 데미지 공식 = 해당 속성의 기본 위력 상수 × 보유한 마력 수치)
// 3. 목표 속성을 매개변수로 받아올 때는 '해당 열거체의 크기가 훗날 매우 거대해질 수 있음'을 감안하여, 원본 객체를 복사하지 않고도 안전하게 열람할 수 있는 효율적인 방식(포인터 제외)으로 인자를 선언하세요.


int main() {
    std::cout << "--- Cumulative Practice: M6 Final Start ---\n";

    // 1. 성능 제약 조건(컴파일 타임 평가) 검증
    static_assert(GetBasePower(MagicType::Fire) == 10, "Compile-time calculation failed!");
    static_assert(GetBasePower(MagicType::Ice) == 5, "Compile-time calculation failed!");
    static_assert(GetBasePower(MagicType::Lightning) == 15, "Compile-time calculation failed!");

    CombatManager manager1;
    CombatManager manager2;

    int powerInput[] = { 100, 200, 300, 400, 500 };

    // 2. 뷰 데이터 기반의 대량 생성 파이프라인 작동 검증
    // powerInput의 앞 3개(100,200,300) -> Fire 로 생성
    manager1.CreateProjectiles(std::span<int>{powerInput, 3}, MagicType::Fire);

    // powerInput의 그다음 2개(400,500) -> Ice 로 생성
    manager2.CreateProjectiles(std::span<int>{powerInput + 3, 2}, MagicType::Ice);

    // 3. 누적 순회 연산 로직 & 참조 전달 검증
    // (10*100) + (10*200) + (10*300) = 6000
    int fireTotal = manager1.CalculateTotalDamage(MagicType::Fire);
    assert(fireTotal == 6000);

    // (5*400) + (5*500) = 4500
    int iceTotal = manager2.CalculateTotalDamage(MagicType::Ice);
    assert(iceTotal == 4500);

    // 속성이 맞지 않으면 합산에서 제외됨을 검증
    int noIce = manager1.CalculateTotalDamage(MagicType::Ice);
    assert(noIce == 0);

    // 4. 오버로딩 메커니즘 검증
    // manager1(3개) + manager2(2개) = 총 5개
    int totalProjectiles = manager1 + manager2;
    assert(totalProjectiles == 5);

    // 5. 보안 및 해제 구조 검증
    // 이 아래 주석 처리된 코드가 막혀(Compile Error) 있어야 올바르게 구현한 것입니다!
    // (의도적으로 복사를 시도해 Rule of 0/5 위반 여부 체크)
    // Projectile copyTest; 
    // Projectile copyTest2 = copyTest; 

    std::cout << "\n[All Logical Tests Passed!]\n";
    std::cout << "Checking Lifetime and Auto-Memory Leaks Destruction...\n";

    // 프로그램 블록이 종료됨에 따라,
    // 터미널에는 "Projectile 1 소멸" 부터 "Projectile 5 소멸" 까지
    // 차례대로 메시지가 출력된다면 최종적으로 완벽하게 통과한 것입니다!
    return 0;
}
