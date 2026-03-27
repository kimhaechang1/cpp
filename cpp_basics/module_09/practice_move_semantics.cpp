#include <iostream>
#include <vector>
#include <memory>
#include <cassert>
#include <algorithm>

// ==============================================================================
// [개념 실습] 이동 시맨틱 (Move Semantics) — practice_move_semantics.cpp
// 이 파일은 "복사"와 "이동"의 성능 및 소유권 차이를 이해하기 위한 훈련용입니다.
// ==============================================================================

class HugeBuffer {
public:
    int* data;
    size_t size;
    bool movedFrom = false; // 이동당했는지 여부 표시용 (테스트용)

    // 생성자
    HugeBuffer(size_t s) : size(s) {
        data = new int[size];
        std::fill_n(data, size, 1);
        std::cout << "[생성] " << size << " 크기의 버퍼 할당 완료.\n";
    }

    // 소멸자
    ~HugeBuffer() {
        if (data) {
            delete[] data;
            // std::cout << "[소멸] 버퍼 해제 완료.\n";
        }
    }

    // ---------------------------------------------------------
    // 연습문제 1: 이동 생성자 (Move Constructor) 구현
    // [요구사항]
    // 1. 상대방(other)의 포인터와 사이즈를 "가로채기" 하세요.
    // 2. 상대방의 포인터는 매너 있게 nullptr로 밀어버리세요. (자원 유실 방지)
    // 3. 상대방의 movedFrom 플래그를 true로 설정하세요. (테스트 검증용)
    // 💡 힌트: R-value 참조(&&)를 사용하고 noexcept를 붙이세요.
    // ---------------------------------------------------------

    // [이동 생성자 작성 영역]
    // TODO: 여기에 이동 생성자를 구현하세요.
    // HugeBuffer(HugeBuffer&& other) noexcept ...
    // 이동생성자, 이동 대입 연산자 오버로딩 등 이것들은 모두 대상의 원천에 가서 
    // 현재와 동일한 타입일때 받아가지고 대상이 가리키는 모든 값을 가져오는것이다.
    // 얼핏 보면 복사 생성자와 뭐가 다른거지 싶지만?
    // 특히 멤버변수가 동적할당일 때 큰 차이를 보인다.
    // 복사 생성자라면 서로 같은것을 가리키면 안되기 때문에 본인 내부 힙메모리 사용 변수에 대해서 크기를 할당하고 반복문을 통해 값을 옮긴다.
    // 하지만 이동시멘틱의 본질은 other 의 원천을 그대로 가져와서 본인것 처럼 사용하는것이다.
    // 따라서 포인터만 옮김으로서 복사생성자의 매번 재할당보다 효율적이게 된다.
    // 주의할것으로 복사생성자는 복사하고 other 에 대해서 재조정이 전혀 이뤄지지 못하도록 const T& 를 사용하는데,
    // 이동시멘틱은 우선 R-Value 처리를 위해 L-Value 의 경우 move() 를통해 R-Value 로 캐스팅한다.
    // R-value 의 경우 파라미터로 받을려면 && 처리를 해야한다.
    HugeBuffer(HugeBuffer&& other) noexcept: data(other.data), size(other.size){
        other.data = nullptr;
        other.movedFrom = true;
    }

    // 복사 생성자 (이미 구현됨 - 깊은 복사)
    HugeBuffer(const HugeBuffer& other) : size(other.size) {
        data = new int[size];
        std::copy(other.data, other.data + size, data);
        std::cout << "[복사] 깊은 복사 수행됨.\n";
    }

    // ---------------------------------------------------------
    // 연습문제 2: 이동 대입 연산자 (Move Assignment Operator) 구현
    // [요구사항]
    // 1. 이미 내게 할당된 자원이 있다면 delete[]로 먼저 정리하세요! (메모리 누수 방지)
    // 2. 상대방(other)의 자원을 가로채고, 상대방을 nullptr로 만듭니다.
    // 3. 자가 대입(this == &other) 상황을 체크하여 예외 처리를 하세요.
    // ---------------------------------------------------------

