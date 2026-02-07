#include <iostream>
#include <string>

// [실습 환경 주의]
// 이 코드는 C++20 표준을 사용합니다.
// 컴파일 시 반드시 '-std=c++20' 옵션을 추가해야 합니다.
// 만약 <format> 헤더가 없다면, C++20을 지원하는 컴파일러로 업데이트가 필요합니다.

#if __has_include(<format>)
    #include <format>
#else
    // 포맷팅 라이브러리가 없을 경우를 위한 간단한 대체 매크로 (실습용)
    #include <cstdio>
    #define NO_FORMAT_LIB
#endif

using namespace std;

struct Player {
    string name;
    int hp;
    float mp;
};

int main() {
    // 1. Uniform Initialization 연습
    // 아래 변수들을 {}를 사용하여 초기화해보세요.
    
    // TODO: int types에 3.14를 넣어서 초기화해보세요. 컴파일 에러가 나야 정상입니다.
    // int val{3.14}; 
    
    int hp{ 100 };
    float mp{ 50.5f };
    string name{ "Hero" };

    // 구조체 초기화
    Player p{ "Knight", 200, 100.0f };

    cout << "=== Modern Initialization ===\n";
    cout << "Player Created.\n";

    // 2. std::format 연습
#ifndef NO_FORMAT_LIB
    // TODO: std::format을 사용하여 아래 문장을 만들어보세요.
    // 목표 출력: "[Knight] HP: 200, MP: 100.00"
    
    string status = std::format("[{}] HP: {}, MP: {:.2f}", p.name, p.hp, p.mp);
    cout << status << endl;
    
#else
    cout << "[System] <format> 헤더를 찾을 수 없어 printf로 대체합니다.\n";
    printf("[%s] HP: %d, MP: %.2f\n", p.name.c_str(), p.hp, p.mp);
#endif

    return 0;
}
