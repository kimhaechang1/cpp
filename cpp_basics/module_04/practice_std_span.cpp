#include <iostream>
#include <vector>
#include <span>  // C++20 필수 헤더
#include <format> // C++20 (없으면 printf 등으로 대체 가능)

using namespace std;
// [실습 환경 주의]
// 이 코드는 C++20 표준 컴파일러가 필요합니다. (g++ -std=c++20)

// -------------------------------------------------------------
// [Old Way] 포인터와 길이를 따로 받음 (지겹다 무섭다)
void LegacyPrint(int* arr, int size) {
    std::cout << "[Legacy] ";
    for (int i = 0; i < size; ++i) {
        std::cout << arr[i] << " "; // arr[i] 접근 시 범위 체크 안 함 (위험)
    }
    std::cout << "\n";
}

// -------------------------------------------------------------
// [New Way] std::span 사용 (깔끔하다 안전하다)
// TODO 1: 매개변수로 std::span<int>를 받도록 수정하세요.
// 읽기 전용으로 생각하면 안되고, 메모리위에 연속데이터를 특정길이만큼 바라보고있는 존재
void ModernPrint(span<int> data) {
    std::cout << "[Modern] ";

    // TODO 2: span은 범위 기반 for문을 지원합니다.
    for (int n : data) {
        std::cout << n << " ";
    }
    std::cout << "(Size: " << data.size() << ")\n";
}

// -------------------------------------------------------------
// [Advanced] 값을 수정해보자
// span은 'View'이므로 const가 아니면 원본 수정이 가능합니다.
void DoubleValues(std::span<int> data) {
    for (int& n : data) {
        n *= 2;
    }
}

int main() {
    std::cout << "=== Module 04 Supplement: std::span ===\n";

    // 1. C 스타일 배열
    int rawArr[] = { 10, 20, 30, 40, 50 };

    // 2. std::vector
    std::vector<int> vec = { 111, 222, 333 };

    // Legacy 호출
    LegacyPrint(rawArr, 5);

    // Modern 호출 (자동 변환!)
    std::cout << "\n>> Using std::span with Array:\n";
    ModernPrint(rawArr); // 배열 -> span 자동 변환

    std::cout << ">> Using std::span with Vector:\n";
    ModernPrint(vec);    // 벡터 -> span 자동 변환

    // 3. Subspan (부분 자르기)
    std::cout << "\n>> Subspan (Slicing without copy):\n";
    // TODO 3: rawArr의 인덱스 1부터 3개만 잘라서(View) 전달해보세요.
    // 힌트: std::span<int> view_all(rawArr);
    //       view_all.subspan(start_index, count);
    span<int> view_all(rawArr);
    span<int> subspan = view_all.subspan(1, 3);

    std::span<int> fullView(rawArr);
    std::span<int> partView = fullView.subspan(1, 3); // {20, 30, 40}

    ModernPrint(partView);

    // 4. 값 수정 테스트
    std::cout << "\n>> Double Values of Subspan:\n";
    DoubleValues(partView); // 20, 30, 40 -> 40, 60, 80 (원본 rawArr가 바뀜!)

    ModernPrint(rawArr); // 원본 확인

    return 0;
}
