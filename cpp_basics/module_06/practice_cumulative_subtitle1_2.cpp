#include <iostream>
#include <string>
#include <vector>
#include <format>
#include <cassert> // 필수 포함

// -------------------------------------------------------------
// [Cumulative Practice] Module 06 Subtitle 1 & 2
// 범위: Namespace, Class, Constructor, Destructor
//       + M1~M5 (vector, string, pointer, dynamic allocation)
// -------------------------------------------------------------

namespace RPG {

    // [Quest 1] Item 클래스를 '직접' 정의하세요.
    // 
    // [요구사항]
    // 1. 클래스 이름: Item
    // 2. 멤버 변수: 
    //    - 아이템 이름 (문자열)
    //    - 가격 (정수)
    //    - (접근 지정자는 정보 은닉을 위해 private 권장)
    // 3. 생성자:
    //    - 이름과 가격을 인자로 받아 초기화 (초기화 리스트 권장)
    // 4. 멤버 함수 (Getter):
    //    - GetName(): 이름 반환 (const 함수)
    //    - GetCost(): 가격 반환 (const 함수)

    // (여기에 Item 클래스를 작성하세요)
    class Item {
        std::string name;
        int cost;
    public:
        Item(std::string n, int c) :
            name(n), cost(c) {}
        std::string GetName() const {
            return name;
        }
        int GetCost() const {
            return cost;
        }
    };

    // [Quest 2] Inventory 클래스를 '직접' 정의하세요.
    //
    // [요구사항]
    // 1. 클래스 이름: Inventory
    // 2. 멤버 변수:
    //    - 아이템들을 저장할 컨테이너 (vector 추천, Item 포인터 저장)
    //    - 현재 골드 (정수)
    // 3. 생성자:
    //    - 시작 골드를 인자로 받아 초기화
    // 4. 소멸자:
    //    - 저장된 아이템들(포인터)을 모두 메모리 해제(delete)하고 벡터 비우기 (Memory Leak 방지)
    // 5. 멤버 함수:
    //    - BuyItem(이름, 가격): 
    //        - 돈이 충분하면 차감 후 Item 동적 할당하여 추가 -> true 반환
    //        - 돈이 부족하면 -> false 반환
    //    - ShowAll():
    //        - 현재 골드와 보유 아이템들의 이름/가격 출력

    // (여기에 Inventory 클래스를 작성하세요)
    class Inventory {
        std::vector<Item*> container;
        int gold;
    public:
        Inventory(int g) {
            gold = g;
        }
        ~Inventory() {
            for (Item* item : container) {
                delete item;
                item = nullptr;
                // 어짜피 소멸자라서 vector 가 사라지는것이기 때문에 nullptr 까지 해줄 필요는 없다.
                // 그리고 주의할 점으로 Item* item : container 의 경우, 복사 생성자 함수 내의 지역변수일 뿐이다.
                // 그래서 nullptr 해봤자 의도하였던 실제 vector 내의 Item* 가 nullptr 되진 않는다.
                // 실제로 해줄거라면, 참조자를 통해 실제값에 접근 하려고 해야한다.
                // Item*& item: container 이런식으로 한다면 item 변수는 실제 변수가 아닌 참조자로서 의미를 갖게 되고 ]
                // 어디에 있는 값을 그대로 보고싶으냐? 를 Item* 로 정함으로서, Item* 가 가리키는 힙메모리 주소를 그대로 참조하게 된다.
            }
        }
        bool BuyItem(std::string name, int price) {
            int gap = gold - price;
            bool result = false;
            if (gap >= 0) {
                Item* item = new Item{ name, price };
                container.push_back(item);
                gold = gap;
                result = true;
            }
            return result;
        }
        void ShowAll() {
            std::cout << "현재 보유 골드: " << gold << std::endl;
            for (Item* item : container) {
                std::cout << "[" << item->GetName() << "] 가격: " << item->GetCost() << std::endl;
            }
        }
    };

} // namespace RPG

int main() {
    std::cout << "=== Cumulative Practice (M6-1, M6-2) ===\n";

    // 1. Item 클래스 테스트
    // (주석을 풀었을 때 컴파일되고 Assert를 통과해야 합니다)
    
    {
        RPG::Item potion("Potion", 100);
        assert(potion.GetName() == "Potion");
        assert(potion.GetCost() == 100);
        std::cout << "Test 1: Item Construction & Getter -> PASS\n";
    }
    

    // 2. Inventory 클래스 테스트
    // (주석을 풀었을 때 컴파일되고 Assert를 통과해야 합니다)
    
    {
        RPG::Inventory myInv(1000);

        // [Case 1] 정상 구매
        bool success1 = myInv.BuyItem("HP Potion", 100);
        assert(success1 == true);

        // [Case 2] 정상 구매 2
        bool success2 = myInv.BuyItem("Long Sword", 500);
        assert(success2 == true);

        // [Case 3] 돈 부족 (남은 돈 400 < 10000)
        bool success3 = myInv.BuyItem("Dragon Armor", 10000);
        assert(success3 == false);

        std::cout << "Test 2: Inventory Buy Logic -> PASS\n";

        myInv.ShowAll();

    } // 블록 끝 -> Inventory 소멸자 호출 -> 내부 아이템 delete 검증

    std::cout << "\n=== All Tests Passed (Memory Leak Check Required) ===\n";
    std::cout << "구현 후 main() 내부의 주석을 해제하여 테스트하세요!\n";

    return 0;
}
