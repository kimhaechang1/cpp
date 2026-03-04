#include <iostream>

class EmptyClass {
    // 아무것도 없음
};

class SimpleClass {
    int id;    // 4 bytes
    char type; // 1 byte
};

class MethodClass {
    int id;    // 4 bytes
    char type; // 1 byte

    // 일반 멤버 함수들 (100개가 있어도...)
    void Attack() {}
    void Defend() {}
    int GetId() { return id; }
};

class StaticClass {
    int id;             // 4 bytes
    static int count;   // static 변수 (객체 소속 아님!)
};
int StaticClass::count = 0;

int main() {
    std::cout << "[1] 비어있는 클래스: " << sizeof(EmptyClass) << " bytes\n";
    std::cout << "[2] 일반 변수만 있는 클래스: " << sizeof(SimpleClass) << " bytes\n";
    std::cout << "[3] 멤버 함수가 꽉 찬 클래스: " << sizeof(MethodClass) << " bytes\n";
    std::cout << "[4] static 변수가 포함된 클래스: " << sizeof(StaticClass) << " bytes\n";

    return 0;
}
