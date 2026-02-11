#include <iostream>
#include <stack>
#include <vector>

// BOJ 10773: 제로
// 문제 링크: https://www.acmicpc.net/problem/10773

int stack[100005] = {-1,};
int pos = 0;

void Push(int x) {
    stack[pos] = x;
    pos++;
}

void Pop() {
    if (pos == 0) return;
    stack[pos - 1] = -1;
    pos--;
}

int main() {
    // Fast IO
    std::ios::sync_with_stdio(0);
    std::cin.tie(0);

    // 가장 최근에 입력된 수를 지운다. 그것은 마지막 입력수가 뭔지 알아야함
    int K;
    std::cin >> K;
    while (K-- > 0) {
        int val;
        std::cin >> val;
        if (val == 0) {
            Pop();
            continue;
        }
        Push(val);
    }
    int sum = 0;
    for (int i = 0; i < pos; i++) {
        if (stack[i] == -1) continue;
        sum += stack[i];
    }
    
    std::cout << sum;
    return 0;
}
