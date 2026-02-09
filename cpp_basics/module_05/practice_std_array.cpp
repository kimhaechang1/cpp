#include <iostream>
#include <array>    // 필수 헤더
#include <algorithm> // std::sort 사용 위함 (<vector> 없어도 됨!)

// -------------------------------------------------------------
// [Modern Way] std::array는 함수로 넘길 때도 크기를 '템플릿'으로 받을 수 있음
// (std::span처럼 유연하진 않지만, 크기가 고정된 경우 매우 강력함)
template <size_t N>
void PrintArray(const std::array<int, N>& arr) {
    std::cout << "[Array View] Size(" << arr.size() << "): ";
    for (int n : arr) {
        std::cout << n << " ";
    }
    std::cout << "\n";
}

int main() {
    std::cout << "=== Module 05 Supplement: std::array ===\n";

    // 1. 선언 및 초기화
    // TODO 1: 크기 5인 int형 std::array 'buf'를 선언하고, {10, 50, 20, 40, 30}으로 초기화하세요.
    // 힌트: std::array<int, 5> ...

    // std::array<int, 5> buf = ...;
    std::array<int, 5> buf = { 10, 50, 20, 40, 30 };


    // 2. 대입 연산 (C 배열은 이게 안 됐음!)
    std::cout << "\n>> Copy Test:\n";
    // TODO 2: buf와 똑같은 타입의 'bufClone'을 만들고, buf를 대입(=)하세요.
    std::array<int, 5> bufClone = buf;

    // std::array<int, 5> bufClone;
    // bufClone = buf;

    // PrintArray(bufClone); 
    PrintArray(bufClone);


    // 3. STL 알고리즘 사용 (정렬)
    std::cout << "\n>> Sort Test:\n";
    // C 배열은 std::sort(arr, arr+5) 이렇게 포인터로 줬어야 했지만...
    // std::array는 begin(), end()를 멤버로 가집니다!

    // TODO 3: std::sort와 .begin(), .end()를 사용하여 buf를 정렬하세요.

    // std::sort(...);
    std::sort(buf.begin(), buf.end());

    // PrintArray(buf);
    PrintArray(buf);


    // 4. 안전한 접근 (.at)
    std::cout << "\n>> Safety Test:\n";
    try {
        // TODO 4: buf.at(10) 에 접근해서 값을 999로 바꿔보세요. (범위 초과 시도)
        // buf.at(10) = 999;
        buf.at(10) = 999;
    }
    catch (const std::exception& e) {
        std::cout << "Caught Exception: " << e.what() << "\n";
        std::cout << "(std::array::at prevents Buffer Overflow!)\n";
    }

    return 0;
}
