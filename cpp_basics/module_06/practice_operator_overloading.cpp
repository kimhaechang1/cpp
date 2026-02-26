#include <iostream>

using namespace std;

class Inventory {
private:
    int* items;
    int capacity;

public:
    // 1. 기본 생성자
    Inventory(int cap = 5) : capacity(cap) {
        items = new int[capacity];
        for (int i = 0; i < capacity; i++) {
            items[i] = 0; // 초기화
        }
        cout << "[기본 생성자] 호출! (용량: " << capacity << ")\n";
    }

    // 2. 소멸자 (메모리 해제)
    ~Inventory() {
        if (items != nullptr) {
            delete[] items;
        }
        cout << "[소멸자] 호출!\n";
    }

    // 3. 복사 생성자 (깊은 복사)
    Inventory(const Inventory& other) {
        this->capacity = other.capacity;
        this->items = new int[this->capacity];
        for (int i = 0; i < this->capacity; i++) {
            this->items[i] = other.items[i];
        }
        cout << "[복사 생성자] 호출! (깊은 복사 진행)\n";
    }

    // TODO: 4. 대입 연산자 (Copy Assignment Operator) 오버로딩 구현하기
    // 힌트: 자신 대입 방지 -> 기존 메모리 해제 -> 깊은 복사 -> *this 반환
    Inventory& operator=(const Inventory& other) {
        // other 는 참조자이다.
        // 역 참조자를 통해 되돌아가서 이를 참조하고있는 대상을 가져오면 그것이 주소가 된다.
        if (&other == this) {
            return *this;
        }

        // 자기자신에 남아있는 동적할당들을 해제해야한다.
        if (this->items != nullptr) {
            delete[] this->items;
        }

        // 값을 그대로 복사한다.
        this->capacity = other.capacity;
        this->items = new int[this->capacity];
        for (int i = 0; i < this->capacity; i++) {
            this->items[i] = other.items[i];
        }

        return *this;
    }


    // 테스트용 아이템 추가 기능
    void AddItem(int index, int itemCode) {
        if (index >= 0 && index < capacity) {
            items[index] = itemCode;
        }
    }

    void PrintItems() const {
        cout << "아이템 목록: ";
        for (int i = 0; i < capacity; i++) {
            cout << items[i] << " ";
        }
        cout << " (주소: " << &items[0] << ")\n"; // 힙 메모리 주소 확인
    }
};

int main() {
    cout << "--- 1. 객체 생성 ---\n";
    Inventory playerA(3); // 용량 3짜리 가방
    playerA.AddItem(0, 100); // 100번 아이템(예: 집행검) 넣음

    Inventory playerB(3); // 용량 3짜리 가방
    playerB.AddItem(0, 200); // 200번 아이템(예: 몽둥이) 넣음

    cout << "\n[A의 정보]\n";
    playerA.PrintItems();
    cout << "\n[B의 정보]\n";
    playerB.PrintItems();

    cout << "\n--- 2. 대입 연산 수행 (playerB = playerA;) ---\n";
    // 아직 대입 연산자를 만들지 않았지만 작동은 합니다. (얕은 복사 발생)
    // 어떻게 터지는지 보시거나, TODO를 먼저 완성하고 돌려보세요!
    playerB = playerA;

    cout << "\n[대입 후 A의 정보]\n";
    playerA.PrintItems();
    cout << "\n[대입 후 B의 정보]\n";
    playerB.PrintItems();

    cout << "\n--- 3. 셀프 대입 테스트 (playerA = playerA;) ---\n";
    playerA = playerA;

    cout << "\n--- 프로그램 종료 (소멸자 타임) ---\n";
    // TODO를 구현하지 않고 여기까지 오면, 이제 Double Free 에러가 발생합니다!
    return 0;
}
