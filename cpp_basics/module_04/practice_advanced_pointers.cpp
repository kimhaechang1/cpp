#include <iostream>
#include <string>

// [실습 설명]
// Module 04 보충: const 참조(const T&)와 이중 포인터(**) 연습
// 동적 할당(new/delete) 없이 순수하게 포인터와 참조의 문법을 익힙니다.

struct Stat {
    int hp;
    int mp;
    int str;
    int dex;
};

// -------------------------------------------------------------
// [1] const T& (Const Reference)
// -------------------------------------------------------------
// 거대한 구조체를 복사 없이 전달하되, 수정은 못하게 막아야 합니다.
// TODO: 매개변수를 'const Stat&'로 변경하세요.
void PrintStat(const Stat& s) {
    std::cout << "[Stat] HP:" << s.hp << " MP:" << s.mp << "\n";
    //s.hp = 9999;
    // s.hp = 9999; // const로 바꾸면 이 줄이 에러가 나야 합니다.
}

// -------------------------------------------------------------
// [2] 이중 포인터 (Double Pointer)
// -------------------------------------------------------------
// "포인터가 가리키는 대상"을 함수 안에서 바꾸고 싶습니다.
// 타겟 변수(target)가 a를 가리키다가 b를 가리키게 만들어 보세요.
void RedirectPointer(int** pp, int* newTarget) {
    if (pp == nullptr) return;

    // TODO: pp를 역참조하여, 바깥의 포인터 변수가 newTarget을 가리키게 하세요.
    // *pp = ...
    //**pp = *newTarget;
    // 여기서 pp 는 포인터를 가리키는 포인터고
    // pp -> p -> a 구조가 생성됨
    // 근데 **pp = *newTarget 은
    // pp -> p -> 의 값을 newTarget -> 의 값으로 교체하라는것
    // 만약 매게변수가 int* pp 였다면
    // pp 라는 변수명을 가진 스택메모리속에서 a 를 가리키는 새로운 포인터변수가 됨

    // 서로다른 함수스택간에 변수는 pointer 로 가리킬 수 있음
    // 여기서 원하는건 p 가 가리키는 대상을 newTarget 이 가리키는것으로 바꿔야함
    // 즉, pp -> p -> a 에서 pp -> p -> b 로 바꿔야하는데
    // **pp = *newTarget 을 해버리면 pp -> p -> a 에서 a 에 들어있는 값이 바뀌게 됨
    // 그게아니라 b 변수를 가리키게 해야하므로
    // pp 가 가리키는 포인터변수에 a 주소가 담겨있는것을 b 주소로 바꿔야함
    *pp = newTarget;
}

int main() {
    std::cout << "=== Module 04 Advanced Practice ===\n";

    // 1. const T& Test
    Stat myStat = { 100, 50, 10, 5 };
    PrintStat(myStat);

    // 2. Double Pointer Test
    int a = 10;
    int b = 20;
    int* ptr = &a; // ptr은 현재 a를 가리킴
    // 현재는 ptr 이라는 포인터는 a 라는 데이터를 가리킴

    std::cout << "\n[Before] *ptr: " << *ptr << " (Target: a)\n";

    // TODO: RedirectPointer를 호출하여 ptr이 b를 가리키게 만드세요.
    // 힌트: ptr의 '주소'를 넘겨야 int**가 받을 수 있습니다.
    // RedirectPointer( ... );
    RedirectPointer(&ptr, &b);

    std::cout << "[After]  *ptr: " << *ptr << " (Target: b?)\n";

    if (*ptr == 20) std::cout << ">> Success: Pointer Redirected!\n";
    // 따라서 *ptr 이 b 의 값으로 교체되게 된다.
    else std::cout << ">> Fail: Still pointing to a or garbage.\n";

    return 0;
}
