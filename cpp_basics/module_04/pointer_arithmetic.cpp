#include <iostream>

using namespace std;

// --------------------------------------------------------
// [Mission] 포인터 연산과 배열 매개변수 실습
// --------------------------------------------------------

// TODO 1: 매개변수를 '배열 형태(int arr[])'로 받아보세요.
// (실제로는 int* arr로 처리된다는 것을 기억하세요!)
void PrintArray(int arr[], int size) {
    cout << "--- PrintArray 함수 시작 ---" << endl;

    // TODO 2: 배열 표기법([])을 사용해서 출력해보세요.
    for (int i = 0; i < size; ++i) {
        // [i]를 사용해서 값을 출력
        cout << arr[i] << " ";
    }
    cout << endl;
    // TODO 3: 포인터 크기 확인
    // sizeof(arr)을 출력해서 배열 전체 크기인지, 포인터 크기인지 확인해보세요.
    cout << "함수 안에서 sizeof(arr): " << sizeof(arr) << endl;

    cout << "---------------------------" << endl;
}

int main() {
    // 1. 배열 선언
    int monsterHps[5] = { 100, 200, 300, 400, 500 };

    // 2. 포인터에 배열 대입
    int* p = monsterHps;

    cout << "main함수에서 sizeof(monsterHps): " << sizeof(monsterHps) << endl;
    cout << "main함수에서 sizeof(p): " << sizeof(p) << endl;

    // 3. 포인터 연산 테스트
    cout << "\n[포인터 연산 테스트]" << endl;
    // TODO 4: *p를 출력하면 뭐가 나올까요?
    // 100 이 나올거야
    cout << "*p: " << *p << endl;

    // TODO 5: *(p + 1)을 출력하면 뭐가 나올까요?
    // 200이 나올거야
    cout << "*(p + 1): " << *(p + 1) << endl;


    // 4. 함수 호출 테스트
    cout << "\n[함수 호출 테스트]" << endl;
    // 질문하신 내용: "포인터 p를 int arr[] 매개변수에 넘길 수 있나요?"
    // 정답: 네, 가능합니다! 직접 해보세요.
    PrintArray(p, 5);

    return 0;
}
