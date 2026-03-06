#include <iostream>
#include <cassert>

// [요구사항 1]
// 목적: 주어진 숫자 N의 팩토리얼(Factorial)을 컴파일 타임에 계산합니다.
// 함수 이름: GetFactorial
// - 입력과 반환값은 넉넉하게 큰 정수 타입을 사용하세요. (예: long long)
// - 힌트: 재귀호출 또는 단순 반복문(C++14 이상) 모두 constexpr 내부에서 사용 가능합니다.
// - 핵심: 반드시 컴파일 시점에 결과를 확정 지을 수 있도록 선언해야 합니다.
constexpr long long GetFactorial(long long val) {
    if (val == 1) {
        return val;
    }
    return val * GetFactorial(val - 1);
}


// [요구사항 2]
// 목적: 맵의 가로(Width)와 세로(Height) 길이를 받아 전체 면적(타일 개수)을 계산합니다.
// 함수 이름: GetTotalTileCount
// - 입력과 반환값 모두 정수 타입을 사용하세요.
// - 배열의 크기 지정 등에 쓸 수 있도록 컴파일 타임 계산이 보장되어야 합니다.
constexpr int GetTotalTileCount(int w, int h) {
    return w * h;
}


int main() {
    std::cout << "--- constexpr Practice Start ---\n";

    // 1. GetFactorial 검증
    // static_assert는 런타임(assert)과 달리 '컴파일 타임'에 조건을 검사합니다.
    // 만약 작성하신 GetFactorial이 일반 함수(런타임)였다면 이곳에서 당장 컴파일 에러가 발생합니다.
    // 이를 통과했다는 것은, 이 코드가 실행될 때 계산되는 것이 아니라 
    // 이미 120이라는 상수로 변환되어 .exe 파일에 박혀있음을 증명합니다!
    static_assert(GetFactorial(5) == 120, "Factorial calculation passed at Compile-Time!");
    static_assert(GetFactorial(10) == 3628800, "Factorial larger number check");
    std::cout << "[Pass] GetFactorial (Compile-Time Evaluated)\n";

    // 2. GetTotalTileCount 배열 크기 검증
    constexpr int width = 10;
    constexpr int height = 5;

    // C++에서 정적 배열(int arr[])의 크기는 반드시 '컴파일 타임 상수'로만 지정할 수 있습니다.
    // 일반 함수라면 에러가 나겠지만, constexpr 함수라면 완벽하게 동작합니다.
    int mapGrid[GetTotalTileCount(width, height)];

    // 배열의 크기가 정말로 50칸(10 * 5)이 맞는지 런타임에 확인합니다.
    assert(sizeof(mapGrid) / sizeof(int) == 50);
    std::cout << "[Pass] GetTotalTileCount array sizing\n";

    std::cout << "--- All Tests Passed! ---\n";
    std::cout << "Note: Game loading time saved! Logic completely solved by Compiler.\n";

    return 0;
}
