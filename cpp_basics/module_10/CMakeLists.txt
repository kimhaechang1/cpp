cmake_minimum_required(VERSION 3.10)
project(HelloCMake)
set(CMAKE_CXX_STANDARD 23)

# 1. 라이브러리 찾기
find_package(fmt CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED) # GTest 추가

# (참고) 제품용 실행 파일
add_executable(App practice_cumulative_10_1.cpp)
target_link_libraries(App fmt::fmt nlohmann_json::nlohmann_json)

# (핵심) 테스트용 실행 파일 생성
# Calculator.cpp를 함께 빌드하여 로직을 공유합니다.
add_executable(TestApp practice_gtest.cpp Calculator.cpp)
target_link_libraries(TestApp 
    GTest::gtest 
    GTest::gtest_main
)

# [M10 Cumulative Practice 2] 퀘스트 시스템 TDD 테스트 타겟 추가
add_executable(QuestJournalTest test_quest_journal.cpp QuestJournal.cpp)
target_link_libraries(QuestJournalTest 
    GTest::gtest 
    GTest::gtest_main
)

# [M10 Cumulative Practice 3] 커밋 분석기 TDD 테스트 타겟 추가
add_executable(CommitAnalyzerTest test_commit_analyzer.cpp CommitAnalyzer.cpp Commit.cpp)
target_link_libraries(CommitAnalyzerTest 
    GTest::gtest 
    GTest::gtest_main
)

