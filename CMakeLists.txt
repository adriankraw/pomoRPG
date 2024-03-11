cmake_minimum_required(VERSION 3.13)
project(pomoRPG)
set(CMAKE_CXX_STANDARD 17)

message(CMAKE_CURRENT_LIST_DIR="${CMAKE_CURRENT_LIST_DIR}")

set(SOURCE_FILES ${CMAKE_CURRENT_LIST_DIR}/main.cpp)

add_executable(pomoRPG ${SOURCE_FILES})
