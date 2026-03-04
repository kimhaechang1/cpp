#include <iostream>
#include <memory>
#include <cassert>
#include <string>

// 정상적으로 메모리가 회수되었는지 추적하기 위한 전역 카운터
static int itemCount = 0;

class Item {
private:
    std::string name;
public:
    Item(std::string n) : name(n) {
        itemCount++;
        std::cout << "[Item] '" << name << "' 아이템이 힙(Heap)에 생성되었습니다. (현재 힙에 있는 아이템: " << itemCount << "개)\n";
    }
    ~Item() {
        itemCount--;
        std::cout << "[Item] '" << name << "' 아이템이 철거되었습니다! (현재 힙에 있는 아이템: " << itemCount << "개)\n";
    }
    std::string GetName() const { return name; }
};

// 상자(Chest) 클래스: 내부에 아이템을 동적 할당하여 보관합니다.
class Chest {
private:
    // [미션 1] Raw Pointer(Item*) 대신 똑똑한 스마트 포인터를 사용하여 Item을 보관하세요.
    // 힌트: std::unique_ptr<클래스이름> 변수명;
    std::unique_ptr<Item> item;


public:
    // [미션 2] 생성자입니다. 넘겨받은 itemName으로 Item을 동적 생성하여 스마트 포인터에 담아주세요.
    // 힌트 1: 원본 포인터를 쓸 때는 new Item(itemName) 이었지만, 
    // 힌트 2: 스마트 포인터에서는 std::make_unique<클래스이름>(생성자_매개변수) 를 씁니다!
    // 변수명 = std::make_unique<...>(...);
    Chest(std::string itemName) {
        item = std::make_unique<Item>(itemName);
    }

    // [미션 3] 스마트 포인터가 가리키고 있는 Item의 이름을 반환하는 함수를 구현하세요.
    // 힌트: 스마트 포인터는 겉보기에 일반 포인터와 조작법이 100% 동일합니다. (-> 연산자 사용)
    std::string GetItemName() const {
        // return ...;
        return item->GetName();
    }

    // [미션 4] 소멸자입니다. 아무것도 지우지 마세요! (Rule of Zero 달성)
    ~Chest() {
        std::cout << "[Chest] 상자가 껍데기(Stack)를 다하고 파괴됩니다... 내부 스마트 포인터 작동 개시!\n";
    }
};

int main() {
    std::cout << "--- [Rule of Zero] 스마트 포인터 미리보기 실습 ---\n";

    assert(itemCount == 0); // 시작 전엔 깨끗해야 함

    { // 임시 지역 스코프(Scope) 하나 열어보기
        std::cout << "=> 보물 상자(Stack 지역 변수)를 하나 생성합니다.\n";
        Chest myChest("Legendary Sword");

        // 아이템이 정상적으로 힙(Heap)에 1개 만들어졌는지 검증
        assert(itemCount == 1);
        // 포인터 접근이 뚫려서 이름이 잘 가져와지는지 검증
        assert(myChest.GetItemName() == "Legendary Sword");

        std::cout << "=> 블록(Scope)이 곧 끝납니다...\n";
    } // <-- 이 괄호를 빠져나가는 순간 myChest가 죽으면서 자동으로 스마트 포인터가 일함

    std::cout << "=> 상자가 사라진 후, 힙(Heap) 검사!\n";
    // 상자가 파괴될 때 내부의 unique_ptr도 파괴되며 자기가 품은 Item을 delete 시켰을 것입니다!
    assert(itemCount == 0);

    std::cout << "\n🌟 [성공] Rule of Zero를 완벽하게 달성하셨습니다! 🌟\n";
    return 0;
}
