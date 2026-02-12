#include <iostream>
#include <string>
#include <format>

// -------------------------------------------------------------
// [Subtitle 2] Constructor & Destructor
// -------------------------------------------------------------

class Knight {
private:
    std::string name;
    int hp;

public:
    // 1. 기본 생성자 (Default Constructor)
    // - 매개변수 없이 'Knight k;' 라고만 썼을 때 호출됨
    // - name은 "Unknown", hp는 50으로 초기화
    // - "기사 탄생!" 출력
    Knight() {
        // TODO: 구현하세요.
        name = "Unknown";
        hp = 50;
    }

    // 2. 매개변수 생성자 (Parameterized Constructor)
    // - 'Knight k("Arthur", 100);' 라고 썼을 때 호출됨
    // - 초기화 리스트(:)를 사용하여 초기화해보세요. (권장)
    // - "전설의 기사 [name] 탄생!" 출력
    Knight(std::string n, int h)
        : name(n), hp(h)
        /* : 여기에 초기화 리스트 작성 */
    {   
        // TODO: 구현하세요.
        std::cout << "전설의 기사 [" << name << "] 탄생!" << std::endl;
    }

    // 3. 소멸자 (Destructor)
    // - 객체가 사라질 때 호출됨
    // - "[name] 기사가 쓰러졌습니다..." 출력
    ~Knight() {
        // TODO: 구현하세요.
        std::cout << std::format("[{}] 기사가 쓰러졌습니다...", name) << std::endl;
    }

    void Dump() {
        std::cout << std::format("[Info] Name: {}, HP: {}\n", name, hp);
    }
};

int main() {
    std::cout << "=== Lifecycle Test ===\n";

    // Scope 1: 기본 생성자 테스트
    {
        std::cout << "--- Block 1 Start ---\n";
        Knight k1; // 여기서 기본 생성자 호출
        k1.Dump();
        std::cout << "--- Block 1 End ---\n";
    } // 여기서 k1 소멸자 호출

    std::cout << "\n";

    // Scope 2: 매개변수 생성자 테스트
    {
        std::cout << "--- Block 2 Start ---\n";
        Knight k2("Lancelot", 200); // 매개변수 생성자 호출
        k2.Dump();
        std::cout << "--- Block 2 End ---\n";
    } // 여기서 k2 소멸자 호출

    std::cout << "\n=== Program End ===\n";
    return 0;
}
