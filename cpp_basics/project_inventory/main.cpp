#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <format>
#include <cassert> // 검증용
#include <crtdbg.h> // 메모리 누수 탐지
#include <span>

// -------------------------------------------------------------
// [Intermediate Mini-Project]
// 주제: 텍스트 RPG 인벤토리 시스템 (TDD 스타일)
// 범위: Module 01 ~ Module 05 (변수, 제어문, 함수, 포인터, 동적할당, 구조체, 열거형)
// 
// [목표]
// main() 함수에 있는 'TEST CODE'들이 모두 통과(Pass)하도록
// 위에 비어있는 함수들을 구현하세요.
// -------------------------------------------------------------

// M1: Enum (아이템 타입)
enum class ItemType {
    WEAPON,
    ARMOR,
    POTION
};

// M1: Struct (아이템 구조체)
struct Item {
    std::string name;
    ItemType type;
    int price;
};

// 상수 정의
// 상수 정의 (M1: const 변수)
const int MAX_SLOTS = 10;

// -------------------------------------------------------------
// 구현해야 할 함수 목록 (Prototyes)
// -------------------------------------------------------------

// 1. 아이템 생성 (Dynamic Allocation)
// 기능: 힙 메모리에 Item을 할당하고, 매개변수로 초기화하여 반환.
Item* CreateItem(std::string name, ItemType type, int price) {
    // TODO: 구현하세요.
    // 1. new Item ...
    // 2. 값 대입
    // 3. 포인터 반환
    // (Dummy Return)
    return new Item{ name, type, price };
}

// 2. 아이템 추가 (Pointer Array Logic)
// 기능: 인벤토리(배열)의 빈 슬롯(nullptr)을 찾아 아이템을 넣으세요.
// 반환: 성공 시 true, 꽉 찼으면 false
// 힌트: std::span을 써서 배열의 크기를 안전하게 받을 수 있습니다. (M4)
bool AddItem(std::span<Item*> inventory, Item* newItem) {
    // TODO: 구현하세요.
    // 반복문을 돌며 nullptr인 곳을 찾아 newItem을 대입.
    for (auto& slot : inventory) {
        if (slot != nullptr) continue;
        slot = newItem;
        return true;
    }
    return false;
}

// 3. 아이템 검색 (Search Logic)
// 기능: 이름으로 아이템을 찾아서 그 포인터를 반환. (없으면 nullptr)
// 매개변수: const 참조인 std::string& name 사용 (M3)
Item* FindItem(std::span<Item*> inventory, const std::string& name) {
    // TODO: 구현하세요.
    for (Item* item : inventory) {
        if (item == nullptr) continue;
        if (item->name != name) continue;
        return item;
    }
    return nullptr;
}

// 4. 아이템 삭제 (Memory Management)
// 기능: 해당 인덱스의 아이템을 메모리 해제(delete)하고, 슬롯을 nullptr로 초기화.
// 주의: 이미 비어있거나 인덱스가 범위를 벗어나면 아무것도 하지 않음.
void RemoveItemAt(std::span<Item*> inventory, int index) {
    // TODO: 구현하세요.
    // 1. 인덱스 체크
    // 2. nullptr 체크
    // 3. delete
    // 4. = nullptr
    int sz = inventory.size();
    if (index < 0 || index >= sz) return;
    Item* target = inventory[index];
    if (target == nullptr) return;
    delete target;
    inventory[index] = nullptr;
}

// 5. 인벤토리 정리 (Clear All)
// 기능: 인벤토리에 남아있는 모든 아이템을 해제하고 비움.
void ClearInventory(std::span<Item*> inventory) {
    // TODO: 구현하세요.
    // RemoveItemAt을 재활용하면 좋겠죠?
    for (int index = 0; index < inventory.size(); index++){
        RemoveItemAt(inventory, index);
    }
}


// -------------------------------------------------------------
// 메인 함수 (테스트 시나리오)
// -------------------------------------------------------------
int main() {
    // 메모리 누수 탐지 설정 (M5)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    std::cout << "=== Intermediate Mini-Project: Inventory System ===\n";

    // 인벤토리 초기화 (포인터 배열)
    // M5: std::array를 사용하면 더 안전합니다.
    std::array<Item*, MAX_SLOTS> inventory;
    inventory.fill(nullptr); // 모든 슬롯을 nullptr로 초기화 (아주 중요!)

    // ---------------------------------------------------------
    // TEST 1: 아이템 생성 (CreateItem)
    // ---------------------------------------------------------
    Item* sword = CreateItem("Steel Sword", ItemType::WEAPON, 100);
    Item* potion = CreateItem("HP Potion", ItemType::POTION, 50);

    // 검증 (Assert)
    assert(sword != nullptr && "아이템이 생성되지 않았습니다.");
    assert(sword->name == "Steel Sword" && "이름이 잘못 설정되었습니다.");
    assert(sword->price == 100 && "가격이 잘못 설정되었습니다.");
    std::cout << "[PASS] CreateItem\n";

    // ---------------------------------------------------------
    // TEST 2: 아이템 추가 (AddItem)
    // ---------------------------------------------------------
    bool added1 = AddItem(inventory, sword); // 0번 슬롯
    bool added2 = AddItem(inventory, potion); // 1번 슬롯

    assert(added1 == true && "첫 번째 아이템 추가 실패");
    assert(inventory[0] == sword && "0번 슬롯에 검이 들어있지 않습니다.");
    assert(inventory[1] == potion && "1번 슬롯에 포션이 들어있지 않습니다.");
    std::cout << "[PASS] AddItem\n";

    // ---------------------------------------------------------
    // TEST 3: 아이템 검색 (FindItem)
    // ---------------------------------------------------------
    Item* found = FindItem(inventory, "HP Potion");
    assert(found == potion && "포션을 찾지 못했거나 잘못된 주소를 반환했습니다.");

    Item* notFound = FindItem(inventory, "Dragon Slayer");
    assert(notFound == nullptr && "없는 아이템인데 nullptr가 아닙니다.");
    std::cout << "[PASS] FindItem\n";

    // ---------------------------------------------------------
    // TEST 4: 아이템 삭제 (RemoveItemAt)
    // ---------------------------------------------------------
    RemoveItemAt(inventory, 0); // 검 삭제

    assert(inventory[0] == nullptr && "삭제 후 슬롯이 nullptr가 아닙니다. (Dangling Pointer 위험)");
    // *주의*: 여기서 sword 포인터를 사용하면 안 됨! (이미 delete됨)
    std::cout << "[PASS] RemoveItemAt\n";

    // ---------------------------------------------------------
    // TEST 5: 전체 정리 (ClearInventory)
    // ---------------------------------------------------------
    // 더미 아이템 몇 개 더 추가
    AddItem(inventory, CreateItem("Shield", ItemType::ARMOR, 200));
    AddItem(inventory, CreateItem("Helmet", ItemType::ARMOR, 150));

    ClearInventory(inventory);

    bool allCleared = true;
    for (auto slot : inventory) {
        if (slot != nullptr) allCleared = false;
    }
    assert(allCleared && "인벤토리가 완전히 비워지지 않았습니다.");
    std::cout << "[PASS] ClearInventory\n";


    std::cout << "\n 모든 테스트를 통과했습니다! 중급 단계를 마스터하셨습니다.\n";
    return 0;
}
