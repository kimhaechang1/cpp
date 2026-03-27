#include <iostream>
#include <utility>

// ==============================================================================
// [실험] 이동 시맨틱의 내부 논리 및 호출 시점 — experiment_move_logic.cpp
// 이 파일은 "이동 생성자"와 "이동 대입 연산자"의 차이를 시각적으로 확인하기 위한 학습용 코드입니다.
// ==============================================================================

struct HugeBuffer {
    int* data;
    HugeBuffer(int size) { 
        data = new int[size]; 
        std::cout << "[생성] " << size << " 크기 할당\n";
    }
    ~HugeBuffer() { 
        if (data) {
            delete data; 
            std::cout << "[소멸] 할당 해제\n";
        } else {
            std::cout << "[소멸] (Hollow Shell - 아무것도 하지 않음)\n";
        }
    }
    
    // 1. 이동 생성자 (Initialization 시 호출)
    // - 새로운 객체가 생성될 때, 다른 rvalue로부터 자원을 '강탈'해옵니다.
    HugeBuffer(HugeBuffer&& other) noexcept {
        std::cout << ">>> [이동 생성자] 호출! (자원을 뺏어와서 새로 생성)\n";
        this->data = other.data;
        other.data = nullptr; // 원본을 무력화 (폭탄 제거)
    }

    // 2. 이동 대입 연산자 (이미 만들어진 객체에 대입 시 호출)
    // - 이미 존재하는 객체에 다른 rvalue의 자원을 '덮어씌울' 때 호출됩니다.
    HugeBuffer& operator=(HugeBuffer&& other) noexcept {
        std::cout << ">>> [이동 대입 연산자] 호출! (기존 것 버리고 뺏어옴)\n";
        if (this != &other) { // 자가 대입 방지
            delete data;      // 1. 내꺼 먼저 비우고
            this->data = other.data; // 2. 상대꺼 뺏어옴
            other.data = nullptr;    // 3. 상대 무력화
        }
        return *this;
    }

    // 복사 생성자 (비교를 위해 복사는 무겁게 처리한다고 가정)
    HugeBuffer(const HugeBuffer& other) {
        std::cout << "[복사 생성자] 호출! (무거운 깊은 복사 수행)\n";
        // 실제 복사 로직은 생략
    }
};

int main() {
    std::cout << "========= 이동 시맨틱 내부 논리 실험 시작 =========\n\n";

    std::cout << "[Step 1] b1 생성\n";
    HugeBuffer b1(100);

    std::cout << "\n[Step 2] b2 선언과 동시에 b1을 이동 (초기화)\n";
    HugeBuffer b2 = std::move(b1); // ✅ 이동 생성자 호출!

    std::cout << "\n[Step 3] 이미 존재하는 b3에 b2를 이동 (대입)\n";
    HugeBuffer b3(200);
    b3 = std::move(b2);             // ✅ 이동 대입 연산자 호출!

    std::cout << "\n[Step 4] 함수 종료 직전 (소멸자 호출 대기)\n";
    return 0;
}
