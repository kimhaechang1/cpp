cmake_minimum_required(VERSION 3.10)
project(Module12_Basics)

set(CMAKE_CXX_STANDARD 17)

# Winsock2 Library Linkage
add_executable(practice_winsock_setup practice_winsock_setup.cpp)
target_link_libraries(practice_winsock_setup ws2_32)