    // [이동 대입 연산자 작성 영역]
    // TODO: 여기에 이동 대입 연산자를 구현하세요.
    // HugeBuffer& operator=(HugeBuffer&& other) noexcept { ... }
    HugeBuffer& operator=(HugeBuffer&& other) noexcept {
        // [작성 위치]
        if (this == &other){
            return *this;
        }
        if (this->data != nullptr){
            delete[] this->data;
        }
        this->size = other.size;
        this->data = other.data;
        other.data = nullptr;
        other.size = 0;
        other.movedFrom = true;
        return *this; 
    }

    // 복사 대입 연산자 (본 실습에서는 생략 - 필요 시 delete 하거나 기본 사용)
    HugeBuffer& operator=(const HugeBuffer&) = delete;
};

// ---------------- C++ 테스트 코드 (Main) ----------------
// 여기 아래의 테스트(Main) 코드는 절대 수정하지 마세요!
// --------------------------------------------------------
int main() {
    std::cout << "========= 이동 시맨틱 훈련 시작 =========\n";

    // [테스트 1: 복사 vs 이동]
    {
        HugeBuffer source(1000);
        
        // 1. 복사 수행
        HugeBuffer copied = source; 
        assert(source.data != nullptr && "복사 후에도 원본은 살아있어야 합니다.");
        assert(copied.size == 1000 && "복사본은 같은 크기를 가져야 합니다.");
        
        // 2. 이동 수행 (std::move 호출)
        // [문구 유도]: "나 이제 이거 안 쓸 거야, 가져가!"
        // 여기서 주의할 점은 이동 대입 연산자가 호출되는거라고 착각하면 안된다.
        // 언제나 연산자의 두 피 연산자는 초기화가 완료된 변수에 대해서만 작동한다.
        // 여기서 moved 는 초기화가 된게 맞을까?
        // 아니다 컴파일러 입장에서는 moved 를 초기화 할때 source 의 원천을 갖고서 초기화 하라는것으로 해석한다.
        // 따라서 이동 생성자가 호출되는것
        // 복사 생성자가 호출되는 원리와 동일하다.
        HugeBuffer moved = std::move(source); 
        // 위 코드는 결국 진짜 이동 생성자가호출되는것 처럼 보일려면 HugeBuffer moved(std::move(source)) 와 같은 원리이다.

        // 검증
        assert(moved.data != nullptr && "이동 후 새 객체가 자원을 가져야 합니다.");
        assert(source.data == nullptr && "이동 후 원본의 포인터는 nullptr이어야 합니다.");
        assert(source.movedFrom == true && "이동 생성자가 정상 호출되어 플래그가 바뀌어야 합니다.");

        std::cout << "[PASS] 테스트 1: 복사 vs 이동 검증 완료\n";
    }

    std::cout << "\n";

    // [테스트 2: unique_ptr의 강제 이동]
    {
        auto p1 = std::make_unique<int>(42);
        
        // ❌ unique_ptr는 복사가 안 됨! 아래는 주석 해제 시 컴파일 에러.
        // auto p2 = p1; 

        // ✅ 오직 이동으로만 소유권 이전 가능
        auto p3 = std::move(p1);

        assert(p1 == nullptr && "이동 후 p1은 빈 상자가 되어야 합니다.");
        assert(*p3 == 42 && "이동 후 p3가 값을 소유해야 합니다.");
        
        std::cout << "[PASS] 테스트 2: unique_ptr 소유권 이전 완료\n";
    }

    // [테스트 3: 이동 대입 연산자 (Assignment) 호출 확인]
    {
        HugeBuffer target(100);
        HugeBuffer source(500);

        // 이미 존재하는 target에 대해 std::move를 통한 대입 시도
        // 위에서 이미 초기화 되지 않은 변수에 대한 대입은 생성자 호출이라고 했다.
        // 이번에 target 과 source 는 충분히 초기화 되어있다.
        // 따라서 피연산자가 둘다 초기화된 변수이기 때문에 이동 대입 연산자가 호출된다.
        target = std::move(source); 

        assert(target.size == 500 && "이동 대입 후 target의 크기가 바뀌어야 합니다.");
        assert(source.data == nullptr && "이동 대입 후 source는 nullptr이어야 합니다.");
        
        std::cout << "[PASS] 테스트 3: 이동 대입 연산자 호출 및 자원 이전 완료\n";
    }

    std::cout << "\n========= 모든 이동 시맨틱 훈련 완료! =========\n";
    return 0;
}
