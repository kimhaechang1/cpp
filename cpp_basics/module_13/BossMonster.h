#pragma once

#include <shared_mutex>
class BossMonster
{
private:
    // [멤버 변수: HP]
    // 무결성을 위해 외부 접근을 금지하고 객체 자체적으로만 통제합니다. (시작 체력 10000)
    int _hp = 10000;

    // [멤버 변수: Lock]
    // 여러 스레드가 동시에 이 보스를 때릴 수 있으므로, 생존 수치 접근 시 충돌 방지 자물쇠가 필수입니다.
    std::shared_mutex _lock;

public:
    // [메서드: TakeDamage]
    // 데미지만큼 생존 수치를 차감합니다. 0 미만으로 떨어지지 않습니다.
    void TakeDamage(int dmg);
    // [메서드: GetHp]
    // 현재 생존 수치를 안전하게 반환합니다.
    const int GetHp() const;

    // [메서드: IsDead]
    // 생존 수치가 0인지 확인합니다.
    const bool IsDead() const;
};
