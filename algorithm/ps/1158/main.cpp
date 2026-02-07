#include <iostream>
#include <vector>
#include <list> 

using namespace std;

// [BOJ 1158] 요세푸스 문제
// https://www.acmicpc.net/problem/1158

struct Node {
    int val;
    Node* prev;
    Node* next;
};

Node* head = new Node{-1, nullptr, nullptr};
Node* tail = new Node{ -1, nullptr, nullptr };
Node* cursor;

void Push_Back(int val) {
    Node* newNode = new Node{ val, nullptr, nullptr };
    Node* prevNode = tail->prev;
    prevNode->next = newNode;
    newNode->next = tail;
    tail->prev = newNode;
    newNode->prev = prevNode;
}

void Move(int k) {
    while (k-- > 1) {
        cursor = cursor->next;
    }
    //cout << "cursor: " << cursor->val << endl;
}

vector<int> results;

void Delete_And_Print() {
    Node* targetNode = cursor;
    Node* prevNode = targetNode->prev;
    Node* nextNode = targetNode->next;

    if (prevNode != targetNode && nextNode != targetNode) {
        if (prevNode != nullptr) {
            prevNode->next = nextNode;
        }
        if (nextNode != nullptr) {
            nextNode->prev = prevNode;
        }
        cursor = nextNode;
    }
    results.push_back(targetNode->val);
    delete targetNode;
}


void Init(int max) {
    head->next = tail;
    tail->prev = head;
    for (int i = 1; i <= max; i++) {
        Push_Back(i);
    }
    cursor = head->next;

    Node* lastNode = tail->prev;
    Node* firstNode = head->next;

    firstNode->prev = lastNode;
    lastNode->next = firstNode;
}



int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int N, K;
    cin >> N >> K;
    // 중요한 점은 계속 동적으로 길이조절이 되야한다는거지
    // Search(), DeleteAndPrint(), Push()
    // 1 2 3 4 5 6 7
    // 처음과 끝을 연결하는식으로 가야할듯
    Init(N);
    while (N-- > 0) {
        Move(K);
        Delete_And_Print();
    }
   
    // 예시 출력 형식: <3, 6, 2, 7, 5, 1, 4>
    cout << "<";
    if (results.size() == 1) {
        cout << results[0];
    }
    else {
        for (int i = 0; i < results.size() - 1; i++) {
            cout << results[i] << ", ";
        }
        cout << results[results.size() - 1];
    }

    cout << ">";

    return 0;
}
