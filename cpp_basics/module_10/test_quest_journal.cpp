#include <gtest/gtest.h>
#include "QuestJournal.h"
#include "Quest.h"
#include <memory>
#include <vector>

/**
 * [MODULE 10] Cumulative Practice 2 (GTest TDD)
 * 
 * 당신의 과제는 이 테스트 파일이 성공적으로 컴파일되고 통과되도록 
 * Quest.h, QuestJournal.h, QuestJournal.cpp를 구현하는 것입니다.
 * 
 * 팁: 테스트 코드가 호출하는 '함수 이름', '매개변수', '반환 타입'을 눈여겨보세요.
 */

// 1. 퀘스트 추가 및 개수 확인 테스트
TEST(QuestJournalSuite, AddAndCount) {
    QuestJournal journal;
    
    // [M9/M10] unique_ptr을 소유권 이전(move) 방식으로 전달해야 함을 추론하세요.
    // [M7] MainQuest와 SideQuest는 Quest를 상속받은 구체 클래스여야 합니다.
    auto mainPtr = std::make_unique<MainQuest>("Ancient Dragon", 1000);
    auto sidePtr = std::make_unique<SideQuest>("Collect Herbs", 100);
    
    // AddQuest는 std::expected (또는 유사한 안전상자)를 반환해야 합니다.
    EXPECT_TRUE(journal.AddQuest(std::move(mainPtr)).has_value());
    EXPECT_TRUE(journal.AddQuest(std::move(sidePtr)).has_value());
    
    // 현재 등록된 퀘스트 총 개수 확인
    EXPECT_EQ(journal.GetQuestCount(), 2);
}

// 2. 중복 퀘스트 등록 에러 테스트
TEST(QuestJournalSuite, DuplicateQuestError) {
    QuestJournal journal;
    
    journal.AddQuest(std::make_unique<MainQuest>("Find the Sword", 500));
    
    // [M6] operator== 을 통해 이름이 같은지 확인하고, 중복이면 에러를 반환해야 합니다.
    auto result = journal.AddQuest(std::make_unique<SideQuest>("Find the Sword", 100));
    
    EXPECT_FALSE(result.has_value()); // 에러 발생 기대
    EXPECT_EQ(journal.GetQuestCount(), 1); // 중복은 추가되지 않아야 함
}

// 3. 이름으로 퀘스트 검색 테스트
TEST(QuestJournalSuite, FindQuest) {
    QuestJournal journal;
    journal.AddQuest(std::make_unique<MainQuest>("Portal Secret", 300));
    
    // [M9] FindQuest는 존재할 경우 const Quest* (또는 참조)를 반환해야 합니다.
    auto result = journal.FindQuest("Portal Secret");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value()->GetName(), "Portal Secret");
    
    // 존재하지 않는 퀘스트 검색
    auto failResult = journal.FindQuest("Ghost Town");
    EXPECT_FALSE(failResult.has_value());
}

// 4. 상태 전이(State Transition) 테스트
TEST(QuestJournalSuite, StateTransition) {
    QuestJournal journal;
    journal.AddQuest(std::make_unique<MainQuest>("Tutorial", 50));
    
    auto questResult = journal.FindQuest("Tutorial");
    ASSERT_TRUE(questResult.has_value());
    
    // [M1] 초기 상태는 Inactive(대기) 등이어야 함
    // (여기서 enum class 내부 명칭을 Inactive 등으로 구현했다고 가정)
    // EXPECT_EQ(questResult.value()->GetState(), QuestState::Inactive);
    
    // [M3/M2] AdvanceState를 통해 상태가 전이되어야 함 (Inactive -> Active -> Completed)
    journal.AdvanceState("Tutorial"); // Inactive -> Active
    
    // 상태가 실제 원본에서 변경되었는지 확인
    auto activeResult = journal.FindQuest("Tutorial");
    // EXPECT_EQ(activeResult.value()->GetState(), QuestState::Active);

    journal.AdvanceState("Tutorial"); // Active -> Completed
    // EXPECT_EQ(activeResult.value()->GetState(), QuestState::Completed);
}

// 5. 특정 상태의 퀘스트 필터링 테스트
TEST(QuestJournalSuite, FilterByState) {
    QuestJournal journal;
    journal.AddQuest(std::make_unique<MainQuest>("MQ1", 100));
    journal.AddQuest(std::make_unique<SideQuest>("SQ1", 50));
    
    journal.AdvanceState("MQ1"); // Active 상태로 변경
    
    // [M8] 람다를 사용하여 특정 조건(Active 상태)의 퀘스트 목록을 추출
    // 반환 타입은 std::vector<const Quest*> 등을 권장합니다.
    // (람다 내부에서는 각 퀘스트의 상태를 체크하는 코드가 들어갑니다)
    // auto activeQuests = journal.FilterByState(...);
    
    // EXPECT_EQ(activeQuests.size(), 1);
}

// 6. 다형성(Polymorphism) 보상 계산 테스트
TEST(QuestJournalSuite, PolymorphicReward) {
    QuestJournal journal;
    journal.AddQuest(std::make_unique<MainQuest>("Boss Raid", 500)); // 보상 2배 기대
    journal.AddQuest(std::make_unique<SideQuest>("Delivery", 100));  // 보상 1배 기대
    
    auto mq = journal.FindQuest("Boss Raid");
    auto sq = journal.FindQuest("Delivery");
    
    ASSERT_TRUE(mq.has_value());
    ASSERT_TRUE(sq.has_value());
    
    // [M7] 순수 가상 함수 GetReward()가 다형적으로 호출되어야 합니다.
    EXPECT_EQ(mq.value()->GetReward(), 1000); // 500 * 2
    EXPECT_EQ(sq.value()->GetReward(), 100);  // 100 * 1
}
