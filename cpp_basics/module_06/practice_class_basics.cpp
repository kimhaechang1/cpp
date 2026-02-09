#include <iostream>
#include <format> // C++20

// -------------------------------------------------------------
// [Subtitle 1] Namespace & Class Basics
// -------------------------------------------------------------

// 1. 네임스페이스 정의
// 'Game'이라는 네임스페이스 안에 'Player' 클래스를 만드세요.
namespace Game {
    class Player {
    private:
        // 2. 캡슐화 (Encapsulation)
        // 이름(name), 체력(hp), 공격력(atk)을 private 변수로 선언하세요.
        // (Modern Init: int hp{100}; 스타일 사용)
        std::string name;
        int hp;
        int atk;

    public:
        // 3. Setter/Getter (인터페이스)
        // 외부에서 초기값을 넣을 수 있는 'Init(n, h, a)' 함수를 만드세요.
        void Init(std::string n, int h, int a) {
            name = n;
            // hp는 0보다 작아질 수 없도록 방어 코드(Guard Clause)를 넣어보세요.
            if (h < 0) h = 0;
            hp = h;
            atk = a;
        }

        // 정보를 예쁘게 출력하는 'Dump()' 함수를 만드세요. (std::format 활용)
        void Dump() {
            // 출력 예: "[Hero] HP: 100, ATK: 10"
            std::cout << std::format("[{}] HP: {}, ATK: {}\n", name, hp, atk);
        }
        
        // hp를 수정하는 함수는 만들지 마세요. (외부에서 임의 조작 금지)
        // 오직 Attack() 같은 행동 함수만 제공하는 것이 OOP의 핵심입니다.
    };
}

int main() {
    std::cout << "=== Module 06 Subtitle 1: Class Basics ===\n";

    // 1. 네임스페이스 사용
    // Game 네임스페이스 안에 있는 Player를 생성하세요.
    // Game::Player p;
    Game::Player p;

    // 2. 초기화 (Init 호출)
    // p.Init("Leon", 150, 20);
    p.Init("Leon", 150, 20);

    // 3. 접근 제한 테스트 (주석을 풀고 에러가 나는지 확인해보세요)
    // p.hp = 10000; // <- 여기서 에러가 나야 성공입니다! (private이므로)
    
    // 4. 정보 출력
    p.Dump();

    return 0;
}
