cmake_minimum_required(VERSION 3.10)
project(Module13_Basics)

set(CMAKE_CXX_STANDARD 23)

find_package(GTest CONFIG REQUIRED)

add_executable(test_thread_pool 
    test_thread_pool.cpp 
    ThreadPool.cpp
)
add_executable(
    practice_select_server practice_select_server.cpp
)
add_executable(
    practice_hybrid_server practice_hybrid_server.cpp ThreadPool.cpp
)

target_link_libraries(test_thread_pool GTest::gtest GTest::gtest_main)
target_link_libraries(practice_select_server ws2_32)

add_executable(practice_select_client practice_select_client.cpp)
target_link_libraries(practice_select_client ws2_32)

add_executable(practice_writefds_server practice_writefds_server.cpp)
target_link_libraries(practice_writefds_server ws2_32)
add_executable(practice_writefds_client practice_writefds_client.cpp)
target_link_libraries(practice_writefds_client ws2_32)
target_link_libraries(practice_hybrid_server ws2_32)

add_executable(test_raid_server 
    test_raid_server.cpp 
    SessionManager.cpp 
    CommandHandler.cpp 
    BossMonster.cpp
)
target_link_libraries(test_raid_server GTest::gtest GTest::gtest_main ws2_32)