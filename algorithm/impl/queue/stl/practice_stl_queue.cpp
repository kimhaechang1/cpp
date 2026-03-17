// 0x06 Queue - STL std::queue Practice
// concept_stl_queue.md 참고
//
// [직접 구현과의 핵심 차이]
// - push(x)  : 뒤에 삽입 (직접 구현의 Push(x)와 동일)
// - pop()    : void 반환 - 값이 필요하면 front()로 먼저 읽어야 함!
// - front()  : 맨 앞 원소 반환 (읽기만)
// - back()   : 맨 뒤 원소 반환 (읽기만)
// - empty()  : bool 반환 (true/false), 직접 구현은 int (1/0)
// - size()   : size_t (unsigned) 반환
//
// [내부 구조]
// std::queue 는 컨테이너 어댑터 - 기본 뼈대는 std::deque 사용

#include <iostream>
#include <queue>
#include <cassert>

using namespace std;

int main() {
    queue<int> q;

    // [TEST 1] push / size / front / back
    q.push(10);
    q.push(20);
    q.push(30);
    assert(q.size() == 3);
    assert(q.front() == 10);
    assert(q.back() == 30);
    cout << "[TEST 1] PASS: push, size, front, back\n";

    // [TEST 2] pop (주의: void 반환 - 값 먼저 읽고 pop)
    int val = q.front();
    q.pop();
    assert(val == 10);
    assert(q.front() == 20);
    assert(q.size() == 2);
    cout << "[TEST 2] PASS: pop (read first, then remove)\n";

    // [TEST 3] empty() returns bool
    q.pop();
    q.pop();
    assert(q.empty() == true);
    assert(q.size() == 0);
    cout << "[TEST 3] PASS: empty check\n";

    // [TEST 4] FIFO 순서 보장
    int order[] = {5, 3, 8, 1, 9};
    for (int v : order) q.push(v);
    for (int v : order) {
        assert(q.front() == v);
        q.pop();
    }
    assert(q.empty());
    cout << "[TEST 4] PASS: FIFO order guarantee\n";

    cout << "\nAll tests passed!\n";
    return 0;
}
