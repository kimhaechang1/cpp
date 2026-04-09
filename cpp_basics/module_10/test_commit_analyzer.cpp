#include <gtest/gtest.h>
#include "CommitAnalyzer.h"
#include "Commit.h"
#include <memory>
#include <sstream>

// 카운터 리셋을 위한 Test 환경
class CommitTest : public ::testing::Test {
protected:
    void SetUp() override {
        Commit::ResetCounter();
    }
};

// 1. FeatureCommit 생성 및 GetTypeLabel 검증
TEST_F(CommitTest, FeatureCommitCreation) {
    FeatureCommit c("abc12345", "Add player", "Kim");
    EXPECT_EQ(c.GetTypeLabel(), "Feature");
    EXPECT_EQ(c.GetHash(), "abc12345");
    // GetType()은 enum class 반환
    EXPECT_EQ(static_cast<int>(c.GetType()), static_cast<int>(CommitType::Feature));
}

// 2. BugfixCommit 생성 검증
TEST_F(CommitTest, BugfixCommitCreation) {
    BugfixCommit c("def67890", "Fix crash", "Lee");
    EXPECT_EQ(c.GetTypeLabel(), "Bugfix");
    EXPECT_EQ(static_cast<int>(c.GetType()), static_cast<int>(CommitType::Bugfix));
}

// 3. operator<< 스트림 출력 검증 ([타입명] 해시: 메시지)
TEST_F(CommitTest, StreamOutput) {
    FeatureCommit c("abc12345", "Add player", "Kim");
    std::ostringstream oss;
    oss << c;
    EXPECT_EQ(oss.str(), "[Feature] abc12345: Add player");
}

// 4. static 멤버를 통한 모든 자식 인스턴스들의 공통 횟수 추적
TEST_F(CommitTest, StaticCommitCounter) {
    FeatureCommit c1("h1", "m1", "a1");
    FeatureCommit c2("h2", "m2", "a2");
    BugfixCommit c3("h3", "m3", "a3");
    EXPECT_EQ(Commit::GetCounter(), 3);
}

// 5. 타입에 따른 우선순위 분기 (switch 사용)
TEST_F(CommitTest, PriorityByType) {
    FeatureCommit c1("h1", "m1", "a1");
    BugfixCommit c2("h2", "m2", "a2");
    // Feature = 1, Bugfix = 3 이 길 기대
    EXPECT_EQ(c1.GetPriority(), 1);
    EXPECT_EQ(c2.GetPriority(), 3);
}

// Analyzer 검증 환경 설정
class AnalyzerTest : public ::testing::Test {
protected:
    void SetUp() override {
        Commit::ResetCounter();
    }
};

// 6. 소유권 이동 및 해시를 통한 빠른 검색 (std::map + std::optional)
TEST_F(AnalyzerTest, RegisterAndFindCommit) {
    CommitAnalyzer analyzer;
    auto ptr = std::make_unique<FeatureCommit>("abc12345", "Add player", "Kim");
    analyzer.RegisterCommit(std::move(ptr));

    auto result = analyzer.FindByHash("abc12345");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value()->GetMessage(), "Add player"); 
}

// 7. 없는 데이터 요청 시 에러 없이 빈 상자 반환
TEST_F(AnalyzerTest, FindReturnsEmptyOnMiss) {
    CommitAnalyzer analyzer;
    auto result = analyzer.FindByHash("nonexistent");
    EXPECT_FALSE(result.has_value());
}

// 8. 7일 고정 크기 배열을 통한 빠른 읽기/쓰기 (std::array)
TEST_F(AnalyzerTest, WeeklyStatsArray) {
    CommitAnalyzer analyzer;
    analyzer.RecordCommitOnDay(0, 3);
    
    // 원본 데이터를 읽기 전용 참조로 받아오길 기대
    const auto& stats = analyzer.GetWeeklyStats();
    EXPECT_EQ(stats[0], 3);
}

// 9. 원본의 소유권은 넘보지 않고 특정 구역만 바라보는 시야 제한 (std::span)
TEST_F(AnalyzerTest, WeekdayStatsSpanView) {
    CommitAnalyzer analyzer;
    analyzer.RecordCommitOnDay(0, 1); // 월
    analyzer.RecordCommitOnDay(1, 2);
    analyzer.RecordCommitOnDay(2, 3);
    analyzer.RecordCommitOnDay(3, 4);
    analyzer.RecordCommitOnDay(4, 5); // 금
    analyzer.RecordCommitOnDay(5, 6); // 토
    analyzer.RecordCommitOnDay(6, 7); // 일

    auto spanView = analyzer.GetWeekdayStats();
    
    // 5일치 평일 데이터 묶음만 대여해 주길 기대.
    EXPECT_EQ(spanView.size(), 5);
    EXPECT_EQ(spanView[0], 1);
    EXPECT_EQ(spanView[4], 5);
}

// 10. 일회용 함수(람다)를 통한 유연한 필터링
TEST_F(AnalyzerTest, FilterByTypeWithLambda) {
    CommitAnalyzer analyzer;
    analyzer.RegisterCommit(std::make_unique<FeatureCommit>("h1", "m1", "a1"));
    analyzer.RegisterCommit(std::make_unique<FeatureCommit>("h2", "m2", "a2"));
    analyzer.RegisterCommit(std::make_unique<BugfixCommit>("h3", "m3", "a3"));

    // Feature 타입만 걸러내는 조건식 주입
    auto features = analyzer.FilterByType([](const Commit* c) {
        return c->GetType() == CommitType::Feature;
    });

    EXPECT_EQ(features.size(), 2);
}

// 11. 총 개수 검증 (간단한 디버그)
TEST_F(AnalyzerTest, GetTotalCommitCount) {
    CommitAnalyzer analyzer;
    EXPECT_EQ(analyzer.GetTotalCount(), 0);
    analyzer.RegisterCommit(std::make_unique<FeatureCommit>("h1", "m1", "a1"));
    analyzer.RegisterCommit(std::make_unique<BugfixCommit>("h2", "m2", "a2"));
    EXPECT_EQ(analyzer.GetTotalCount(), 2);
}
