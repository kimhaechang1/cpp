// 0x07 Deque - Array-based Direct Implementation
// concept_deque.md 참고
// head = MX (중앙) 에서 양쪽으로 뻗어나가는 방식

#include <iostream>
#include <cassert>

using namespace std;

const int MX = 10000;
int dat[2 * MX + 5];
int head = MX;
int tail = MX;

int Size() {
    return tail - head;
}

int Empty() {
    return (head == tail) ? 1 : 0;
}

void Push_Front(int x) {
    head--;
    dat[head] = x;
}

void Push_Back(int x) {
    dat[tail] = x;
    tail++;
}

int Front() {
    if (Empty()) return -1;
    return dat[head];
}

int Back() {
    if (Empty()) return -1;
    return dat[tail - 1];
}

int Pop_Front() {
    if (Empty()) return -1;
    int val = dat[head];
    head++;
    return val;
}

int Pop_Back() {
    if (Empty()) return -1;
    int val = dat[tail - 1];
    tail--;
    return val;
}

void Reset() {
    head = MX;
    tail = MX;
}

int main() {
    // [TEST 1] push_back / push_front / size
    Push_Back(10);
    Push_Back(20);
    Push_Front(5);
    // 상태: [5, 10, 20]
    assert(Size() == 3);
    assert(Front() == 5);
    assert(Back() == 20);
    cout << "[TEST 1] PASS: push, size, front, back\n";

    // [TEST 2] pop_front
    assert(Pop_Front() == 5);
    assert(Front() == 10);
    assert(Size() == 2);
    cout << "[TEST 2] PASS: pop_front\n";

    // [TEST 3] pop_back
    assert(Pop_Back() == 20);
    assert(Back() == 10);
    assert(Size() == 1);
    cout << "[TEST 3] PASS: pop_back\n";

    // [TEST 4] empty on last pop
    Pop_Back();
    assert(Empty() == 1);
    assert(Front() == -1);
    assert(Back() == -1);
    assert(Pop_Front() == -1);
    assert(Pop_Back() == -1);
    cout << "[TEST 4] PASS: empty checks\n";

    // [TEST 5] push_front repeated (head 방향으로 확장)
    Reset();
    Push_Front(3);
    Push_Front(2);
    Push_Front(1);
    // 상태: [1, 2, 3]
    assert(Pop_Front() == 1);
    assert(Pop_Front() == 2);
    assert(Pop_Front() == 3);
    assert(Empty() == 1);
    cout << "[TEST 5] PASS: push_front repeated\n";

    cout << "\nAll tests passed!\n";
    return 0;
}
