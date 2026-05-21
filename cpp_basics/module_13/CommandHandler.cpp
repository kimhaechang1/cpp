#include "CommandHandler.h"

// CommandHandler 파생 클래스들의 Execute 메서드를 구현하세요.
// 응답 문자열에는 테스트가 검증하는 정보("damage", "10000", 플레이어 이름 등)가 포함되어야 합니다.

std::string AttackHandler::Execute(const CommandPacket &packet, Session*session) 
{
    this->_boss->TakeDamage(packet.payload);
    return "["+std::string(session->GetName()) + "] 이 Boss 몬스터에 " + std::to_string(packet.payload) + " 만큼 damage 를 입혔습니다.";
}

std::string HealHandler::Execute(const CommandPacket &packet, Session *session) 
{
    int realHeal = 0;
    int resultHp = session->GetHp() + packet.payload;
    int gain = packet.payload;
    if (resultHp > 100) {
        resultHp = 100;
        gain = resultHp - session->GetHp();
    }
    
    session->SetHp(resultHp);
    return "["+ std::string(session->GetName()) + "] 이" + std::to_string(gain) + " 만큼 heal 을 하였습니다." ;
}

std::string StatusHandler::Execute(const CommandPacket &packet, Session *session) 
{
    return "[BossMonster]: hp=" + std::to_string(this->_boss->GetHp()) + ", [Session] name=" + std::string(session->GetName()) + ", hp=" + std::to_string(session->GetHp());
}