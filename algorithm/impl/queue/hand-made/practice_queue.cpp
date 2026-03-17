// 0x06 Queue - Array-based Direct Implementation
// concept_queue.md 참고
//
// [핵심 원리]
// - head : 꺼낼 원소의 위치 (실제 데이터가 있는 인덱스)
// - tail : 다음에 데이터가 들어올 빈 공간의 인덱스
// - size() = tail - head
// - empty() = (head == tail)
// - 데이터를 실제로 지우지 않고, head 포인터만 뒤로 밀어서 논리적 삭제

#include <iostream>
#include <cassert>

using namespace std;

const int MX = 1000005;
int dat[MX];
int head = 0;
int tail = 0;

int Size() {
    return tail - head;
}

int Empty() {
    return (head == tail) ? 1 : 0;
}

void Push(int x) {
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

int Pop() {
    if (Empty()) return -1;
    int val = dat[head];
    head++;
    return val;
}

void Reset() {
    head = 0;
    tail = 0;
}

int main() {
    // [TEST 1] push / size / front / back
    Push(10);
    Push(20);
    Push(30);
    assert(Size() == 3);
    assert(Front() == 10);
    assert(Back() == 30);
    cout << "[TEST 1] PASS: push, size, front, back\n";

    // [TEST 2] pop (FIFO 순서 확인)
    assert(Pop() == 10);
    assert(Pop() == 20);
    assert(Size() == 1);
    assert(Front() == 30);
    cout << "[TEST 2] PASS: pop (FIFO order)\n";

    // [TEST 3] empty after all popped
    Pop();
    assert(Empty() == 1);
    assert(Front() == -1);
    assert(Back() == -1);
    assert(Pop() == -1);
    cout << "[TEST 3] PASS: empty checks\n";

    // [TEST 4] push 후 순서 보장
    Reset();
    int order[] = {5, 3, 8, 1, 9};
    for (int v : order) Push(v);
    for (int v : order) {
        assert(Pop() == v);
    }
    assert(Empty() == 1);
    cout << "[TEST 4] PASS: FIFO order guarantee\n";

    cout << "\nAll tests passed!\n";
    return 0;
}
