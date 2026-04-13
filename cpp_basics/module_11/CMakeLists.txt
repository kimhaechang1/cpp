cmake_minimum_required(VERSION 3.10)
project(Module11Concurrency)

# C++23 표준 사용
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. 외부 라이브러리 찾기 (vcpkg 연동)
find_package(fmt CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)
find_package(Threads REQUIRED)

# 2. 테스트 타겟 추가: DungeonRaidTest
add_executable(DungeonRaidTest 
    test_dungeon_raid.cpp 
    Monster.cpp 
    DungeonRaid.cpp
)

# 3. 라이브러리 링크
target_link_libraries(DungeonRaidTest 
    fmt::fmt 
    GTest::gtest_main
    GTest::gtest
    Threads::Threads
)

# 4. (참고) 개별 실습 파일들 (연습용 단독 타겟)
# add_executable(PracticeMutex practice_mutex.cpp)
# add_executable(PracticeJThread practice_jthread.cpp)
