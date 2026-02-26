#include <iostream>
#include <cassert>
#include <span> 

using namespace std;

// =====================================================================
// [누적 연습문제 요구사항] - 여기서부터 스스로 모든 것을 작성해 보세요!
// 백지 상태(Blank Slate)에서 시작합니다!
// =====================================================================
// 
// 1. [Module 1] 아이템 등급을 나타내는 열거형 클래스(enum class) Rarity 를 만드세요.
//    - 값: Common, Rare, Epic
//
// 2. [Module 1/6] 구조체(struct) Item 을 만드세요.
//    - 멤버: int id, Rarity rarity 
//    - 반드시 Uniform Initialization {} 을 사용하여 기본값(0, Common)을 초기화하세요.
//
// 3. [Module 6] 'GameSystem' 이라는 네임스페이스(namespace)를 만드세요.
//
// 4. [Module 6] GameSystem 네임스페이스 안에 'PlayerInventory' 클래스를 만드세요.
//    - [멤버 변수 (private)]: 
//      * 복수개의 아이템을 제어할 동적 할당 변수 (이름: items)
//      * 가방이 담을 수 있는 최대 아이템 개수 (이름: capacity)
//      * 현재 가방에 들어있는 실제 아이템 개수 (이름: count)
//
//    - [멤버 함수 (public)]: (선언부 작성 훈련을 위해 스스로 타입과 인자를 고민해보세요!)
//      * (1) 기본 생성자: 기초 초기화 및 동적 메모리 할당 용도
//      * (2) 소멸자: 힙 메모리 반납 용도
//      * (3) 복사 생성자: 깊은 복사가 발생하도록 하는 용도
//      * (4) 대입 연산자 (Copy Assignment): 연쇄 대입과 깊은 복사, 메모리 누수를 고려하여 대입 연산을 오버로딩 하는 용도
//      * (5) 아이템 추가 함수: 배열 맨 끝에 새로운 ID와 등급을 추가 (이름: AddItem)
//      * (6) 아이템 뷰어 반환 함수: Module 4에서 배운 안전한 메모리 뷰 기법을 사용하여 배열의 뷰를 밖으로 전달하는 함수 (이름: GetItemsView)
//      * (7) 현재 저장된 개수 반환 함수 (이름: GetCount)
//      * (8) 최대 용량 반환 함수 (이름: GetCapacity)
//
// =====================================================================

// 여기에 코드를 쭉 작성해주시면 됩니다!
// 화이팅!
enum Rarity {
    Common,
    Rare,
    Epic
};

struct Item {
    int id;
    Rarity rarity;

    Item() {
        id = 0;
        rarity = Rarity::Common;
    }

    Item(int i, Rarity r) : id(i), rarity(r) {}
};

namespace GameSystem {
    class PlayerInventory {
        Item* items;
        int capacity;
        int count;
    public:
        PlayerInventory(int cap = 5) : capacity(5), count(0) {
            items = new Item[this->capacity];
        }
        ~PlayerInventory() {
            if (items != nullptr) {
                delete[] items;
            }
        }
        PlayerInventory(PlayerInventory& other) {
            this->capacity = other.capacity;
            this->count = other.count;
            this->items = new Item[this->capacity];
            for (int i = 0; i < this->count; i++) {
                this->items[i].id = other.items[i].id;
                this->items[i].rarity = other.items[i].rarity;
            }
        }
        PlayerInventory& operator=(PlayerInventory& other) {
            if (&other == this) {
                return *this;
            }

            if (this->items != nullptr) {
                delete[] this->items;
            }

            this->count = other.count;
            this->capacity = other.capacity;
            this->items = new Item[this->capacity];
            for (int i = 0; i < this->count; i++) {
                this->items[i].id = other.items[i].id;
                this->items[i].rarity = other.items[i].rarity;
            }
            return *this;
        }
        void AddItem(const int id, const Rarity rarity) {
            if (this->count >= capacity) return;
            this->items[this->count].id = id;
            this->items[this->count].rarity = rarity;
            this->count += 1;
        }
        auto GetItemsView() {
            span<Item> views(this->items, count);
            return views;
        }

        int GetCount() const {
            return this->count;
        }

        int GetCapacity() const {
            return this->capacity;
        }
    };
}

// =====================================================================

// [테스트 프레임워크] 아래 main 함수는 수정하지 마세요!
int main() {
    using namespace GameSystem;

    cout << "--- TDD 스타일 테스트 시작 ---\n";

    // 1. 데이터 준비
    PlayerInventory invA(3);
    invA.AddItem(1001, Rarity::Epic);
    invA.AddItem(1002, Rarity::Rare);

    // 2. 복사 생성자 테스트 (초기화)
    PlayerInventory invB = invA;

    auto viewA = invA.GetItemsView();
    auto viewB = invB.GetItemsView();

    assert(invA.GetCount() == invB.GetCount());
    if (invA.GetCount() > 0) {
        assert(viewA[0].id == viewB[0].id);
        assert(viewA.data() != viewB.data() && "에러: 얕은 복사(Shallow Copy)가 발생했습니다! 주소가 같습니다!");
    }
    cout << "[SUCCESS] 복사 생성자 (깊은 복사) 테스트 통과!\n";

    // 3. 대입 연산자 테스트
    PlayerInventory invC(10);
    invC.AddItem(5000, Rarity::Common);

    invC = invA;

    auto viewC = invC.GetItemsView();
    assert(invC.GetCapacity() == invA.GetCapacity());
    if (invC.GetCount() > 0) {
        assert(viewC[0].id == viewA[0].id);
        assert(viewC.data() != viewA.data() && "에러: 대입 연산에서 얕은 복사가 발생했습니다!");
    }
    cout << "[SUCCESS] 대입 연산자 (깊은 복사 & 메모리 해제) 테스트 통과!\n";

    // 4. 셀프 대입 테스트
    invC = invC;
    assert(invC.GetCount() == invA.GetCount() && "에러: 셀프 대입 시 원본 데이터가 날아갔습니다!");
    cout << "[SUCCESS] 셀프 대입 방지 테스트 통과!\n";

    cout << "--- 모든 테스트가 안전하게 통과되었습니다! (메모리 누수 없다면 정상 종료) ---\n";
    return 0;
}

