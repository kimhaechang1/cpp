#include <iostream>

using namespace std;

// --------------------------------------------------------
// [Mission] 포인터의 기본 동작 확인하기
// --------------------------------------------------------

int main() {
    // 1. 일반 변수 선언
    int hp = 100;

    // TODO 1: hp의 '주소'를 출력해보세요. (& 연산자 사용)
    cout << "hp의 주소: " << &hp << endl;


    // 2. 포인터 변수 선언
    // TODO 2: hp의 주소를 저장하는 포인터 변수 ptr을 선언하세요.

    int* ptr = &hp;


    // TODO 3: ptr이 가지고 있는 값(주소)을 출력해보세요. (위의 hp 주소와 같은지 확인)
    cout << "ptr의 값: " << ptr << endl;


    // 3. 역참조 (Dereference) - 읽기
    // TODO 4: ptr을 이용해서 hp의 값을 읽어와 출력하세요. (* 연산자 사용)
    cout << "ptr이 가리키는 값: " << *ptr << endl;


    // 4. 역참조 (Dereference) - 쓰기
    // TODO 5: ptr을 이용해서 hp의 값을 50으로 변경하세요.

    *ptr = 50;


    cout << "변경 후 hp 값: " << hp << endl; // 50이 나와야 성공!

    return 0;
}
