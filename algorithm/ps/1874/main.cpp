#include <iostream>
#include <stack>
#include <vector>
#include <string>
#include <sstream>

// BOJ 1874: 스택 수열
// 문제 링크: https://www.acmicpc.net/problem/1874

int arr[100001] = { 0, };
int stack[100001] = { -1, };
int pos = 0;

std::stringstream stream;

void Push(int x) {
    stack[pos] = x;
    stream << "+" << std::endl;
    pos++;
}
int Peek() {
    if (pos == 0) return -1;
    return stack[pos - 1];
}

bool isEmpty() {
    return pos == 0;
}

int Pop() {
    if (pos == 0) return -1;
    int rt = stack[pos - 1];
    pos--;
    stream << "-" << std::endl;
    return rt;
}

void Debug() {
    for (int i = 0; i < pos; i++) {
        std::cout << stack[i] << " ";
    }
    std::cout << std::endl;
}

int main() {
    // Fast IO
    std::ios::sync_with_stdio(0);
    std::cin.tie(0);
    bool flag = true;
    // 불가능한 경우, 현재단계에서 pop 으로 나와야할 값이 현재까지 들어간 최대값보다 작으면서 마지막으로 pop 했던 숫자보다 큰 경우
    int n;
    std::cin >> n;
    for (int i = 0; i < n; i++) {
        std::cin >> arr[i];
    }
    
    int max = 0;
    int lastPop = -1;
    std::string no = "NO";
    for (int i = 0; i < n; i++) {
        if (arr[i] > max) {
            for (int j = max + 1; j <= arr[i]; j++) {
                Push(j);
            }
            max = std::max(max, arr[i]);
            lastPop = Pop();
        }
        else {
            if (lastPop != -1 && lastPop < arr[i]) {
                flag = false;
                break;
            }
            if (isEmpty() || Peek() != arr[i]) {
                flag = false;
                break;
            }
            lastPop = Pop();
        }
        //Debug();
        //std::cout << "arr[i]=" << arr[i] << "max=" << max << "lastPop=" << lastPop << std::endl;
    }

    if (!flag) std::cout << no;
    else std::cout << stream.str();

    return 0;
}
