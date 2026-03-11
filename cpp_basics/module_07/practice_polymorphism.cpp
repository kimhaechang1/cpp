#include <iostream>
#include <vector>

// ----------------------------------------------------------------------------
// [실습 목표]
// 1. 업캐스팅(Upcasting)을 통해 부모 포인터로 자식을 조작하는 원리를 배웁니다.
// 2. 가상 함수(virtual)가 없을 때의 '정적 바인딩'과, 있을 때의 '동적 바인딩' 차이를 눈으로 확인합니다.
// 3. sizeof 연산자를 통해 컴파일러가 몰래 주입하는 vptr (8바이트)의 존재를 증명합니다.
// ----------------------------------------------------------------------------

// [STEP 1. 부모 클래스 설계 (virtual 미적용)]
// Monster 클래스를 선언합니다.
// - 멤버 변수: int hp (기본값 100) 선언.
// - 멤버 함수: void Attack() "몬스터의 일반 공격!" 출력. (아직 virtual을 붙이지 마세요!)

class Monster {
public:
    int hp{ 100 };

    // TODO: 일단은 그냥 만들고, 나중에 [STEP 4]에서 여기 앞에 virtual 을 붙여보세요.
    virtual void Attack() {
        std::cout << "[Monster] 몬스터의 일반 공격!\n";
    }
    // vptr 이 있을것
    // vptr 은 상속받은 곳에서도 공유하게 된다.
};


// [STEP 2. 자식 클래스 설계]
// Monster를 상속받는 Slime 클래스와 Orc 클래스를 만드세요.
// - 함수 재정의(Overriding): 각각 Attack() 함수를 다시 만들어서 
//   "[Slime] 끈적한 공격!", "[Orc] 거대한 도끼 공격!" 이라고 출력하게 하세요.

class Slime : virtual public Monster {
public:
    // TODO: 부모의 Attack()을 덮어씁니다. (Overriding)
    void Attack() override {
        std::cout << "[Slime] 끈적한 점액 공격!\n";
    }
    // 여기서는 부모의 vptr 과 함께 사용하며 곧바로 오버라이딩된 함수의 주소를 바꾼다.
};

class Orc : virtual public Monster {
public:
    void Attack() override {
        std::cout << "[Orc] 거대한 도끼 공격!\n";
    }
};


int main() {
    std::cout << "========== [ TEST 1: 메모리 크기 (vptr 유무) ] ==========\n";
    // [STEP 3. 메모리 확인]
    // 현재 아무런 virtual 함수가 없습니다. 아래 출력 결과가 어떻게 나올지 예상해 보고 실행하세요.
    // (만약 위 상단 Monster 에 virtual 을 붙인다면 이 크기가 어떻게 바뀔까요?)
    std::cout << "Monster 클래스의 크기: " << sizeof(Monster) << " byte\n";
    std::cout << "Slime 클래스의 크기: " << sizeof(Slime) << " byte\n";
    std::cout << "---------------------------------------------------------\n\n";


    std::cout << "========== [ TEST 2: 정적 바인딩 vs 동적 바인딩 ] ==========\n";
    // [STEP 4. 업캐스팅의 함정]
    // 1. 부모 포인터 배열에 슬라임 1마리와 오크 1마리를 동적 할당하여 넣습니다. (업캐스팅 발생!)
    Monster* monsters[2];
    monsters[0] = new Slime();
    monsters[1] = new Orc();

    // 2. 아래 반복문을 돌려 공격 명령을 내립니다.
    // 현재는 "몬스터의 일반 공격!" 만 2번 출력되는 [정적 바인딩] 상태일 것입니다.
    for (int i = 0; i < 2; ++i) {
        monsters[i]->Attack();
    }

    // [STEP 5. 문제 해결 (다형성 발동!)]
    // 1. 위 STEP 1의 Monster::Attack() 함수 원형 맨 앞에 `virtual` 키워드를 붙이세요.
    // 2. 위 STEP 2의 Slime과 Orc의 Attack() 함수 원형 맨 끝에 `override` 키워드를 붙이세요. (명시적 권장)
    // 3. 다시 실행해 보세요! 
    //    👉 sizeof 값이 8바이트 (vptr) 만큼 늘어났는지 확인!
    //    👉 출력 결과가 "슬라임 공격!", "오크 공격!" 으로 똑똑하게 바뀌었는지 (동적 바인딩) 확인!

    
    // ----- 메모리 정리 (소멸자 관련 내용은 나중에 배우게 되지만 메모리 누수 방지를 위해!) -----
    for (int i = 0; i < 2; ++i) {
        delete monsters[i];
    }

    return 0;
}
