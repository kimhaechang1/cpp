#include "BossMonster.h"
#include "CommandHandler.h"
#include "Session.h"
#include "SessionManager.h"
#include <gtest/gtest.h>
#include <string>
#include <thread>
#include <vector>
#include <winsock2.h>

// 이 테스트가 통과하도록 구현 파일들을 모두 완성하세요!

// ---------------------------------------------------------
// Suite 1: SessionManagerTest (세션 관리)
// ---------------------------------------------------------

TEST(SessionManagerTest, RegisterAndFindSession)
{
    SOCKET fakeSock = 100;
    SessionManager::GetInstance()->RegisterSession(fakeSock, "PlayerA", "127.0.0.1", 5000);

    auto sessionOpt = SessionManager::GetInstance()->FindSession(fakeSock);
    ASSERT_TRUE(sessionOpt.has_value());
    EXPECT_EQ(sessionOpt.value()->GetName(), "PlayerA");

    SessionManager::GetInstance()->RemoveSession(fakeSock);
}

TEST(SessionManagerTest, FindNonExistentSessionReturnsEmpty)
{
    auto sessionOpt = SessionManager::GetInstance()->FindSession(9999);
    EXPECT_FALSE(sessionOpt.has_value());
}

TEST(SessionManagerTest, RemoveSession)
{
    SOCKET fakeSock = 101;
    SessionManager::GetInstance()->RegisterSession(fakeSock, "PlayerB", "127.0.0.1", 5001);
    SessionManager::GetInstance()->RemoveSession(fakeSock);

    auto sessionOpt = SessionManager::GetInstance()->FindSession(fakeSock);
    EXPECT_FALSE(sessionOpt.has_value());
}

TEST(SessionManagerTest, SendQueuePushAndPop)
{
    SOCKET fakeSock = 102;
    SessionManager::GetInstance()->RegisterSession(fakeSock, "PlayerC", "127.0.0.1", 5002);
    auto session = SessionManager::GetInstance()->FindSession(fakeSock).value();

    session->PushToSendQueue("Hello");
    session->PushToSendQueue("World");

    auto pop1 = session->PopFromSendQueue();
    auto pop2 = session->PopFromSendQueue();
    auto pop3 = session->PopFromSendQueue();

    ASSERT_TRUE(pop1.has_value());
    EXPECT_EQ(pop1.value(), "Hello");

    ASSERT_TRUE(pop2.has_value());
    EXPECT_EQ(pop2.value(), "World");

    EXPECT_FALSE(pop3.has_value());

    SessionManager::GetInstance()->RemoveSession(fakeSock);
}

TEST(SessionManagerTest, SendQueueEmptyCheck)
{
    SOCKET fakeSock = 103;
    SessionManager::GetInstance()->RegisterSession(fakeSock, "PlayerD", "127.0.0.1", 5003);
    auto session = SessionManager::GetInstance()->FindSession(fakeSock).value();

    EXPECT_TRUE(SessionManager::GetInstance()->IsSendQueueEmpty(fakeSock));
    session->PushToSendQueue("Data");
    EXPECT_FALSE(SessionManager::GetInstance()->IsSendQueueEmpty(fakeSock));
    session->PopFromSendQueue();
    EXPECT_TRUE(SessionManager::GetInstance()->IsSendQueueEmpty(fakeSock));

    SessionManager::GetInstance()->RemoveSession(fakeSock);
}

// ---------------------------------------------------------
// Suite 2: BossMonsterTest (공유 자원 보호)
// ---------------------------------------------------------

TEST(BossMonsterTest, TakeDamageReducesHp)
{
    BossMonster boss; // 기본 체력 10000
    boss.TakeDamage(300);
    EXPECT_EQ(boss.GetHp(), 9700);
}

TEST(BossMonsterTest, HpNeverGoesBelowZero)
{
    BossMonster boss;
    boss.TakeDamage(15000);
    EXPECT_EQ(boss.GetHp(), 0);
}

TEST(BossMonsterTest, IsDeadAfterLethalDamage)
{
    BossMonster boss;
    boss.TakeDamage(10000);
    EXPECT_TRUE(boss.IsDead());
}

TEST(BossMonsterTest, ConcurrentDamageIsSafe)
{
    BossMonster boss;
    std::vector<std::thread> threads;

    // 10개의 스레드가 동시에 100의 데미지를 가함
    for (int i = 0; i < 10; ++i)
    {
        threads.push_back(std::thread([&boss]() { boss.TakeDamage(100); }));
    }

    for (auto &t : threads)
    {
        t.join();
    }

    // 정확히 1000 데미지가 차감되었어야 함 (Data Race 방어)
    EXPECT_EQ(boss.GetHp(), 9000);
}

