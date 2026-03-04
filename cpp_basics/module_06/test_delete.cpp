#include <iostream>

class Slime {
public:
    Slime() { std::cout << "태어남\n"; }
    ~Slime() { std::cout << "죽음\n"; }
};

int main() {
    Slime slime;
    
    // 컴파일러 자체에서 막는 경우: 객체 자체를 delete 하려고 할 때
    // delete slime; // ERROR: type 'class Slime' argument given to 'delete', expected pointer

    // 문법은 맞췄지만(주소를 넘김), 실행 중(Runtime) 지옥도가 열리는 경우
    std::cout << "스택 메모리를 강제로 지워보겠습니다...\n";
    Slime* pSlime = &slime;
    delete pSlime; // 🚨 무시무시한 런타임 크래시 발생!

    return 0;
}
