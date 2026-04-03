cmake_minimum_required(VERSION 3.10)
project(HelloCMake)
set(CMAKE_CXX_STANDARD 23)

# 1. fmt 라이브러리 찾기
find_package(fmt CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

add_executable(App practice_cumulative_10_1.cpp)

# 2. App 타겟에 fmt 라이브러리 연결
target_link_libraries(App fmt::fmt nlohmann_json::nlohmann_json)

