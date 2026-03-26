#include <iostream>
#include <concepts>
#include <vector>
#include <string>
#include<ostream>

// [Q] 타입 T가 '공격력(power)' 멤버를 가지고 있어야 한다는 '제약 조건(Concept)'을 정의하세요.
// TODO: HasPower 컨셉 선언 (v.power 가 유효해야 함)
template<typename T>
concept HasPower = requires(T v) {
    v.power;
};

// [Q] 타입 T가 '출력 가능(Printable)' 해야 한다는 '제약 조건'을 정의하세요.
// TODO: Printable 컨셉 선언 (std::cout << v 가 유효해야 함)

template<typename T>
concept Printable = requires(T v) {
    // cout << v 가 되야 하니까
    std::cout << v;
};


// ---------------------------------------------------------------------------
// [TASK] 아래 템플릿 함수에 위에서 만든 컨셉들을 적용하세요.
// ---------------------------------------------------------------------------

// 1. 공격력이 있는 객체만 받아서 데미지를 출력하는 함수
// template <TODO: HasPower T>
template<HasPower T>
void PrintAttackPower(T const& target) {
     std::cout << "Target Power: " << target.power << std::endl;
}

// 2. 출력 가능한 타입만 받아서 값을 출력하는 제네릭 함수
template<Printable T>
void SafePrint(T const& value) {
     std::cout << "[VALUE] " << value << std::endl;
}

// 테스트용 구조체
struct Warrior { int power{ 100 }; };
struct Potion { int healAmount{ 50 }; }; // power 가 없음!

int main() {
    Warrior w;
    Potion p;

    // [성공 케이스]
    PrintAttackPower(w); // OK
     SafePrint(12345);    // OK (int는 출력 가능)
     SafePrint("Hello");  // OK (string은 출력 가능)

    // [실패 케이스 - 주석 해제 시 컴파일 에러가 나야 함]
    // PrintAttackPower(p); // Error: Potion은 power 가 없음!
    // struct NonPrintable {};
    // SafePrint(NonPrintable{}); // Error: << 연산자가 정의되지 않음!

    std::cout << "[SUCCESS] Concepts Practice Finished!" << std::endl;
    return 0;
}
