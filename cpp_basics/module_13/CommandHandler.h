#pragma once
#include "BossMonster.h"
#include "Session.h"
#include <string>

// CommandType 정의 (패킷에서 사용)
enum class CommandType { Attack, Heal, Status };

// CommandPacket 구조체 (M1, Uniform Initialization 대상)
struct CommandPacket
{
    CommandType type;
    int payload; // 데미지량 또는 힐량
};

// [ICommandHandler]
// 이 클래스는 직접 인스턴스화가 불가능한 설계도(인터페이스)입니다.
// 파생 클래스가 반드시 구현해야 할 순수한 실행 메서드를 선언합니다.
class ICommandHandler
{
public:
    // 가상 소멸자를 잊지 마세요!
    // 가상 소멸자를 잊지 말아야하는 이유, 스택 메모리상에 업케스팅이 이뤄지고 그것에 따라 소멸자를 호출하는 대상이
    // 부모에게 직접적으로 호출되어 고아 메모리가 남을 수 있다. 그리고 인터페이스는 물론 외부에서 인스턴스 생성은 할 수
    // 없지만, 이 클래스를 상속받는 클래스에서는 생성자를 호출할 수 있다. 따라서 소멸자를 안하는것이 아닌 가상소멸자로
    // 진행한다.
    virtual ~ICommandHandler() = default;

    // [메서드: Execute]
    // 파생 클래스마다 고유한 게임 로직을 수행하고, 클라이언트에게 보낼 응답 문자열을 반환합니다.
    // 인자로 const CommandPacket& 와 Session* 를 받습니다.
    virtual std::string Execute(const CommandPacket &packet, Session *session) = 0;
};

// [AttackHandler]
// 보스 몬스터의 생존 수치를 차감하는 공격 로직을 수행합니다.
class AttackHandler : public ICommandHandler
{
private:
    BossMonster* _boss;

public:
    AttackHandler(BossMonster* boss) : _boss{boss} {}
    std::string Execute(const CommandPacket& packet, Session* session) override;
};

class HealHandler : public ICommandHandler
{
public:
    HealHandler() {}
    std::string Execute(const CommandPacket& packet, Session* session) override;
};

class StatusHandler : public ICommandHandler
{
private:
    BossMonster* _boss;

public:
    StatusHandler(BossMonster* boss) : _boss{boss} {}
    std::string Execute(const CommandPacket& packet, Session* session) override;
};
