#include <iostream>
#include <vector>
#include <array>
#include <format> // C++20

// -------------------------------------------------------------
// [메모리 주소의 진실]
// Stack 메모리와 Heap 메모리는 주소 영역대가 완전히 다릅니다.
// 눈으로 직접 확인해봅시다.
// -------------------------------------------------------------

// 0. 전역 변수 (Global/Data 영역)
// 프로그램 시작부터 끝까지 살아있는 친구들입니다.
std::array<int, 5> globalArray = {10, 20, 30, 40, 50};
int globalVar = 999;

int main() {
    std::cout << "=== Memory Layout Verification (Stack vs Heap vs Data) ===\n\n";

    // 1. 순수 스택(Stack) 변수 (Local Variable)
    // 함수 안에서 만들어진 변수들은 '스택 프레임' 안에 들어갑니다.
    int stackVar = 10;
    
    // 2. std::array (Local - 스택)
    std::array<int, 5> stackArray = {1, 2, 3, 4, 5};

    // 3. std::vector (Heap 사용)
    std::vector<int> heapVector = {1, 2, 3, 4, 5};

    // 4. 순수 힙(Heap) 변수
    int* pureHeapVar = new int(20);

    // -------------------------------------------------------------
    // 주소 출력 (Address Printing)
    // -------------------------------------------------------------
    
    std::cout << "[Data Segment (Global/Static)]\n";
    std::cout << "Global Array Obj:     " << &globalArray << " (Low Address)\n";
    std::cout << "Global Var:           " << &globalVar << "\n\n";

    std::cout << "[Heap Region (Dynamic)]\n";
    std::cout << "std::vector Data:     " << heapVector.data() << " (Remote Control Target)\n";
    std::cout << "Pure Heap Variable:   " << pureHeapVar << "\n\n";

    std::cout << "[Stack Region (Local)]\n";
    std::cout << "Stack Var Address:    " << &stackVar << " (High Address)\n";
    std::cout << "std::array Address:   " << &stackArray << " (Inside Stack Frame)\n";
    std::cout << "std::vector Object:   " << &heapVector << " (Shell is on Stack)\n";

    // -------------------------------------------------------------
    // 거리 계산 (Address Distance)
    // -------------------------------------------------------------
    // 스택 변수와 벡터 데이터 사이의 거리가 얼마나 먼지 확인해봅시다.
    long long stackAddr = (long long)&stackVar;
    long long vectorDataAddr = (long long)heapVector.data();
    
    std::cout << "\n[Distance Analysis]\n";
    std::cout << "Diff (Stack - Heap):  " << std::abs(stackAddr - vectorDataAddr) << " bytes\n";
    std::cout << ">> 주소 차이가 어마어마하게 크죠? 완전히 다른 동네입니다.\n";

    delete pureHeapVar;
    return 0;
}
