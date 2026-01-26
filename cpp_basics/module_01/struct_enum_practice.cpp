#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <malloc.h>

using namespace std;

// [Gemini Agent]
// C++ 기초: Struct(구조체)와 Enum(열거형) 통합 연습
// 이전에 배운 vector와 string을 결합하여 '파티 관리 시스템'을 만들어봅시다.

// ---------------------------------------------------------

// TODO 1: 'ElementType' 이라는 이름의 enum(열거형)을 정의하세요.
// 속성: FIRE, WATER, EARTH, WIND
// 힌트: enum ElementType { ... };

enum ElementType {
    FIRE, WATER, EARTH, WIND
};


// TODO 2: 'Character' 라는 이름의 struct(구조체)를 정의하세요.
// 멤버 변수:
// - string name; (이름)
// - int hp;      (체력)
// - ElementType element; (속성)

struct Character {
    string name;
    int hp;
    ElementType element;
};

struct BadStruct {
    char a;
    double b;
    char c;
};

struct GoodStruct {
    double a;
    char b;
    char c;
};

// ---------------------------------------------------------

int main() {
    cout << "=== Struct & Enum & Vector Practice ===" << endl;

    // TODO 3: Character 구조체 변수 'hero'를 선언하고 초기화하세요.
    // 이름: "Hero", HP: 100, 속성: FIRE
    Character* hero = new Character(
        "Hero", 100, ElementType::FIRE
    );

    //BadStruct* bad = (BadStruct*)malloc(sizeof(BadStruct));
    // 주의: null 참조
    /*cout << "bad->a: " << bad->a << endl;*/

    // malloc 스타일
    //Character* hero1 = (Character*)malloc(sizeof(Character));
    // 주의: null 참조
    //cout << hero1->name << endl;

    // memset 스타일
    //Character* hero2 = (Character*)memset(hero2, 0, sizeof(hero2));
    // 주의: 쓰레기값(이상한주소) 이미 들어가 있기 때문에, 이상한 주소로가서 0으로 밀어버릴려다가 오류가 남
    //cout << hero1->name << endl;


    // TODO 4: 'hero'의 속성에 따라 공격 멘트를 출력하는 switch문을 작성하세요.
    // FIRE -> "화염 공격!", WATER -> "물대포!" 등
    switch (hero->element) {
        case ElementType::FIRE:
            cout << "화염 공격!" << endl;
            break;
        case ElementType::WATER:
            cout << "물대포!" << endl;
            break;
        case ElementType::EARTH:
            cout << "지진!" << endl;
            break;
        case ElementType::WIND:
            cout << "돌풍!" << endl;
            break;
    }

    // ---------------------------------------------------------
    // [메모리 검증] 눈으로 직접 확인해봅시다! (중요)
    // ---------------------------------------------------------

    // TODO 5: ElementType(Enum)이 실제로 몇 바이트를 차지하는지 출력하세요.
    // 예상: 4바이트 (int 취급)
    // 힌트: sizeof(ElementType)
    cout << sizeof(ElementType) << endl;


    // TODO 6: Character(Struct)가 실제로 몇 바이트를 차지하는지 출력하세요.
    // 예상: string 크기 + int(4) + eum(4) + 패딩(?)
    // 힌트: sizeof(Character)
    cout << sizeof(hero->hp) << endl;
    cout << sizeof(&hero) << endl; // 주소의 주소 즉 8바이트
    cout << sizeof(hero) << endl; // 포인터 변수는 주소를 저장하기에 8바이트
    cout << sizeof(*hero) << endl; // 이렇게 해야 전체 크기가 나옴 
    // 근데 왜 48일까? string 크기 재보니까 40이던데

    // TODO 6-1: [심화] 순서에 따른 크기 차이를 직접 증명해봅시다.
    // main 함수 위에 아래 두 구조체를 정의하고 sizeof를 출력해보세요.
    /*
    struct BadStruct { char a; double b; char c; };
    struct GoodStruct { double b; char a; char c; };
    */
    cout << "Bad Padding: " << sizeof(struct BadStruct) << endl;  // 예상: 24
    cout << "Good Packing: " << sizeof(struct GoodStruct) << endl; // 예상: 16


    // ---------------------------------------------------------
    // 여기서부터는 vector와 결합하는 연습입니다! (누적 학습)
    // ---------------------------------------------------------

    // TODO 7: Character 구조체를 담을 수 있는 빈 벡터 'party'를 선언하세요.
    // 힌트: vector<Character> party;

    vector<Character> party;


    // TODO 8: 'party'에 다음 캐릭터들을 추가하세요. (push_back 사용)
    // 1. 이름: "Slime", HP: 10, 속성: WATER
    // 2. 이름: "Orc",   HP: 50, 속성: EARTH
    // 힌트: party.push_back({ "Name", 10, WATER });
    party.push_back({ "Slime", 10, ElementType::WATER });
    party.push_back({ "Orc", 50, ElementType::EARTH });


    // TODO 9: 범위 기반 for문을 사용하여 'party'에 있는 모든 캐릭터의 정보를 출력하세요.
    // 출력 형식: [이름] HP: 00 (속성: 0)
    // 힌트: for (const auto& chara : party) { ... }
    for (const auto& chara : party) {
        cout << '[' << chara.name << ']' << " hp: " << chara.hp << " (속성: " <<  chara.element << ")" << endl;
    }


    return 0;
}
