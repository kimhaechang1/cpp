#include <iostream>
#include <format> // C++20
#include <string>

// -------------------------------------------------------------
// [Subtitle 1] Namespace & Class Basics
// -------------------------------------------------------------

namespace Game {
    class Player {
        std::string name;
        int hp;
        int atk;

    public:
        void Init(std::string n, int h, int a) {
            name = n;
            hp = h;
            atk = a;
        }

        void Dump();

       
    };
}

void Game::Player::Dump() {
    std::cout << "[" << name << "] HP: " << hp << ", ATK: " << atk;
}
// 1. 네임스페이스 정의
// 'Game'이라는 네임스페이스 안에 'Player' 클래스를 만드세요.
// namespace Game { ... }

    // class Player { ... }

    // 2. 캡슐화 (Encapsulation)
    // - private: 외부에서 접근 불가 (이름, 체력, 공격력)
    // - public: 외부에서 접근 가능 (Init, Dump)

        // [멤버 변수]
        // std::string name;
        // int hp;
        // int atk;

        // [멤버 함수]
        // void Init(std::string n, int h, int a);
        // void Dump(); -> "[Hero] HP: 100, ATK: 10" 형식으로 출력

// }; // namespace 끝

int main() {
    std::cout << "=== Module 06 Subtitle 1: Class Basics ===\n";

    // 1. 네임스페이스 사용
    // Game::Player p;
    Game::Player p;

    // 2. 초기화 (Init 호출)
    // p.Init("Leon", 150, 20);
    p.Init("Leon", 150, 20);

    // 3. 접근 제한 테스트 (주석을 풀고 에러가 나는지 확인해보세요)
    // p.hp = 10000; // <- 여기서 에러가 나야 성공입니다! (private이므로)
    //p.hp = 10000;

    // 4. 정보 출력
     p.Dump();

    return 0;
}
