#include <iostream>
#include <format> // C++20

// 1. 매개변수로 받으면 배열은 포인터로 '퇴화(Decay)'합니다.
// void Test(int arr[]) 라고 써도 사실 컴파일러는 void Test(int* arr) 로 번역합니다.
void FunctionScope(int arr[]) {
    // 여기서 arr은 그냥 포인터(주소값)일 뿐입니다.
    // 64비트 시스템에서 포인터의 크기는 항상 8바이트입니다.
    // 배열 전체 크기(20바이트)가 아닙니다!
    std::cout << "[Function] sizeof(arr): " << sizeof(arr) << " bytes (Warning: It's a pointer!)\n";

    // ❌ 컴파일 에러!
    // 컴파일러: "arr가 가리키는 건 알겠는데, 어디까지 읽어야 해?"
    // for (int n : arr) { ... } 
}

// 2. 템플릿을 쓰면 배열의 크기(N)를 강제로 같이 넘길 수 있습니다. (고급 기법)
// 하지만 함수가 길이에 따라 따로 만들어지는 단점이 있습니다.
template <size_t N>
void TemplateScope(int (&arr)[N]) {
    std::cout << "[Template] Size N detected: " << N << "\n";
    // 이건 가능함 (N을 아니까)
    for (int n : arr) {
        std::cout << n << " ";
    }
    std::cout << "\n";
}

int main() {
    int myArr[5] = { 1, 2, 3, 4, 5 };

    std::cout << "=== Array Decay Test ===\n";

    // 1. 메인 스코프 (배열이 선언된 곳)
    // 컴파일러가 선언을 보고 크기를 암.
    std::cout << "[Main] sizeof(myArr): " << sizeof(myArr) << " bytes (Correct!)\n";
    
    // ✅ 가능 (범위 기반 for문)
    for (int n : myArr) {
        std::cout << n << " ";
    }
    std::cout << "\n\n";

    // 2. 함수로 전달 (포인터로 퇴화)
    FunctionScope(myArr);

    // 3. 템플릿 사용 (배열 참조 유지)
    // TemplateScope(myArr); 

    std::cout << "\n>> 결론: 함수로 넘기면 길이 정보가 사라지기 때문에, 길이를 따로 주거나 std::span을 써야 합니다.\n";

    return 0;
}
