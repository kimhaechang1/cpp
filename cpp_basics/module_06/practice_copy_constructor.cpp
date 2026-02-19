#include <iostream>
#include <vector>
#include <string>
#include <cassert>

// -------------------------------------------------------------
// [Module 06] Copy Constructor Practice (Deep Copy)
// 목표: 얕은 복사(Shallow Copy)의 위험성을 이해하고, 
//       깊은 복사(Deep Copy)를 수행하는 복사 생성자를 직접 구현합니다.
// -------------------------------------------------------------

namespace RPG {

    class Item {
        std::string name;
        int cost;
    public:
        Item(std::string n, int c) : name(n), cost(c) {}
        std::string GetName() const { return name; }
        int GetCost() const { return cost; }
    };

    class Inventory {
    private:
        std::vector<Item*> items; // 동적 할당된 Item의 포인터들을 저장
        int gold;

    public:
        // [1] 기본 생성자
        Inventory(int g) : gold(g) {}

        // [2] 소멸자 (메모리 해제)
        ~Inventory() {
            std::cout << "DEBUG: Destructor Called for Inventory at " << this << std::endl;
            for (Item* item : items) {
                if (item) delete item;
            }
            items.clear();
        }

        // [3] 아이템 구매
        void BuyItem(std::string name, int price) {
            Item* newItem = new Item(name, price);
            items.push_back(newItem);
            gold -= price; // (간소화를 위해 잔액 체크 생략)
        }

        // TODO: 복사 생성자를 '직접' 구현하세요.
        // 주석 처리된 부분을 완성해야 합니다.
        // [요구사항]
        // 1. 'other'의 gold를 복사합니다.
        // 2. 'other.items'를 순회하며, 각 아이템을 *새로 동적 할당(new)* 하여 내 목록(items)에 추가합니다.
        // 3. 단순히 포인터만 복사하면(얕은 복사), 소멸자에서 Double Free 에러가 발생합니다.
        
        Inventory(const Inventory& other) {
            std::cout << "DEBUG: Deep Copy Constructor Called!" << std::endl;
            // 구현하세요...
            this->gold = other.gold;
            for (Item* item : other.items) {
                this->items.push_back(new Item{ item->GetName(), item->GetCost() });
            }
        }
        
        void Show() const {
            std::cout << "[Inventory " << this << "] Gold: " << gold
                << ", Item Count: " << items.size() << std::endl;
        }
    };

} // namespace RPG

int main() {
    std::cout << "=== Copy Constructor Test ===\n";

    {
        std::cout << "1. Create Inven A\n";
        RPG::Inventory A(1000);
        A.BuyItem("Excalibur", 100);

        std::cout << "2. Copy A to B (B = A)\n";
        // 복사 생성자가 호출됩니다.
        // 만약 복사 생성자가 구현되지 않았다면, 기본(얕은) 복사가 일어납니다.
        RPG::Inventory B = A;

        B.Show();

        std::cout << "3. End of Block (Destructors will be called)\n";
        // 여기서 A와 B가 소멸됩니다.
        // 얕은 복사라면?? -> A가 지운 메모리를 B가 또 지우려 함 -> 💥 펑!
        // 깊은 복사라면?? -> A는 A꺼 지우고, B는 B꺼 지우고 -> 평화 🕊️
    }

    std::cout << "=== Program Survived! ===\n";
    return 0;
}
