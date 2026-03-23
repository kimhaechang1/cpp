#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <cassert>
#include<span>

// ==============================================================================
// [누적 연습 M8-1] 제네릭 인벤토리와 파밍 시스템
// 
// [출제 범위]
// - M1~M3: 기본 제어문, 함수와 Call by Reference
// - M4~M5: 원시 포인터 객체의 동적 할당과 메모리 해제
// - M6~M7: 캡슐화, 상속, 다형성(순수 가상 함수), 가상 소멸자
// - M8: 템플릿(제네릭) 클래스 생성, std::vector, std::unordered_map
//
// [요구사항]
// 1. 순수 가상 함수를 가진 추상 클래스 구현 (Item)
//    - 아이템 ID를 보관하는 정수형 필드(접근 제한자는 스스로 판단)가 있어야 합니다. 단, 이 객체들은 게임 내에서 반드시 동적 할당되어 관리되어야 하며 추후 이 객체들의 원본 데이터가 수정될 가능성이 존재합니다.
//    - 매개변수가 하나 있는 생성자를 만들어 ID를 초기화하세요.
//    - ID를 반환하는 멤버 함수(`GetId`)가 있어야 합니다.
//    - 기능: "아이템을 형상화" 하는 순수 가상 함수 `Visualize` 를 정의하세요. (문자열 반환)
//
// 2. 파생 클래스 2개 구현 (Weapon, Potion)
//    - 생성자에서 상속받은 ID를 초기화해야 합니다.
//    - `Visualize` 호출(오버라이딩) 시 Weapon은 "[무기]", Potion은 "[물약]" 이라는 문자열을 각각 반환합니다.
//
// 3. 템플릿 클래스 (GenericSlot) 구현
//    - 이 클래스는 들어오는 "임의의 타입(타입의 종류에 구애받지 않음)"의 데이터를 온전히 보관해야 합니다.
//    - 내부에서 데이터를 보관하는 필드(멤버 변수)의 이름은 'slot' 이어야 합니다.
//    - 단, 이 템플릿에 들어오는 데이터는 "오직 포인터 변수뿐이다"라고 확정 짓고, 생성자 선언부에서 올바르게 비어있음 처리를 하세요.
//    - 데이터를 주입하는 `SetData()` 와 반환하는 `GetData()` 메서드를 반드시 만드세요.
//
// 4. 아이템 자동 분류기 구현 (ProcessLoot)
//    - (일반 함수입니다. 반환 타입과 매개변수 타입은 힌트 없이 스스로 역추적하세요.)
//    - 입력 데이터 1: 바닥에 떨어진 몬스터의 드랍 아이템 목록 (동적으로 생성된 Item 객체 원본들을 순차적으로 보관하는 가변 배열)
//    - 입력 데이터 2: 텅 비어있는 플레이어의 딕셔너리 가방 (내부에 정렬된 상태를 유지해야 하며, 정수형 Key를 통해 동적 메모리를 갖는 아이템들의 원본(Value)을 보관하는 GenericSlot 을 알아낼 수 있습니다.)
//    - [기능]
//      a. 가변 배열의 모든 원소를 순회(for문)합니다.
//      b. 각 아이템 객체 안에서 꺼낸 ID를 열쇠(Key)로 삼아, 가방(딕셔너리)의 보관함(GenericSlot) 내부에 원본 객체를 쏙 집어넣습니다.
//      c. 함수가 종료된 후 바깥 쪽에 있는 원본 가방 데이터 요소들을 접근하여 수정된 결과를 보존하고 있어야 합니다!
//
// [ Blank Slate 적용]: 시그니처 힌트는 없습니다. 아래 테스트 코드를 철저히 분석하고 스스로 타입을 깎아내세요!
// ==============================================================================

// 1. 여기에 Item 의 뼈대(추상 클래스)를 작성하세요.
class Item {
private:
    int id;
public:
    Item(int _id) : id{_id} {

    }
    int GetId() {
        return this->id;
    }
    virtual std::string Visualize() = 0;
    
    virtual ~Item() {

    }
};
// 2. 여기에 Weapon 과 Potion 을 작성하세요.
class Weapon : public Item {
public:
    Weapon(int _id) : Item{_id} {

    }
    std::string Visualize() override {
        return "[무기]";
    }
    virtual ~Weapon() {

    }

};

class Potion : public Item {
public:
    Potion(int _id) : Item{_id} {
    }
    std::string Visualize() override{
        return "[물약]";
    }
    virtual ~Potion(){

    }
};

// 3. 여기에 GenericSlot 템플릿 클래스를 작성하세요.
template<typename T>
class GenericSlot {
private:
    T slot;
public:
    GenericSlot() {
        slot = nullptr;
    }
    T GetData() {
        return this->slot;
    }
    void SetData(T data) {
        this->slot = data;
    }
};

// 4. 여기에 ProcessLoot 함수를 작성하세요.
void ProcessLoot(const std::span<Item*> item_view, std::unordered_map<int, GenericSlot<Item*>>& backpack) {
    for (auto item : item_view) {
        backpack[item->GetId()].SetData(item);
    }
}




// ---------------- C++ 테스트 코드 (Main) ----------------
// 절대 수정하지 마세요!
int main() {
    std::cout << "========= 테스트 시작 =========" << std::endl;

    // 1. 드랍 아이템(동적 할당) 생성 (M4, M5, M7)
    std::vector<Item*> droppedLoot;
    droppedLoot.push_back(new Weapon(101));
    droppedLoot.push_back(new Potion(205));
    droppedLoot.push_back(new Weapon(50));

    // 2. 플레이어 인벤토리 준비 (M8)
    std::map<int, GenericSlot<Item*>> myInventory;

    // 3. 아이템 분류 프로세스 실행 (M3, M8)
    ProcessLoot(droppedLoot, myInventory);

    // 4. 검증 로직
    assert(myInventory.size() == 3 && "ProcessLoot 실패: 3개의 아이템이 가방에 들어와야 합니다.");

    Item* slot101 = myInventory[101].GetData();
    assert(slot101 != nullptr && "GenericSlot 데이터 누락");
    assert(slot101->Visualize() == "[무기]" && "101번 아이템(Weapon) 다형성 실패");

    Item* slot205 = myInventory[205].GetData();
    assert(slot205 != nullptr && "GenericSlot 데이터 누락");
    assert(slot205->Visualize() == "[물약]" && "205번 아이템(Potion) 다형성 실패");

    std::cout << "[PASS] 핵심 프로세스 검증 완료" << std::endl;

    // 5. 메모리 해제 (M5)
    for (size_t i = 0; i < droppedLoot.size(); ++i) {
        delete droppedLoot[i];
    }
    droppedLoot.clear();

    std::cout << "========= 누적 연습(M8-1) 완벽 통과! =========" << std::endl;
    return 0; // 정상 종료
}
