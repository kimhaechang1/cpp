#include <iostream>
#include <cassert>

// ----------------------------------------------------------------------------
// [실습 목표]
// 1. 다이아몬드 상속 구조에서 발생하는 이름 모호성(Ambiguity) 에러를 직접 체험합니다.
// 2. 가상 상속(virtual inheritance)을 통해 문제를 해결해 봅니다.
// 3. sizeof 연산자를 통해 메모리의 중복 상태와 vbptr 의 존재를 확인합니다.
// ----------------------------------------------------------------------------

// [STEP 1. 최상위 부모 클래스]
// Character 클래스를 정의하세요.
// int형의 'hp' 멤버 변수를 가집니다.
class Character {
public:
    int hp;
};

// [STEP 2. 다이아몬드 구조의 중간 부모들]
// Warrior 클래스를 선언하고 Character를 '다중 상속' 받게 하세요. (가상 상속은 지금 넣지 마세요!)
// - int형의 'rage' 멤버 변수를 가집니다.
class Warrior : virtual public Character{
public:
    int rage;
};

// Mage 클래스를 선언하고 Character를 '다중 상속' 받게 하세요. (역시 가상 상속 금지!)
// - int형의 'mana' 멤버 변수를 가집니다.
class Mage : virtual public Character {
public:
    int mana;
};

// [STEP 3. 최하위 자식 클래스 (다이아몬드의 완성)]
// MagicSwordsman 클래스를 선언하고 Warrior와 Mage를 '다중 상속' 받게 하세요.
// 클래스 내부는 비워두셔도 됩니다.
class MagicSwordsman: public Warrior, public Mage {

};


int main() {
    // [STEP 4. 에러 체험하기 (주석 해제 후 빌드 시도)]
    //MagicSwordsman player;
    //player.hp = 100; // 🚨 컴파일하면 어떤 에러가 나나요? 확인 후 이 줄은 다시 주석 처리하세요.
    // ambiguas 에러 발생하네


    // [STEP 5. 메모리 구조 눈으로 확인 (하드웨어 검증)]
    // 아래 코드를 통해 각각의 크기가 몇 바이트로 나오는지 콘솔에서 직접 확인하세요.

    // std::cout << "--- [Virtual 미적용 상태] ---\n";
     //std::cout << "Character 크기: " << sizeof(Character) << " byte\n";
     //std::cout << "Warrior 크기: " << sizeof(Warrior) << " byte\n";
     //std::cout << "Mage 크기: " << sizeof(Mage) << " byte\n";
     //std::cout << "MagicSwordsman 크기: " << sizeof(MagicSwordsman) << " byte\n\n";

    // [STEP 6. 가상 상속(virtual) 도입하여 문제 해결하기]
    // 1. 위 STEP 2로 돌아가서, Warrior와 Mage가 Character를 상속받을 때 `virtual` 키워드를 추가합니다.
    // 2. 그리고 아래의 검증 코드를 주석 해제하여 에러 없이 player의 hp에 접근할 수 있는지, 
    //    메모리 크기(sizeof)가 어떻게 달라졌는지(vbptr의 흔적) 직접 눈으로 확인하세요.

    
    MagicSwordsman myPlayer;
    myPlayer.hp = 200; // 가상 상속으로 하나의 hp가 보장되어 에러가 사라져야 합니다.
    int a = 150;
    int* p = &a;
    std::cout << "ptr size: " << sizeof(p) << std::endl;
    std::cout << "--- [Virtual 적용 (가상 상속) 상태] ---\n";
    std::cout << "MyPlayer의 HP: " << myPlayer.hp << '\n';
    std::cout << "Character 크기: " << sizeof(Character) << " byte\n";
    std::cout << "Warrior 크기(virtual 적용): " << sizeof(Warrior) << " byte\n";
    std::cout << "Mage 크기(virtual 적용): " << sizeof(Mage) << " byte\n";
    std::cout << "MagicSwordsman 크기: " << sizeof(MagicSwordsman) << " byte\n";
    

    return 0;
}
