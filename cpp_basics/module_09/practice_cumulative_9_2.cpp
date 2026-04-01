#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <cassert>
#include <algorithm>

// =========================================================================
// [Cumulative Practice 9-2] Type Inference & Structured Binding
// 
// 🚨 엄격한 백지 정책 적용 🚨
// 주제: [다형성 인벤토리 데이터베이스 추출기]
// 요구사항:
// - 기존 M1~M8 (클래스, 다형성, STL, 람다) 기능과
// - M9 (스마트 포인터, 이동 시맨틱, 구조 분해 할당, decltype)을 결합합니다.
// =========================================================================

// -------------------------------------------------------------------------
// [TODO 1] M6(클래스/추상), M7(다형성/가상 소멸자)
// 1. 'Item' 이라는 이름의 추상 클래스를 작성하세요. 
//    - 외부 노출은 금지하되 자식에게만 허용되는 멤버 변수(정수형 itemId)를 가지세요.
//    - "다형성 환경에서 자식 삭제 시 누수 방지 공사"를 필수로 수행하세요.
//    - 순수 가상 함수 'GetItemName()'을 선언하세요.
// 2. Item을 상속받는 'Weapon' 클래스를 구현하세요.
//    - 생성자(Item_ID, 이름(string))로 부모를 초기화합니다.
//    - GetItemName()을 오버라이딩(명시적 키워드 사용) 하세요.
// -> 여기에 작성
class Item {
protected:
    int itemId;
public:
    Item(int itemId) : itemId{itemId} {

    }
    virtual ~Item() {

    }
    virtual std::string GetItemName() = 0;
    int GetItemId() {
        return this->itemId;
    }
    Item(Item&& other) {
        this->itemId = other.itemId;
    }
    
    Item& operator=(Item&& other) {
        if (this == &other) {
            return *this;
        }
        this->itemId = other.itemId;
    }
};

class Weapon : public Item {
    std::string name;
public:
    Weapon(int itemId, std::string name) : Item{ itemId }, name{name} {

    }
    virtual ~Weapon() {

    }

    std::string GetItemName() override {
        return this->name;
    }

    Weapon(Weapon&& other) : Item{ other.itemId } {
        this->name = other.name;
    }

    Weapon& operator=(Weapon&& other) {
        if (this == &other) {
            return *this;
        }
        this->itemId = other.itemId;
        this->name = other.name;
    }
    
};


// -------------------------------------------------------------------------
// [TODO 2] M8(STL), M9(스마트 포인터/이동 시맨틱)
// 독점 소유권을 가지는 Item 객체들을 보관할 'DbManager' 구조체를 만드세요.
// 내부에 아이템의 고유 ID를 Key로 하고, "아이템의 독점 소유권 래퍼"를 Value로 가지는 
// map(이름: itemTable) 구조를 하나 선언합니다.
// -> 여기에 작성
struct DbManager {
    std::map<int, std::unique_ptr<Item>> itemTable;
};


// -------------------------------------------------------------------------
// [TODO 3] M3(함수/참조), M8(람다), M9(구조 분해 할당, 이동)
// [🚨 함수 원형 스스로 작성!]
// 이름: ExtractAllWeapons
// 기능 요구사항:
// 1. 매개변수로 아까 만든 'DbManager' 원본 데이터를 넘겨받습니다. (복사 금지!)
// 2. 반환값으로 "아이템 독점 소유권의 무리의 1차원 컨테이너(가변 가변 배열)"를 리턴합니다.
// 3. (구조 분해 할당) 원본 DbManager의 itemTable 맵을 순회하면서, 직관적인 이름으로 분해하세요. (ex. id, item)
// 4. (이동 시맨틱) 분해한 후 원본 맵에 있는 아이템 소유권을 강제로 빼앗아(강탈), 반환할 배열에 밀어 넣습니다.
//    참고로, 원본 맵은 더 이상 데이터를 가지고 있지 않아야 정상 작동한 것입니다!
// -> 여기에 작성
std::vector<std::unique_ptr<Item>> ExtractAllWeapons(DbManager& db) {
    std::vector<std::unique_ptr<Item>> rtValue;
    for (auto& [id, itemPtr] : db.itemTable) {
        // move 자체가 하는일이 rvalue 로 바꿔주는것도 있지만
        // 이동시멘틱을 호출한 뒤에 곧바로 nullptr 을 대입한다.
        rtValue.push_back(std::move(itemPtr));
    }
    db.itemTable.clear();
    return rtValue;
}



int main() {
    std::cout << "[ Cumulative 9-2: Database Extractor Start ]\n\n";

    // 데이터 세팅
    // TODO 2에서 작성한 DbManager 인스턴스를 하나 만들고 (이름: manager)
    // 그 내부의 itemTable에 Weapon 객체를 2개 스마트 포인터 형태로 집어넣습니다.
    // -> 여기에 세팅 코드 작성 (ID 1: "Sword", ID 2: "Bow")
    DbManager manager;
    std::unique_ptr<Item> sword = std::make_unique<Weapon>(1, "Sword");
    std::unique_ptr<Item> bow = std::make_unique<Weapon>(2, "Bow");
    manager.itemTable[sword->GetItemId()] = std::move(sword);
    manager.itemTable[bow->GetItemId()] = std::move(bow);

    // 검증 1: 시작 시 맵 크기 확인
    assert(manager.itemTable.size() == 2);

    // TODO 3에서 만든 함수를 호출하여 무기 배열 획득 (반환된 객체를 'auto' 로 받습니다)
    // -> 여기에 호출 코드 작성 (auto extractedItems = ...)
    auto extractedItems = ExtractAllWeapons(manager);


    // 검증 2: 원본 맵에서 데이터가 완전히 뽑아져 나갔는지 검증!
    // (이동 처리가 잘 되었다면 원본 객체 내부 소유권 포인터들은 nullptr 가 되지만 map 원소 갯수가 지워지진 않음.
    // 하지만 만약 요소 내부가 비었는지 체크하는거라면, 추출된 갯수로 테스트합니다.)

     assert(extractedItems.size() == 2);
     assert(extractedItems[0]->GetItemName() == "Sword");
     assert(extractedItems[1]->GetItemName() == "Bow");

    // ---------------------------------------------------------
    // [TODO 4] M9(decltype)를 사용한 타입 동일성 보장
    // -> 추출된 리스트의 첫 번째 아이템의 타입을 완벽히 복제한 변수 'firstItemClone(임시 포인터)'를 작성하세요.
    // -> 값의 복사가 아니라 동일한 소유권 래퍼의 "타입"만 가져와서, 이동(move)으로 받아와야 합니다.
    // -> 여기에 작성
     auto firstItemClone = std::move(extractedItems[0]);

     assert(firstItemClone->GetItemName() == "Sword");

    std::cout << "🌟 9-2 종합 테스트 (다형성, 소유권 이동, 구조 분해) 통과 완료! 🌟\n";
    return 0;
}
