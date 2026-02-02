#include <iostream>
#include <string>

using namespace std;

// [구조체 정의]
struct Stat {
    int hp;
    int attack;
    int defense;
};

// [함수 정의 구역]
// TODO 1: 두 정수의 값을 서로 바꾸는 Swap 함수를 만드세요.
// (Call by Value와 Call by Reference의 차이를 확인하기 위함입니다)
// 이름 규칙: PascalCase (예: SwapValues)
void SwapValues_Value(int a, int b) {
    int temp = a;
    a = b;
    b = temp;
    cout << "[SwapValues_Value] 함수 내부: a=" << a << ", b=" << b << endl;
}

// TODO 1-2: 원본 값이 바뀌도록 참조(&)를 사용해 구현하세요.
void SwapValues_Ref(int& a, int& b) {
    // 여기에 구현하세요
    int temp = a;
    a = b;
    b = temp;
}


// TODO 2: 구조체를 const reference로 받아서 출력하는 함수를 만드세요.
// 이름: PrintStat
// 기능: "HP: {hp}, ATK: {attack}, DEF: {defense}" 형식으로 출력
// 힌트: void PrintStat(const Stat& s)
void PrintStat(const Stat& s) {
    // 여기에 구현하세요
    cout << "HP: " << s.hp << ", ATK: " << s.attack << ", DEF: " << s.defense << endl;
}


// TODO 3: 데미지 계산 함수를 만드세요. (반환값이 있는 함수)
// 이름: CalculateDamage
// 매개변수: damage(int), defense(int)
// 기능: damage - defense 결과를 반환하되, 0보다 작으면 0을 반환
// 힌트: std::max(0, damage - defense) 사용 가능
int CalculateDamage(int damage, int defense) {
    
    return max(0, damage - defense); // 임시 반환값
}


int main() {
    // 입출력 속도 향상
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    cout << "=== 1. Swap 테스트 ===" << endl;
    int num1 = 10;
    int num2 = 20;

    cout << "Original: " << num1 << ", " << num2 << endl;

    SwapValues_Value(num1, num2);
    cout << "After Value Swap: " << num1 << ", " << num2 << " (변경 없어야 함)" << endl;

    SwapValues_Ref(num1, num2);
    cout << "After Ref Swap: " << num1 << ", " << num2 << " (변경 되어야 함!)" << endl;


    cout << "\n=== 2. Struct 전달 테스트 ===" << endl;
    Stat heroStat = { 100, 15, 5 };
     PrintStat(heroStat); // 주석을 풀고 테스트하세요.


    cout << "\n=== 3. 반환값 테스트 ===" << endl;
    int atk = 20;
    int def = 5;
     int result = CalculateDamage(atk, def);
     cout << "데미지 결과: " << result << " (예상: 15)" << endl;

    return 0;
}
