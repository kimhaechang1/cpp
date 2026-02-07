#include <iostream>
#include <vector>
#include <algorithm>
#include <list>
#include <string>

using namespace std;

// [BOJ 5397] 키로거
// https://www.acmicpc.net/problem/5397

struct Node {
    char val;
    Node* next;
    Node* prev;
};

Node* head;
Node* cursor;

void Push(char v) {
    Node* newNode = new Node{ v, nullptr, nullptr };
    Node* nextNode = cursor->next;
    cursor->next = newNode;
    newNode->next = nextNode;
    if (nextNode != nullptr) {
        nextNode->prev = newNode;
    }
    newNode->prev = cursor;
}

void BackSpace() {
    if (cursor == head) return;
    Node* target = cursor;
    Node* nextNode = cursor->next;
    Node* prevNode = cursor->prev;
    if (prevNode != nullptr) {
        prevNode->next = nextNode;
        cursor = prevNode;
    }
    if (nextNode != nullptr) {
        nextNode->prev = prevNode;
    }
    // [Agent Assist] 메모리 누수 방지
    // 연결 리스트에서 노드를 제거할 때는 반드시 delete를 호출하여 메모리를 해제해야 합니다.
    delete target;
}

void Right() {
    if (cursor->next == nullptr) return;
    cursor = cursor->next;
}

void Left() {
    if (cursor == head) return;
    cursor = cursor->prev;
}

void Print() {
    Node* cur = head->next;
    while (cur != nullptr) {
        cout << cur->val;
        cur = cur->next;
    }
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int T;
    cin >> T;
    while (T--) {
        string L;
        cin >> L;
        // - << 지우기
        // <, > 커서이동
        head = new Node{ '0', nullptr, nullptr };
        cursor = head;
        for (char ch : L) {
            if (ch == '<') {
                Left();
            }
            else if (ch == '>') {
                Right();
            }
            else if (ch == '-') {
                BackSpace();
            }
            else {
                Push(ch);
                Right();
            }
            //Print();
            //cout << endl;
            //cout << "#######################" << endl;
        }
        Print();

        // [Agent Assist] 테스트 케이스 간 메모리 정리 (Cleanup)
        // 다음 테스트 케이스에서 새로운 노드들을 생성하기 전에, 
        // 이번 테스트 케이스에서 사용한 모든 노드(head 포함)를 삭제해야 합니다.
        // 그렇지 않으면 잃어버린 메모리(Memory Leak)가 계속 쌓이게 됩니다.
        Node* delNode = head;
        while (delNode != nullptr) {
            Node* next = delNode->next;
            delete delNode;
            delNode = next;
        }

        cout << "\n";
    }

    return 0;
}
