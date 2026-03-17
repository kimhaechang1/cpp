#include <iostream>
#include <string>
#include <vector>
#include <cassert>

// ---------------------------------------------------------
// [실습 요구사항]
// 
// 1. 인터페이스(추상 클래스) 설계: IInteractable
//    - 기능 1: Interact() -> 반환값 없음. 상호작용 시 메시지 출력. (순수 가상 함수)
//    - 기능 2: GetName() -> 상호작용 대상의 이름을 string으로 반환. (순수 가상 함수)
//    - 주의: 상속을 위한 가상 소멸자를 반드시 포함하세요.
//
// 2. 자식 클래스 1 구현: Chest (상자)
//    - GetName() 호출 시 "Treasure Chest" 반환
//    - Interact() 호출 시 "You opened the chest and found a gold coin!" 출력
//
// 3. 자식 클래스 2 구현: Door (문)
//    - GetName() 호출 시 "Iron Door" 반환
//    - Interact() 호출 시 "The door creaks open slowly..." 출력
//
// 4. 추상 클래스 본체(Body) 실습:
//    - IInteractable 의 가상 소멸자 혹은 다른 순수 가상 함수에 본체를 클래스 외부에서 구현해보고,
//      자식 클래스에서 호출해보세요. (자유 선택)
// ---------------------------------------------------------

// TODO: 여기에 클래스들을 구현하세요.
class IInteractable {
public:
    // 순수가상함수 선언시에는 = 0; 빼먹지 말기
    virtual void Interact() = 0;
    virtual std::string GetName() = 0;
    
    virtual ~IInteractable() {
        // 가상 함수가 포함된 클래스는 런타임에 vptr 이 생긴다.
        // vptr 은 자식과 부모가 공유하는 필드가된다.
        // 가상 소멸자 역시 가상함수처럼 작동한다.
        // 단, 부모의 소멸자가 가상함수인 경우 자식을 갔다가 돌아와서 부모를 실행하는 과정을 거칠뿐이다.
    }
};

class Chest : public IInteractable {
public:
    // 가상함수가 포함된 클래스는 런타임에 vptr 이 생긴다.
    // 근데 애초에 부모에서 vptr 이 있으니까 그기를 덮어씌워서 자신의 컴파일타임에 생기는 vtable 을 바라보게 하면 된다.
    void Interact() override {
        std::cout << "You opened the chest and found a gold coin!" << std::endl;
    }

    std::string GetName() override {
        return "Treasure Chest";
    }
};

class Door : public IInteractable {
public:
    void Interact() override {
        std::cout << "The door creaks open slowly..." << std::endl;
    }

    std::string GetName() override {
        return "Iron Door";
    }
};



int main() {
    // 1. 추상 클래스 인스턴스화 금지 체크
    // 아래 주석을 해제했을 때 컴파일 에러가 나야 정상입니다.
    // IInteractable test; 

    // 2. 다형성을 이용한 상호작용 시스템 테스트
    std::vector<IInteractable*> worldObjects;

    worldObjects.push_back(new Chest());
    worldObjects.push_back(new Door());

    std::cout << "--- Interaction Start ---" << std::endl;
    for (auto obj : worldObjects) {
        std::cout << "[" << obj->GetName() << "]: ";
        obj->Interact();
    }

    // 3. 검증 (Validation)
    assert(worldObjects[0]->GetName() == "Treasure Chest");
    assert(worldObjects[1]->GetName() == "Iron Door");

    // 4. 하드웨어/메모리 검증
    std::cout << "\n--- Memory Analysis ---" << std::endl;
    // 추상 클래스와 자식 클래스의 크기를 비교해보세요. (vtable 포인터 포함 여부 등)
    std::cout << "Size of Chest object: " << sizeof(Chest) << " bytes" << std::endl;

    // 리소스 해제
    for (auto obj : worldObjects) {
        delete obj;
    }
    worldObjects.clear();

    std::cout << "\nAll interactions and tests passed!" << std::endl;

    return 0;
}
