#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

const int MX = 1000005;
int dat[MX], pre[MX], nxt[MX];
int unused = 1;

// [Gemini Agent]
// 0x04 연결 리스트 (Linked List) - Array Implementation 실습
// STL list를 사용하지 않고 배열 3개로 연결 리스트를 직접 구현합니다.

void traverse() {
    int cur = nxt[0];
    while (cur != -1) {
        cout << dat[cur] << ' ';
        cur = nxt[cur];
    }
    cout << "\n\n";
}

void traverse1() {
    int cur = nxt[0];
    while (cur != -1) {
        cout << dat[cur] << ' ' << endl;
        cout << "currentAddr: " << cur << endl;
        cout << "prevAddr: " << pre[cur] << endl;
        cout << "nextAddr: " << nxt[cur] << endl;
        if (pre[cur] != -1) {
            cout << "prevValue: " << dat[pre[cur]] << endl;
        }
        if (nxt[cur] != -1) {
            cout << "nextValue: " << dat[nxt[cur]] << endl;
        }
        cur = nxt[cur];
    }
    cout << "\n\n";
}

// addr 번지 뒤에 num을 추가
void insert(int addr, int num) {
    // TODO 1: 새로운 노드(unused)에 값(num)을 저장하고, 연결 관계를 갱신하세요.
    // 1. dat[unused] 에 값 저장
    // 2. 새 노드의 pre, nxt 설정 (addr과 addr의 다음 노드 사이)
    // 3. addr의 다음 노드의 pre 재설정 (조심: 다음 노드가 -1일 수도 있음? 아니면 더미라 괜찮?) 
    //    -> PS용 구현에서는 더미가 맨 앞(0)에만 있고 맨 뒤는 -1로 끝남.
    //    -> 따라서 nxt[addr]이 -1이 아닐 때만 pre 설정해줘야 함.
    // 4. addr의 nxt 재설정
    // 5. unused 증가
    // 노드생성
    dat[unused] = num;
    int nextAddr = nxt[addr];
    nxt[addr] = unused;
    nxt[unused] = nextAddr;
    if (nextAddr != -1) {
        pre[nextAddr] = unused;
    }
    pre[unused] = addr;
    unused++;
    
}

// addr 번지 노드를 삭제
void erase(int addr) {
    // TODO 2: addr 노드를 리스트에서 제거하세요. (연결만 끊으면 됨)
    // 1. addr의 이전 노드의 nxt를 addr의 다음 노드로 변경
    // 2. addr의 다음 노드의 pre를 addr의 이전 노드로 변경 (단, 다음 노드가 -1이 아닐 때)
    int nextAddr = nxt[addr];
    int preAddr = pre[addr];
    
    if (preAddr != -1) {
        nxt[preAddr] = nextAddr;
    }
    if (nextAddr != -1) {
        pre[nextAddr] = preAddr;
    }
}

void fill_test() {
    fill(pre, pre + MX, -1);
    fill(nxt, nxt + MX, -1);
    unused = 1; // 0번지는 Dummy Node
}

void test1() {
    cout << "****** TEST 1 ******\n";
    fill_test();
    cout << pre[0] << " " << nxt[0] << endl;
    insert(0, 10); // 10
    traverse1();
    //traverse();
    insert(0, 30); // 30 10
    traverse();
    insert(2, 40); // 30 40 10
    traverse();
    insert(1, 20); // 30 40 10 20
    traverse();
    insert(4, 70); // 30 40 10 20 70
    traverse();
}

void test2() {
    cout << "****** TEST 2 ******\n";
    fill_test();
    insert(0, 30); // 30
    insert(1, 40); // 30 40
    insert(2, 50); // 30 40 50
    insert(3, 12345); // 30 40 50 12345
    erase(1); // 40 50 12345 (30 삭제됨)
    traverse();
    erase(2); // 40 12345 (50 삭제됨)
    traverse();
    erase(4); // 40 (12345 삭제됨)
    traverse();
    erase(3); // (empty, 40 삭제됨)
    traverse();
}

int main(void) {
    test1();
    test2();
    return 0;
}
