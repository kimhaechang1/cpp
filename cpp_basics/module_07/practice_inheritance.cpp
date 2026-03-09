#include <iostream>
#include <cassert>

// [지시사항]
// 1. 'Monster'라는 이름의 부모 클래스를 만드세요.
//    - 목적: 몬스터들의 공통 속성과 기능을 모아두는 곳
//    - 필요한 변수: 체력(hp), 공격력(attack)
//    - 필요한 기능 1: 클래스 생성 시 체력과 공격력을 전달받아 초기화하는 목적의 생성자
//    - 필요한 기능 2: 몬스터가 공격을 받았을 때 데미지를 전달받아 체력을 깎는 목적의 함수 (TakeDamage)
class Monster {
public:
    int hp;
    int attack;

    Monster(int h, int atk) : hp(h), attack(atk) {

    }

    void TakeDamage(int atk) {
        hp = atk > hp ? 0 : hp - atk;
    }
};

// 2. 'Slime'이라는 이름의 자식 클래스를 만드세요.
//    - 목적: Monster를 상속받는 슬라임
//    - 필요한 추가 변수: 끈적임 지수(stickiness)
//    - 필요한 기능: 체력, 공격력, 끈적임 지수를 모두 전달받아 초기화하는 생성자 
//      (🚨 주의: 부모의 속성은 반드시 우리가 배운 문법을 통해 부모 생성자에게 넘겨주어 초기화해야 합니다!)
class Slime : public Monster{
public:
    int stickiness;
    Slime(int hp, int atk, int sticki) : Monster{ hp, atk }, stickiness(sticki) {

    }
};

// 3. 'Orc'라는 이름의 자식 클래스를 만드세요.
//    - 목적: Monster를 상속받는 오크
//    - 필요한 추가 변수: 분노 수치(rage)
//    - 필요한 기능: 체력, 공격력, 분노 수치를 모두 전달받아 초기화하는 생성자

// =============== [ 여 기 에  코 드 를  작 성 하 세 요 ] ===============
class Orc : public Monster {
public:
    int rage;
    Orc(int hp, int atk, int rage) : Monster{hp, atk}, rage(rage) {

    }
};



// ====================================================================

int main() {
    // -------------------------------------------------------------
    // [테스트 1] Slime 객체 생성 및 상속 확인
    // -------------------------------------------------------------
    // 설명: 50의 체력, 10의 공격력, 5의 끈적임을 가진 슬라임 생성
    Slime slime(50, 10, 5);
    
    // 상속받은 속성(hp, attack)과 자신의 속성(stickiness)이 잘 초기화되었는지 검증
    assert(slime.hp == 50);
    assert(slime.attack == 10);
    assert(slime.stickiness == 5);

    // 부모로부터 물려받은 행동(메서드)이 제대로 작동하는지 검증
    slime.TakeDamage(15);
    assert(slime.hp == 35);

    std::cout << "[SUCCESS] Slime 객체 생성 및 기능 상속 테스트 통과!\n";

    // -------------------------------------------------------------
    // [테스트 2] Orc 객체 생성 및 상속 확인
    // -------------------------------------------------------------
    // 설명: 100의 체력, 30의 공격력, 100의 분노를 가진 오크 생성
    Orc orc(100, 30, 100);

    assert(orc.hp == 100);
    assert(orc.attack == 30);
    assert(orc.rage == 100);

    orc.TakeDamage(20);
    assert(orc.hp == 80);

    std::cout << "[SUCCESS] Orc 객체 생성 및 기능 상속 테스트 통과!\n";

    // -------------------------------------------------------------
    // [테스트 3] 메모리 크기 (Size) 확인 - 눈으로 보는 상속 구조
    // -------------------------------------------------------------
    std::cout << "\n[메모리 크기 확인]\n";
    std::cout << "- 부모(Monster) 크기: " << sizeof(Monster) << " bytes\n";
    std::cout << "- 자식(Slime) 크기: " << sizeof(Slime) << " bytes\n";
    std::cout << "- 자식(Orc) 크기:   " << sizeof(Orc) << " bytes\n";

    // 만약 int형(4바이트) 변수를 사용하셨다면, Monster는 8 bytes, 자식들은 12 bytes가 나와야 정상입니다!

    std::cout << "\n🎉 모든 테스트를 통과했습니다! 상속(Inheritance)의 기본을 완벽히 마스터하셨습니다! 🎉\n";

    return 0;
}
