#include <iostream>
#include <string>
#include <vector>
#include <cassert> // 검증용
#include<functional>

using namespace std;

int GlobalVariable = 10;

// --------------------------------------------------------
// [Mission] 함수 포인터 실습 (TDD Style)
// --------------------------------------------------------
// 아래의 주석(TODO)에 따라 함수를 구현하고, 
// main() 함수의 테스트 코드를 통과(PASS)시키세요.
// --------------------------------------------------------

// [1] 사칙연산 함수들 구현
// TODO 1: 두 정수를 더해서 반환하는 Add 함수 구현
int Add(int a, int b) {
    return a + b; // 구현하세요
}

// TODO 2: 두 정수를 빼서 반환하는 Sub 함수 구현
int Sub(int a, int b) {
    return a - b; // 구현하세요
}

// TODO 3: 두 정수를 곱해서 반환하는 Mul 함수 구현
int Mul(int a, int b) {
    return a * b; // 구현하세요 
}


// [2] 계산기 함수 (함수 포인터를 매개변수로 받음)
// TODO 4: 마지막 인자로 "두 정수를 받아서 정수를 반환하는 함수 포인터"를 받는 Calculate 함수를 완성하세요.
// 힌트: int (*func)(int, int)
void Calculate(int a, int b, int (*f)(int, int)) {
    // TODO 5: 넘겨받은 함수 포인터(func)를 실행하여 결과를 출력하세요.
    // 출력 형식: "계산 결과: <값>"
    cout << "계산 결과: " << (*f)(a, b) << endl;
}

void Test(int a, int b, int* f()) {
    // 이거는 함수 포인터가 아니라 int* 형을 반환하는 함수 포인터 파라미터가 된다.
    // 컴파일러가 int* (*f)(int, int) 가 된다는것
    int* result = f();
    cout << "글로벌 변수에 a 와 b 를 더함: " << GlobalVariable + (a + b) << endl;
}


int* GetGlobal() {
    return &GlobalVariable;
}




void FunctionalCalc(int a, int b, function<int(int, int)> f) {
    cout << "TEST: " << f(a, b) << endl;
}

// --------------------------------------------------------
// [Test Code] - 변경하지 마세요!
// --------------------------------------------------------
int main() {
    cout << "=== Function Pointer Test Start ===" << endl;

    // 1. 함수 1차 검증
    if (Add(10, 20) == 30) cout << "[PASS] Add(10, 20) == 30" << endl;
    else cout << "[FAIL] Add(10, 20) != 30 (Actual: " << Add(10, 20) << ")" << endl;

    if (Sub(10, 20) == -10) cout << "[PASS] Sub(10, 20) == -10" << endl;
    else cout << "[FAIL] Sub(10, 20) != -10 (Actual: " << Sub(10, 20) << ")" << endl;


    // 2. 함수 포인터 변수 선언 및 사용 테스트
    cout << "\n[Test 2] Function Pointer Variable" << endl;
    int (*myFunc)(int, int) = nullptr;

    myFunc = Add;
    if (myFunc(3, 4) == 7) cout << "[PASS] (*myFunc)(3, 4) == 7 (Pointing Add)" << endl;
    else cout << "[FAIL] (*myFunc) failed" << endl;

    myFunc = Mul;
    if (myFunc(3, 4) == 12) cout << "[PASS] (*myFunc)(3, 4) == 12 (Pointing Mul)" << endl;
    else cout << "[FAIL] (*myFunc) failed" << endl;


    // 3. 매개변수로 전달 테스트 (Calculate 함수)
    cout << "\n[Test 3] Passing Function Pointer" << endl;
    // Calculate 함수가 완성이 안되면 컴파일 에러가 날 수 있으므로 주석 처리 해두었습니다.
    // TODO를 다 하신 후 주석을 풀고 실행해보세요.

    
    cout << "Expected: 30" << endl;
    Calculate(10, 20, Add);

    cout << "Expected: -10" << endl;
    Calculate(10, 20, Sub);

    FunctionalCalc(10, 20, Add);

    Test(10, 20, GetGlobal);
   
    return 0;
}
