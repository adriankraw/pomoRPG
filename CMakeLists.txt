cmake_minimum_required(VERSION 3.13)
project(pomoRPG)
set(CMAKE_CXX_STANDARD 17)

message(CMAKE_CURRENT_LIST_DIR="${CMAKE_CURRENT_LIST_DIR}")

file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/main.cpp")

add_executable(pomoRPG ${SOURCES})

target_include_directories(pomoRPG PRIVATE "./extern/")

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/saveFile.txt
	DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
