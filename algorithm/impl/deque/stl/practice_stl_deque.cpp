// 0x07 Deque - STL std::deque Practice
// concept_deque_stl.md 참고
//
// [직접 구현과의 핵심 차이]
// - pop_front() / pop_back() : void 반환 (값 없음!)
//   값이 필요하면 front() / back() 으로 먼저 읽은 후 pop 해야 함
// - empty() : int 1/0 이 아닌 bool (true/false) 반환
// - size()  : size_t (unsigned) 반환

#include <iostream>
#include <deque>
#include <cassert>

using namespace std;

int main() {
    deque<int> dq;

    // [TEST 1] push_back / push_front / size / front / back
    dq.push_back(10);
    dq.push_back(20);
    dq.push_front(5);
    // 상태: [5, 10, 20]
    assert(dq.size() == 3);
    assert(dq.front() == 5);
    assert(dq.back() == 20);
    cout << "[TEST 1] PASS: push, size, front, back\n";

    // [TEST 2] pop_front (주의: void 반환 - 값 먼저 읽고 pop)
    int frontVal = dq.front();
    dq.pop_front();
    assert(frontVal == 5);
    assert(dq.front() == 10);
    cout << "[TEST 2] PASS: pop_front\n";

    // [TEST 3] pop_back
    int backVal = dq.back();
    dq.pop_back();
    assert(backVal == 20);
    assert(dq.back() == 10);
    cout << "[TEST 3] PASS: pop_back\n";

    // [TEST 4] empty() returns bool
    dq.pop_back();
    assert(dq.empty() == true);
    assert(dq.size() == 0);
    cout << "[TEST 4] PASS: empty check\n";

    // [TEST 5] index access with []  (std::deque supports random access)
    dq.push_back(100);
    dq.push_back(200);
    dq.push_back(300);
    assert(dq[0] == 100);
    assert(dq[1] == 200);
    assert(dq[2] == 300);
    cout << "[TEST 5] PASS: operator[] random access\n";

    // [TEST 6] range-based for
    cout << "[TEST 6] Range-based for: ";
    for (int val : dq) {
        cout << val << " ";
    }
    cout << "\n";

    cout << "\nAll tests passed!\n";
    return 0;
}
