#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>
#include <iostream>

// 디버그 모드에서 메모리 누수 정보를 출력하기 위한 설정
#ifdef _DEBUG
#define new new( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#endif

struct Player {
    int hp;
    int mp;
};

int main() {
    // 프로그램 종료 시 메모리 누수 정보를 출력하도록 설정 (Visual Studio 환경)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    std::cout << "=== 동적 할당 실습 ===\n";

    // --------------------------------------------------------
    // [실습 1] 기본 변수 동적 할당
    // --------------------------------------------------------
    std::cout << "[1] int형 변수 하나를 동적 할당하고 값을 대입해보세요.\n";
    // TODO: int* p = ...
    int* p = new int;

    // TODO: 값 대입 및 출력
    *p = 10;
    std::cout << *p << std::endl;

    // TODO: 해제 (delete)
    delete p;

    // --------------------------------------------------------
    // [실습 2] 동적 배열 할당
    // --------------------------------------------------------
    int size;
    std::cout << "\n[2] 배열의 크기를 입력하세요: ";
    std::cin >> size;

    // TODO: 입력받은 크기만큼의 int 배열을 동적 할당하세요.
    int* arr = new int[size];
    // int arr[] << 이거는 사이즈가 정해져있어야함 변수가 아니어야함 ㅇㅇ

    // TODO: 배열의 각 요소에 10, 20, 30... 순서대로 값을 채우고 출력하세요.
    arr[0] = 10;
    arr[1] = 20;
    arr[2] = 30;
    for (int i = 0; i < size; i++) {
        std::cout << "arr["<<i<<"]="<< arr[i] << std::endl;
    }

    // TODO: 다 쓴 배열을 해제하세요.
    delete[] arr;


    // --------------------------------------------------------
    // [실습 3] 구조체 동적 할당 (응용)
    // --------------------------------------------------------
    std::cout << "\n[3] Player 구조체를 동적 할당하고 초기화해보세요.\n";
    // TODO: Player* myPlayer = ... 
    Player* myPlayer = new Player{};

    // TODO: 화살표 연산자(->)를 사용하여 hp와 mp를 설정하세요.
    myPlayer->hp = 10;
    myPlayer->mp = 20;

    // TODO: 해제
    delete myPlayer;
    std::cout << "\n실습 종료! (Output 창에서 메모리 누수 여부를 확인하세요)\n";

    // 의도적인 메모리 누수 테스트 (주석을 풀고 실행해보세요)
     int* leak = new int[5];

    return 0;
}
