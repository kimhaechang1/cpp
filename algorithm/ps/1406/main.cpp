#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <string>
#include <list>

using namespace std;

struct Node {
    char val;
    Node* next;
    Node* prev;
};

Node* head;
Node* cursor; 
// 커서가 가리키는 노드는 노드의 뒤에 커서가 있다고 생각하면 된다.
// 그래야 제일 왼쪽 커서가 초기로 잡을 수 있다.
// 헤드를 가리키고 있다면 헤드 뒤를 가리킨다고 생각해야하고 이는 곧 맨앞 커서가 된다.
// 맨 뒷노드를 가리키고 있다면, 커서는 가장 마지막 글자 오른쪽에 위치한 것이 된다.

void P(char ch) {
    Node* newNode = new Node{
        ch, nullptr, nullptr
    };
    Node* nextNode = cursor->next;
    cursor->next = newNode;
    newNode->next = nextNode;
    if (nextNode != nullptr) {
        nextNode->prev = newNode;
    }
    
    newNode->prev = cursor;
}

void D() {
    if (cursor->next == nullptr) {
        return;
    }
    cursor = cursor->next;
}

void L() {
    if (cursor->prev == nullptr) {
        return;
    }
    cursor = cursor->prev;
}

void B() {
    if (cursor == head) return;
    Node* target = cursor;
    Node* prevNode = cursor->prev;
    Node* nextNode = cursor->next;
    
    if (prevNode != nullptr) {
        prevNode->next = nextNode;
        cursor = prevNode;
    }
    if (nextNode != nullptr) {
        nextNode->prev = prevNode;
    }
    delete target;
}


void Init(string init_val) {
    head = new Node{
        '0', nullptr, nullptr
    };
    cursor = head;

    for (char ch : init_val) {
        P(ch);
        D();
    }
}

void Debug() {
    Node* nxt = head->next;
    while (nxt != nullptr) {
        cout << nxt->val << "->>";
        nxt = nxt->next;
    }
    cout << endl;
}

void PrintCursor() {
    cout << "현재 커서: " << cursor->val << endl;
}

void Print() {
    Node* nxt = head->next;
    while (nxt != nullptr) {
        cout << nxt->val;
        nxt = nxt->next;
    }
}

vector<string> Split(string input, char delim) {
    vector<string> result;
    istringstream iss(input);
    string buf;
    while (getline(iss, buf, delim)) {
        result.push_back(buf);
    }

    return result;
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    string N;
    cin >> N;
    Init(N);
    int M;
    cin >> M;
    //Debug();
    //PrintCursor();
    //cout << "###################################" << endl;
    for (int i = 0; i < M; i++) {
        char cmd;
        cin >> cmd;
        
        if (cmd == 'P') {
            char val;
            cin >> val;
            P(val);
            D();
        }
        else if (cmd == 'L') {
            L();
        }
        else if (cmd == 'D') {
            D();
        }
        else if (cmd == 'B') {
            B();
        }
        //Debug();
        //PrintCursor();
        //cout << "##########################################" << endl;
    }
    Print();
    
    return 0;
}
