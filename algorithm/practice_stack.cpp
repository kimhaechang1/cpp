#include <iostream>
#include <vector>
#include <cassert>

// -------------------------------------------------------------
// [Algorithm 0x05] Stack Implementation (Array Ver.)
// 목표: STL stack을 쓰지 않고, 배열과 인덱스만으로 스택을 구현해보세요.
// -------------------------------------------------------------

const int MX = 1000005; // 충분히 큰 크기
int dat[MX]; // 데이터를 담을 배열
int pos = 0; // 다음 원소가 들어갈 인덱스 (Size 역할도 겸함)

// 1. push: 스택에 x를 추가하는 함수
void push(int x) {
    // TODO: 구현하세요.
    // dat 배열의 pos 위치에 x를 넣고, pos를 증가시킵니다.
    dat[pos] = x;
    pos++;
}

// 2. pop: 스택의 꼭대기(top) 원소를 제거하는 함수
void pop() {
    // TODO: 구현하세요.
    // pos를 감소시킵니다. (0보다 작아지지 않도록 주의?)
    // PS에서는 보통 입력 조건이 pop을 할 수를 보장하지만, 여기선 간단히 체크해도 좋습니다.
    if (pos == 0) return;
    dat[pos] = -1;
    pos--;
}

// 3. top: 스택의 꼭대기(top) 원소를 반환하는 함수
int top() {
    // TODO: 구현하세요.
    // pos가 가리키는 곳은 '비어있는' 곳이므로, pos-1 위치의 값을 반환해야 합니다.
    if (pos - 1 < 0) return -1;
    return dat[pos - 1];
}

// 4. test: 스택이 비었는지 확인하는 함수 (비었으면 true, 아니면 false)
// 보통 STL에선 empty()라고 합니다.
bool empty() {
    // pos가 0이면 빈 것입니다.
    return pos == 0; // Dummy return
}

// -------------------------------------------------------------
// 메인 함수 (테스트)
// -------------------------------------------------------------
void test() {
    push(5); push(4); push(3);
    assert(top() == 3); // 5, 4, 3(top)

    pop(); pop();
    assert(top() == 5); // 5(top)

    push(10); push(12);
    assert(top() == 12); // 5, 10, 12(top)

    pop();
    assert(top() == 10); // 5, 10(top)

    assert(empty() == false);
    pop(); pop();
    assert(empty() == true); // empty

    std::cout << "Stack Test Passed!" << std::endl;
}

int main() {
    test();
    return 0;
}
