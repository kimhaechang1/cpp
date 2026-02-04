#include <iostream>
#include <vector>

// [Gemini Agent]
// 0x04 연결 리스트 (Linked List) - Memory Pool Implementation
// "Struct but with Array speed"
// 힙 할당(new) 없이 미리 잡아둔 배열(Pool)에서 노드를 꺼내 쓰는 방식입니다.
// 게임 엔진, 임베디드, 고성능 서버에서 필수적인 "Object Pooling" 패턴의 기초입니다.

using namespace std;

// 1. 노드 정의 (Struct)
struct Node {
    int data;
    Node* next; // 포인터를 쓰지만, 가리키는 곳은 pool 배열 내부입니다.
};

// 2. 메모리 풀 할당 (Global or Class Member)
const int POOL_SIZE = 10000;
Node pool[POOL_SIZE]; // 힙/스택이 아닌 BSS(Data) 영역 (전역변수일 때)
int pool_idx = 0;     // "다음에 사용할 빈 노드"의 인덱스

// 3. 할당기 (Allocator)
// new Node(val) 대신 이 함수를 호출합니다.
Node* my_alloc(int val) {
    if (pool_idx >= POOL_SIZE) {
        cout << "Error: Memory Pool is Full!" << endl;
        return nullptr;
    }
    // TODO 1: pool 배열에서 pool_idx 번째 노드의 주소를 가져옵니다.
    Node* curr = &pool[pool_idx];

    // TODO 2: 가져온 노드의 data를 초기화하고, next를 nullptr로 설정합니다.
    curr->data = val;
    curr->next = nullptr;

    // TODO 3: pool_idx를 1 증가시킵니다.
    pool_idx++;

    return curr; // (수정 필요) 할당한 노드 주소 반환
}

// Global Head Pointer
Node* head = nullptr;

// 리스트 앞에 추가 (기존 포인터 방식과 로직은 같음, 할당만 다름)
void pushFront(int val) {
    // TODO 4: my_alloc을 사용하여 새 노드를 가져옵니다.
    Node* newNode = my_alloc(val);

    // TODO 5: 새 노드를 연결 리스트의 맨 앞에 연결합니다.
    newNode->next = head;
    head = newNode;
}

void printList() {
    Node* curr = head;
    while (curr != nullptr) {
        cout << curr->data << " -> ";
        curr = curr->next;

        // 디버깅 팁: 이 노드가 pool의 몇 번째 칸인지 확인해봅시다.
        long long offset = curr - pool; // 포인터 연산 (인덱스 차이 계산)
        cout << "(idx: " << offset << ") ";
    }
    cout << "nullptr" << endl;
}

int main() {
    cout << "=== Memory Pool Linked List Practice ===" << endl;
    cout << "Pool Base Address: " << pool << endl;

    pushFront(10);
    pushFront(20);
    pushFront(30);

    printList();
    // 예상 출력: 30 -> (idx: 2) 20 -> (idx: 1) 10 -> (idx: 0) nullptr
    // 설명: 10이 먼저 할당(idx 0), 20이 그다음(idx 1), 30이 마지막(idx 2)
    // 리스트 연결 순서는 30 -> 20 -> 10 이지만, 메모리 주소는 0 -> 1 -> 2 순서대로 배치됨 (Cache Friendly!)

    return 0;
}
