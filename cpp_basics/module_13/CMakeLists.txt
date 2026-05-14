cmake_minimum_required(VERSION 3.10)
project(Module13_Basics)

set(CMAKE_CXX_STANDARD 23)

find_package(GTest CONFIG REQUIRED)

add_executable(test_thread_pool 
    test_thread_pool.cpp 
    ThreadPool.cpp
)

target_link_libraries(test_thread_pool GTest::gtest GTest::gtest_main)
