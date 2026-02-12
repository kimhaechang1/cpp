#include <iostream>
#include <format>

// -------------------------------------------------------------
// [Subtitle 3] Copy Constructor (Deep Copy vs Shallow Copy)
// -------------------------------------------------------------

class Pet {
public:
    int* hp; // 포인터 변수 (동적 할당된 메모리를 가리킴)

    // 생성자
    Pet() {
        std::cout << "[Pet] 생성됨 (Resource Alocated)\n";
        hp = new int(100); // 힙 메모리에 100 저장
    }

    // 소멸자
    ~Pet() {
        std::cout << "[Pet] 소멸됨 (Resource Released)\n";
        if (hp != nullptr) {
            delete hp; // 메모리 해제
            hp = nullptr;
        }
    }

    // [TODO] 복사 생성자 (Deep Copy) 구현하기
    // 이 부분을 주석 처리하고 실행하면 -> 얕은 복사로 인한 [Double Free] 크래시 발생!
    // 이 부분을 구현하면 -> 깊은 복사로 안전하게 실행됨.
    Pet(const Pet& other) {
        std::cout << "[Pet] 복사 생성자 호출 (Deep Copy!)\n";
        
        // 1. 내 몫의 새로운 메모리 할당 (new)
        // hp = ...
        hp = new int;

        // 2. 원본(other)의 '값'을 가져와서 내 메모리에 넣기
        // *hp = ...
        *hp = *other.hp;
    }

    void Dump() {
        std::cout << std::format("Pet Addr: {}, HP Addr: {}, HP Value: {}\n", 
            (void*)this, (void*)hp, *hp);
    }
};

int main() {
    std::cout << "=== Copy Constructor Test ===\n";

    {
        Pet p1;
        p1.Dump();

        std::cout << "--- p2 = p1 (Copy) ---\n";
        
        // 여기서 복사 생성자가 호출됩니다.
        // 만약 복사 생성자가 없다면? -> 얕은 복사(Shallow Copy) -> p1.hp와 p2.hp가 같은 주소!
        Pet p2 = p1; 
        
        p2.Dump(); // 주소를 확인해보세요.
        
    } // 블록 끝 -> p2 소멸(delete), 그 다음 p1 소멸(delete)
      // 만약 얕은 복사라면 여기서 💣 펑! (Double Free)

    std::cout << "=== Program End (Alive?) ===\n";
    return 0;
}