// ---------------------------------------------------------
// Suite 3: CommandHandlerTest (다형적 명령 처리)
// ---------------------------------------------------------

TEST(CommandHandlerTest, AttackHandlerReducesBossHp)
{
    BossMonster boss;
    AttackHandler attackHandler(&boss);

    SOCKET fakeSock = 200;
    SessionManager::GetInstance()->RegisterSession(fakeSock, "Attacker", "1.1.1.1", 1111);
    auto session = SessionManager::GetInstance()->FindSession(fakeSock).value();

    CommandPacket pkt{CommandType::Attack, 500};
    std::string response = attackHandler.Execute(pkt, session);

    EXPECT_EQ(boss.GetHp(), 9500);
    EXPECT_NE(response.find("500"), std::string::npos); // 응답에 500 데미지 정보 포함 확인

    SessionManager::GetInstance()->RemoveSession(fakeSock);
}

TEST(CommandHandlerTest, HealHandlerRestoresSessionHp)
{
    HealHandler healHandler;

    SOCKET fakeSock = 201;
    SessionManager::GetInstance()->RegisterSession(fakeSock, "Healer", "1.1.1.1", 1111);
    auto session = SessionManager::GetInstance()->FindSession(fakeSock).value();

    session->SetHp(70); // 테스트 세팅을 위해 70으로 깎음
    CommandPacket pkt{CommandType::Heal, 50};
    healHandler.Execute(pkt, session);

    EXPECT_EQ(session->GetHp(), 100); // 120이 아니라 최대치인 100으로 캡(Cap) 씌워짐

    SessionManager::GetInstance()->RemoveSession(fakeSock);
}

TEST(CommandHandlerTest, StatusHandlerReturnsBossAndPlayerInfo)
{
    BossMonster boss;
    StatusHandler statusHandler(&boss);

    SOCKET fakeSock = 202;
    SessionManager::GetInstance()->RegisterSession(fakeSock, "Observer", "1.1.1.1", 1111);
    auto session = SessionManager::GetInstance()->FindSession(fakeSock).value();

    CommandPacket pkt{CommandType::Status, 0};
    std::string response = statusHandler.Execute(pkt, session);

    EXPECT_NE(response.find("10000"), std::string::npos);    // 보스 체력 포함 확인
    EXPECT_NE(response.find("Observer"), std::string::npos); // 유저 이름 포함 확인

    SessionManager::GetInstance()->RemoveSession(fakeSock);
}

// ---------------------------------------------------------
// Suite 4: IntegrationFlowTest (E2E 통합 시나리오)
// ---------------------------------------------------------

TEST(IntegrationFlowTest, FullRaidScenario)
{
    BossMonster boss;
    AttackHandler attackHandler(&boss);
    HealHandler healHandler;
    StatusHandler statusHandler(&boss);

    SOCKET sockA = 300, sockB = 301;
    SessionManager::GetInstance()->RegisterSession(sockA, "PlayerA", "IP_A", 100);
    SessionManager::GetInstance()->RegisterSession(sockB, "PlayerB", "IP_B", 200);

    auto sessionA = SessionManager::GetInstance()->FindSession(sockA).value();
    auto sessionB = SessionManager::GetInstance()->FindSession(sockB).value();

    // 1. A가 10000 데미지로 공격하여 보스 처치
    CommandPacket attackPkt{CommandType::Attack, 10000};
    sessionA->PushToSendQueue(attackHandler.Execute(attackPkt, sessionA));

    EXPECT_TRUE(boss.IsDead());

    // 2. B가 스스로에게 20 힐을 시전 (기본 HP가 100이므로 변화 없어야 함)
    CommandPacket healPkt{CommandType::Heal, 20};
    sessionB->PushToSendQueue(healHandler.Execute(healPkt, sessionB));

    // 3. A가 큐에서 응답 꺼내기 (Main Thread의 writefds 비동기 Send 시뮬레이션)
    auto aRes = sessionA->PopFromSendQueue();
    ASSERT_TRUE(aRes.has_value());
    EXPECT_NE(aRes.value().find("10000"), std::string::npos);

    // 4. Session 정리 및 제거 확인
    SessionManager::GetInstance()->RemoveSession(sockA);
    SessionManager::GetInstance()->RemoveSession(sockB);

    EXPECT_FALSE(SessionManager::GetInstance()->FindSession(sockA).has_value());
}
