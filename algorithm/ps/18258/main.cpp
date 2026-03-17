#include <iostream>
#include <string>
#include<sstream>
using namespace std;
const int MX = 2000005;
int queue[MX];
int N;
int back = 0;
int front = 0;
void Push(int x);
int Pop();
int Size();
int Empty();
int Front();
int Back();

int Size() {
    return back - front;
}
int Empty() {
    return Size() == 0 ? 1 : 0;
}
void Push(int x) {
    queue[back % MX] = x;
    back++;
}
int Pop() {
    if (Empty() == 1) {
        return  -1;
    }
    int rt = queue[front];
    front++;
    return rt;
}
int Front() {
    return Empty() == 1 ? -1 : queue[front];
}
int Back() {
    return Empty() == 1 ? -1 : queue[back - 1];
}

int main() {
    // 코드를 작성해주세요.
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cin >> N;
    stringstream stream;
    for (int i = 0; i < N; i++) {
        string op;
        cin >> op;
        if (op == "push") {
            int x;
            cin >> x;
            Push(x);
        }
        else if (op == "pop") {
            stream << Pop() << endl;;
        }
        else if (op == "size") {
            stream << Size() << endl;
        }
        else if (op == "empty") {
            stream << Empty() << endl;
        }
        else if (op == "front") {
            stream << Front() << endl;
        }
        else if (op == "back") {
            stream << Back() << endl;
        }
    }
    cout << stream.str();
    return 0;
}
