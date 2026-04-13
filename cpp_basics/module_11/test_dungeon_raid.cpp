#include "DungeonRaid.h"
#include "Monster.h"
#include <gtest/gtest.h>
#include <memory>
#include <span>
#include <stop_token>
#include <thread>
#include <vector>

/**
 * [MODULE 11] Cumulative Practice: Dungeon Raid System (TDD)
 *
 * 당신의 과제는 이 테스트 파일이 성공적으로 컴파일되고 통과되도록
 * Monster.h/cpp, DungeonRaid.h/cpp를 구현하는 것입니다.
 *
 * 팁: 테스트 코드가 호출하는 '함수 이름', '매개변수', '반환 타입'을 눈여겨보고
 *     헤더 파일에 알맞은 시그니처를 역추론하세요.
 */

// ----------------------------------------------------------------------------
// Suite 1: MonsterSuite (다형성 & 소유권)
// ----------------------------------------------------------------------------

TEST(MonsterSuite, GoblinHpAndAttack)
{
    // [M9] unique_ptr로 관리되는 몬스터 생성
    std::unique_ptr<Monster> goblin = std::make_unique<Goblin>();

    // [M7] 부모 포인터로 접근해도 자식의 데이터와 로직이 작동해야 함
    EXPECT_EQ(goblin->GetHp(), 50);
    EXPECT_EQ(goblin->GetAttackName(), "Slash");
}

TEST(MonsterSuite, DragonHpAndAttack)
{
    std::unique_ptr<Monster> dragon = std::make_unique<Dragon>();

    EXPECT_EQ(dragon->GetHp(), 500);
    EXPECT_EQ(dragon->GetAttackName(), "Fire Breath");
}

TEST(MonsterSuite, BossDropOptional)
{
    std::unique_ptr<Monster> boss = std::make_unique<Dragon>();
    std::unique_ptr<Monster> mob = std::make_unique<Goblin>();

    // [M9] std::optional을 사용하여 드롭 아이템이 있을수도 없을수도 있음을 표현
    auto bossDrop = boss->GetDropItem();
    auto mobDrop = mob->GetDropItem();

    EXPECT_TRUE(bossDrop.has_value());
    EXPECT_EQ(bossDrop.value(), "Dragon Heart");

    EXPECT_FALSE(mobDrop.has_value());
}

TEST(MonsterSuite, SpanMonsterView)
{
    std::vector<std::unique_ptr<Monster>> monsters;
    monsters.push_back(std::make_unique<Goblin>());
    monsters.push_back(std::make_unique<Goblin>());
    monsters.push_back(std::make_unique<Dragon>());

    // [M4] std::span을 사용하여 원본 벡터의 소유권 없이 뷰(View)만 전달
    std::span<const std::unique_ptr<Monster>> monsterView = monsters;

    EXPECT_EQ(monsterView.size(), 3);
    EXPECT_EQ(monsterView[0]->GetAttackName(), "Slash");
    EXPECT_EQ(monsterView[2]->GetAttackName(), "Fire Breath");
}

// ----------------------------------------------------------------------------
// Suite 2: DungeonRaidSuite (스레드 안전성 & 자원 보호)
// ----------------------------------------------------------------------------

TEST(DungeonRaidSuite, SingleThreadRecord)
{
    DungeonRaid raid{1};             // 1층 탐험대 생성
    DungeonRaid::ResetGlobalKills(); // 초기화

    raid.RecordKill();
    raid.RecordKill();

    EXPECT_EQ(raid.GetKillCount(), 2);
    // [M6] static 필드는 모든 인스턴스가 공유함
    EXPECT_GE(DungeonRaid::GetTotalGlobalKills(), 2);
}

TEST(DungeonRaidSuite, MultiThreadMutexProtection)
{
    DungeonRaid raid{2};
    DungeonRaid::ResetGlobalKills();

    std::vector<std::jthread> raiders;

    // [M11] 10개의 스레드가 동시에 접근하여 경쟁 상태 유도
    for (int i = 0; i < 10; ++i) {
        // [M8] 람다 안에서 RecordKill을 100번씩 호출
        raiders.emplace_back([&raid]() {
            for (int j = 0; j < 100; ++j) {
                raid.RecordKill();
            }
        });
    }

    // [M11] jthread 소멸 시 자동 join 됨을 보장받지만,
    // 검증을 위해 스코프를 닫거나 벡터를 비워서 join을 명시적으로 유도
    raiders.clear();

    // [M11] Mutex와 lock_guard가 작동했다면 정확히 1000이 나와야 함
    EXPECT_EQ(raid.GetKillCount(), 1000);
    EXPECT_EQ(DungeonRaid::GetTotalGlobalKills(), 1000);
}

TEST(DungeonRaidSuite, FormatReport)
{
    DungeonRaid raid{3};
    for (int i = 0; i < 42; ++i)
        raid.RecordKill();

    // [M1] std::format으로 조립된 정교한 보고서 문자열 확인
    std::string report = raid.GetReport();

    EXPECT_NE(report.find("Floor 3"), std::string::npos);
    EXPECT_NE(report.find("42"), std::string::npos);
}

TEST(DungeonRaidSuite, FloorStateEnum)
{
    DungeonRaid raid{4};
    // [M1] enum class FloorState 검증
    EXPECT_EQ(raid.GetState(), FloorState::Idle);

    raid.RecordKill();
    EXPECT_EQ(raid.GetState(), FloorState::InProgress);
}

// ----------------------------------------------------------------------------
// Suite 3: ConcurrencySuite (jthread & stop_token)
// ----------------------------------------------------------------------------

TEST(ConcurrencySuite, JthreadAutoJoin)
{
    bool flag{false};

    {
        // [M11] std::jthread는 join()을 호출하지 않아도 파괴 시 자동 조인됨
        std::jthread worker([&flag]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            flag = true;
        });
    } // 여기서 worker 파괴 -> join() 발생

    // join()이 정상적으로 끝났으므로 flag는 반드시 true여야 함
    EXPECT_TRUE(flag);
}

TEST(ConcurrencySuite, StopTokenEvacuation)
{
    DungeonRaid raid{5};
    std::vector<std::unique_ptr<Monster>> monsters;

    // 100마리의 몬스터 준비
    for (int i = 0; i < 100; ++i) {
        monsters.push_back(std::make_unique<Goblin>());
    }

    {
        // [M11] stop_token을 지원하는 ExecuteRaid 함수를 별도 스레드에서 실행
        // (함수 시그니처에 stop_token이 첫 번째 인자로 있어야 함을 추론)
        std::jthread worker(ExecuteRaid,
                            std::span<const std::unique_ptr<Monster>>{monsters},
                            std::ref(raid));

        // 200ms 동안은 공략 진행
        std::this_thread::sleep_for(std::chrono::milliseconds(200));

        // [M11] 긴급 철수 명령 (request_stop)
        worker.request_stop();
    } // worker 조인 대기

    int totalKilled = raid.GetKillCount();

    // 몬스터 1마리당 50ms 소요되므로, 200ms면 약 3~5마리만 잡고 멈췄어야 함
    EXPECT_GT(totalKilled, 0);   // 한 마리 이상은 잡았을 것
    EXPECT_LT(totalKilled, 100); // 100마리 다 잡기 전에 멈췄을 것
}
