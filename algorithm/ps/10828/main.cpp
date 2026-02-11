#include <iostream>
#include <stack>
#include <string>
#include <sstream>

// BOJ 10828: 스택
// 문제 링크: https://www.acmicpc.net/problem/10828
int stack[10005] = {-1,};
int pos = 0;

void Push(int x) {
    stack[pos] = x;
    pos++;
}

int Size() {
    return pos;
}

int Empty() {
    if (Size() == 0) {
        return 1;
    }
    return 0;
}

int Top() {
    if (Empty() == 1) {
        return -1;
    }
    return stack[pos - 1];
}

int Pop() {
    if (Empty() == 1) {
        return -1;
    }
    int rs = stack[pos - 1];
    pos--;
    return rs;
}



int main() {
    // Fast IO
    std::ios::sync_with_stdio(0);
    std::cin.tie(0);
    std::stringstream stream;


    // TODO: 구현하세요.
    int N;
    std::cin >> N;
    while (N-- > 0) {
        std::string cmd;
        std::cin >> cmd;
        if (cmd == "push") {
            int x;
            std::cin >> x;
            Push(x);
        }
        if (cmd == "pop") {
            stream << Pop() << std::endl;
        }
        if (cmd == "top") {
            stream << Top() << std::endl;
        }
        if (cmd == "size") {
            stream << Size() << std::endl;
        }
        if (cmd == "empty") {
            stream << Empty() << std::endl;
        }
    }
    std::cout << stream.str();

    return 0;
}
