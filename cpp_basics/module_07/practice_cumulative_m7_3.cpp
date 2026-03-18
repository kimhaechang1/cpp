#include <iostream>
#include <string>
#include <vector>
#include <cassert>

// [요구사항 1] 엔티티의 상태(정상, 기절, 사망)를 나타내는 열거형을 정의하세요.
// 이름: EntityStatus (값: Normal, Stunned, Dead)
enum EntityStatus {
    Normal, Stunned, Dead
};



// [요구사항 2] 모든 엔티티의 최상위 추상 클래스를 설계하세요.
// 이름: BaseEntity
// - protected 멤버: 정수형 고유 번호(id)
// - 생성자: id를 초기화함
// - 소멸자: "Entity [ID] Destroyed"를 출력해야 함 (메모리 누수 방지를 위한 특수 처리 필요)
// - 순수 가상 함수: Action() (매개변수 없음, 리턴 없음)
// - 일반 가상 함수: GetStatus() (현재 상태 리턴)
class BaseEntity {
protected:
    int id;
public:
    BaseEntity(int _id): id(_id) {

    }
    virtual ~BaseEntity() {
        // virtual 함수를 갖고있다.
        // 즉 업캐스팅 가능성이 있기에 부모 소멸자를 가상 소멸자로 만들어야한다.
        std::cout << "Entity [" << this->id << "] Destroyed" << std::endl;
    }
    virtual void Action() = 0;
    // 순수 가상함수를 만들었다.
    // 반드시 자식에서 구현해야한다.
    // default 함수를 만들고싶다면 외부에서 구현하면된다.
    virtual EntityStatus GetStatus() {
        return EntityStatus::Normal;
    }
};



// [요구사항 3] 공격 가능한 인터페이스를 설계하세요. (다중 상속 실습)
// 이름: IAttackable
// - 순수 가상 함수: Attack() (매개변수 없음, 리턴 없음)
class IAttackable {
public:
    virtual void Attack() = 0;
};


// [요구사항 4] BaseEntity와 IAttackable을 상속받는 구체 클래스를 설계하세요.
// 이름: Character
// - private 멤버: 문자열 이름(name), 상태(status)
// - 생성자: id, name을 입력받아 초기화. 초기 상태는 Normal.
// - 소멸자: "Character [Name] Destroyed" 출력
// - Action(): "[Name] is acting..." 출력
// - Attack(): "[Name] performs an attack!" 출력
// - GetStatus(): 현재 status 리턴 (override 필수)
class Character : public BaseEntity, public IAttackable {
private:
    std::string name;
    EntityStatus status;
public:
    Character(int _id, std::string _name) : BaseEntity{_id}, name(_name) {
        this->status = EntityStatus::Normal;
    }
    virtual ~Character() {
        std::cout << "[" << this->name << "] is Destroyed" << std::endl;
    }
    void Action() override {
        std::cout << "[" << this->name << "] is acting..." << std::endl;
    }
    void Attack() override {
        std::cout << "[" << this->name << "] performs an attack!" << std::endl;
    }
    EntityStatus GetStatus() override {
        return this->status;
    }

};


// [요구사항 5] 엔티티의 생명주기를 관리하는 매니저 클래스를 설계하세요. (RAII & 메모리 관리)
// 이름: EntityManager
// - 멤버: BaseEntity 포인터들을 보관할 수 있는 동적 배열 또는 컨테이너
// - 소멸자: 보관 중인 모든 엔티티를 안전하게 메모리 해제(delete)해야 함
// - 함수: AddEntity(BaseEntity* entity) -> 엔티티 추가
// - 함수: ProcessEntities() -> 모든 엔티티의 Action()을 호출
// - 함수: ExecuteAttacks() -> IAttackable로 형변환(Casting) 가능한 엔티티들만 Attack() 호출
class EntityManager {
private:
    std::vector<BaseEntity*> entities;
public:
    ~EntityManager() {
        for (int i = 0; i < entities.size(); i++) {
            if (entities[i] != nullptr) {
                delete entities[i];
            }
        }
    }
    void AddEntity(BaseEntity* entity) {
        this->entities.push_back(entity);
    }
    void ProcessEntities() {
        for (int i = 0; i < entities.size(); i++) {
            entities[i]->Action();
        }
    }
    void ExecuteAttacks() {
        for (int i = 0; i < entities.size(); i++) {
            IAttackable* entity = dynamic_cast<IAttackable*>(entities[i]);
            if (entity) {
                entity->Attack();
            }
        }
    }
};



int main() {
    std::cout << "--- Starting Cumulative Practice M7_3 ---" << std::endl;

    {
        // 1. 매니저 생성
        EntityManager* manager = new EntityManager();

        // 2. 캐릭터들 동적 할당하여 추가 (M5, M7 활용)
        // 주의: manager에 추가된 객체들은 manager의 소멸자에서 책임지고 지워야 함
        manager->AddEntity(new Character(1, "Warrior"));
        manager->AddEntity(new Character(2, "Archer"));

        std::cout << "\n[Step 1: Process Actions]" << std::endl;
        manager->ProcessEntities();

        std::cout << "\n[Step 2: Execute Attacks]" << std::endl;
        manager->ExecuteAttacks();

        // 3. 매니저 소멸 (RAII 테스트)
        std::cout << "\n[Step 3: Cleanup]" << std::endl;
        delete manager;
        // 결과창에 Warrior, Archer의 소멸자 메시지가 모두 정상 출력되어야 함.
        // 만약 virtual destructor 처리가 안 되었다면 메모리 릭이 발생하거나 파생 소멸자가 호출 안 됨.
    }

    std::cout << "\n--- Practice Complete ---" << std::endl;
    return 0;
}
