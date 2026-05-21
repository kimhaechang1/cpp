#include "BossMonster.h"
#include <mutex>
#include <shared_mutex>

// BossMonster의 멤버 함수들을 구현하세요.
void BossMonster::TakeDamage(int dmg)
{
    std::unique_lock<std::shared_mutex> locked(this->_lock);
    if (this->_hp - dmg <= 0)
    {
        this->_hp = 0;
        return;
    }
    this->_hp -= dmg;
}

const int BossMonster::GetHp() const
{

    return this->_hp;
}

const bool BossMonster::IsDead() const
{
    return this->_hp == 0;
}