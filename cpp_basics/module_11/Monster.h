#pragma once
#include <memory>
#include <optional>
#include <string>

/**
 * [M7] Monster 추상 클래스
 *
 * 모든 몬스터의 뼈대(골격)가 되는 최상위 설계도를 만드세요.
 * 이 설계도 자체로는 실체화(인스턴스)가 불가능해야 합니다.
 */
class Monster
{
  public:
    /**
     * [M7] 가상 소멸자
     * 부모 설계도를 통해 자식 인스턴스가 소멸될 때 메모리 누수가 일어나지
     * 않도록 특별한 장치를 해야 합니다.
     */
    // (소멸자 선언)
    virtual ~Monster() = default;

    /**
     * [M7] 순수 가상 함수: 공격 이름 조회
     * 각 몬스터가 반드시 자신만의 방식으로 구현해야 하는 공격 이름 조회 기능을
     * 선언하세요. 이 기능은 본체(Body)가 없어야 합니다.
     */
    // (GetAttackName 함수 선언)
    // [Review] const는 반환 타입 앞에 위치하고, virtual이 맨 앞에 오는 것이 관례
    virtual const std::string &GetAttackName() const = 0;

    /**
     * [M6/M7] HP 조회
     * 현재 몬스터의 체력을 외부에서 읽기 전용으로 조회할 수 있는 기능이
     * 필요합니다.
     */
    // (GetHp 함수 선언)
    // [Review] 값 반환(int)에서 const는 무의미 - 호출자가 받는 것은 복사본
    int GetHp() const
    {
        return this->hp;
    }

    /**
     * [M9] optional 드롭 아이템
     * 보스급 몬스터만 아이템을 드롭합니다. 에러 방지를 위해 -1이나 nullptr 대신
     * '값이 있을 수도 비어있을 수도 있는' 안전상자로 포장해 반환하세요.
     */
    // (GetDropItem 함수 선언)
    virtual std::optional<std::string> GetDropItem() = 0;

  protected:
    /**
     * [M7] HP 접근 제어
     * 외부 노출은 철저히 막되, 오직 나를 상속받은 자식 클래스에게만
     * 데이터 접근 권한을 물려주어야 합니다.
     */
    // (HP 필드)
    int hp;
    Monster(int hp_) : hp{hp_}
    {
    }
    Monster(Monster &other) : hp{other.hp}
    {
    }
    Monster(Monster &&other) : hp{other.hp}
    {
        other.hp = 0;
    }
    Monster &operator=(Monster &other)
    {
        if (this == &other) {
            return *this;
        }
        this->hp = other.hp;
        return *this;
    }
    Monster &operator=(Monster &&other)
    {
        if (this == &other) {
            return *this;
        }
        this->hp = other.hp;
        other.hp = 0;
        return *this;
    }
};

/**
 * [M7] Goblin 클래스
 * 하급 몬스터입니다. HP는 50이며, 'Slash' 공격을 사용합니다.
 * 보스가 아니므로 드롭 아이템이 없습니다.
 */
class Goblin : public Monster
{
    // (필요한 멤버 및 생성자 선언)
    std::string attackName;

  public:
    Goblin() : Monster{50}, attackName{"Slash"}
    {
    }
    virtual ~Goblin()
    {
    }
    Goblin(Goblin &other) : Monster{other}, attackName{other.attackName}
    {
    }
    Goblin(Goblin &&other)
        // [Review] rvalue 참조라도 이름이 있으면 lvalue이므로 std::move 필수
        : Monster{std::move(other)}, attackName{std::move(other.attackName)}
    {
    }
    Goblin &operator=(Goblin &other)
    {
        if (this == &other) {
            return *this;
        }
        Monster::operator=(other);
        this->attackName = other.attackName;
        return *this;
    }
    Goblin &operator=(Goblin &&other)
    {
        if (this == &other) {
            return *this;
        }
        // [Review] 이동 대입이므로 부모에게도 std::move로 전달
        Monster::operator=(std::move(other));
        this->attackName = std::move(other.attackName);
        return *this;
    }
    const std::string &GetAttackName() const override
    {
        return this->attackName;
    }

    std::optional<std::string> GetDropItem() override;
};

/**
 * [M7] Dragon 클래스
 * 보스급 몬스터입니다. HP는 500이며, 'Fire Breath' 공격을 사용합니다.
 * 처치 시 'Dragon Heart'를 드롭합니다.
 */
class Dragon : public Monster
{
    // (필요한 멤버 및 생성자 선언)
    std::string attackName;

  public:
    Dragon() : Monster{500}, attackName{"Fire Breath"}
    {
    }
    virtual ~Dragon()
    {
    }
    Dragon(Dragon &other) : Monster{other}, attackName{other.attackName}
    {
    }
    Dragon(Dragon &&other)
        // [Review] rvalue 참조라도 이름이 있으면 lvalue이므로 std::move 필수
        : Monster{std::move(other)}, attackName{std::move(other.attackName)}
    {
    }
    Dragon &operator=(Dragon &other)
    {
        if (this == &other) {
            return *this;
        }
        Monster::operator=(other);
        this->attackName = other.attackName;
        return *this;
    }
    Dragon &operator=(Dragon &&other)
    {
        if (this == &other) {
            return *this;
        }
        // [Review] 이동 대입이므로 부모에게도 std::move로 전달
        Monster::operator=(std::move(other));
        this->attackName = std::move(other.attackName);
        return *this;
    }
    const std::string &GetAttackName() const override
    {
        return this->attackName;
    }
    std::optional<std::string> GetDropItem() override;
};
