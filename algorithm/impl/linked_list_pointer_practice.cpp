#include <iostream>

// [Gemini Agent]
// 0x04 연결 리스트 (Linked List) - Pointer Implementation
// 구조체와 포인터를 사용한 정석적인 연결 리스트 구현 실습입니다.
// 메모리를 효율적으로 쓰지만, 매번 new/delete를 해야 하는 단점이 있습니다.

using namespace std;

struct Node {
    int data;
    Node* next;
};

// 리스트의 첫 번째 노드를 가리키는 포인터
Node* head = nullptr;

// 리스트 출력 함수
void printList() {
    Node* curr = head;
    while (curr != nullptr) {
        cout << curr->data << " -> ";
        curr = curr->next;
    }
    cout << "nullptr" << endl;
}

// 리스트의 *맨 앞*에 노드 추가 (O(1))
void pushFront(int val) {
    // TODO 1: 새로운 노드를 동적 할당(new)하고 값을 넣으세요.
    Node* newNode = new Node{val, nullptr};

    // TODO 2: 새 노드의 next가 현재 head를 가리키게 하세요.
    // 헤드는 항상 젤 앞노드를 가리키고 있어야 한다.
    // 헤드가 지금까지 가리키고 있던 노드를 현재 젤앞이될 노드 다음으로 붙인다.
    newNode->next = head;

    // TODO 3: head가 새 노드를 가리키게 하세요.
    head = newNode;
}

// 특정 노드(prevNode) *뒤*에 새 노드 추가 (O(1))
void insertAfter(Node* prevNode, int val) {
    if (prevNode == nullptr) {
        cout << "이전 노드가 NULL입니다." << endl;
        return;
    }
    // TODO 4: 새 노드 할당 & 값 저장
    Node* newNode = new Node{val, nullptr};

    // TODO 5: 새 노드의 next 처리 (중간에 끼워넣기)
    Node* nextNode = prevNode->next;
    newNode->next = nextNode;

    // TODO 6: 앞 노드의 next 처리
    prevNode->next = newNode;
}

// 리스트에서 특정 값을 가진 *첫 번째* 노드 삭제 (O(N))
void deleteNode(int key) {
    // TODO 7: head가 삭제 대상인 경우 처리
    // 헤드가 더미노드가 아니고 가리키는 대상이 있기 때문
    if (head == nullptr) return;
    Node* targetNode = nullptr;
    if (head->data == key) {
        Node* temp = head;
        head = head->next;
        delete temp;
        return;
    }
    else {
        // TODO 8: 순회하며 삭제할 노드(target)와 그 앞 노드(prev) 찾기
        Node* curr = head;
        Node* prev = nullptr;
        bool found = false;

        while (curr != nullptr) { // 끝까지 순회
            if (curr->data == key) {
                found = true;
                break;
            }
            prev = curr;
            curr = curr->next;
        }

        // TODO 9: 찾았으면 연결 끊고(prev->next = target->next) 메모리 해제(delete)
        if (found) {
            Node* next = curr->next;
            if (prev != nullptr) {
                prev->next = next;
            }
            delete curr;
        }
    }
}

int main() {
    cout << "=== Pointer Linked List Practice ===" << endl;

    // 1. 값 추가 테스트
    pushFront(10); // 10 -> nullptr
    pushFront(20); // 20 -> 10 -> nullptr
    pushFront(30); // 30 -> 20 -> 10 -> nullptr
    printList();

    // 2. 중간 삽입 테스트
    // head(30) 뒤에 25 넣기
    insertAfter(head, 25); // 30 -> 25 -> 20 -> 10 -> nullptr
    printList();

    // 3. 삭제 테스트
    deleteNode(20); // 30 -> 25 -> 10 -> nullptr
    printList();

    deleteNode(30); // (head 삭제) 25 -> 10 -> nullptr
    printList();

    // 4. 메모리 해제 (Free)
    // 실제로는 프로그램 종료 전 모든 노드를 delete 해야 함 (RAII가 아니므로)
    // 여기서는 생략하지만, Module 05에서 자세히 다룹니다!

    return 0;
}
