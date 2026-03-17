#include <iostream>
#include <stack>
#include <vector>
#include <string>
#include <cassert>

// -------------------------------------------------------------
// [Algorithm 0x05] STL Stack Practice
// 목표: <stack> 헤더의 std::stack 사용법을 익힙니다.
// -------------------------------------------------------------

void PracticeBasic() {
    std::cout << "1. Basic Int Stack Practice" << std::endl;

    // TODO 1: int형 데이터를 담을 스택 's'를 선언하세요.
    std::stack<int> s;


    // TODO 2: 스택에 10, 20, 30을 순서대로 넣으세요(push).
    s.push(10); s.push(20); s.push(30);


    // TODO 3: 스택이 비어있는지 확인하고(empty), 비어있지 않다면 가장 위의 값(top)을 출력하세요.
    // 예상 출력: 30
    if (!s.empty()) {
        std::cout << s.top() << std::endl;
    }

    // TODO 4: 스택의 크기(size)를 출력하세요.
    // 예상 출력: 3
    std::cout << s.size() << std::endl;


    // TODO 5: 스택에 있는 모든 원소를 꺼내면서(pop) 출력하세요.
    // 출력 순서: 30 20 10
    std::cout << "Popping: ";
    while (!s.empty()) {
        // TODO: top 출력 -> pop
        std::cout << s.top() << std::endl;
        s.pop();
    }
    std::cout << std::endl;
}

void PracticeString() {
    std::cout << "\n2. String Stack Practice (Reverse Print)" << std::endl;
    std::vector<std::string> words = { "Hello", "C++", "World", "Stack" };

    // TODO 6: string을 담을 스택 'strStack'을 선언하고, words의 단어들을 순서대로 push 하세요.
    std::stack<std::string> strStack;
    for (auto& word : words) {
        strStack.push(word);
    }

    // TODO 7: 스택을 이용하여 단어들을 역순으로 출력하세요.
    // 예상 출력: Stack World C++ Hello
    std::cout << "Reverse: ";
    while (!strStack.empty()) {
        std::cout << strStack.top() << " ";
        strStack.pop();
    }

    std::cout << std::endl;
}

int main() {
    PracticeBasic();
    PracticeString();
    return 0;
}
