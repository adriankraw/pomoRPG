cmake_minimum_required(VERSION 3.13)
project(pomoRPG)
set(CMAKE_CXX_STANDARD 17)

set(NETWORK false)

message(CMAKE_CURRENT_LIST_DIR="${CMAKE_CURRENT_LIST_DIR}")
add_compile_definitions(CMAKE_SYSTEM_NAME)
file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/main.cpp")

add_executable(pomoRPG ${SOURCES})
target_compile_options(pomoRPG PRIVATE -O3 -Wall -Wextra)

if(NETWORK)
	add_compile_definitions(NETWORK)
	target_include_directories(pomoRPG PRIVATE "./extern/")
endif()

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/saveFile.txt
	DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/start.pomo
	DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/timerList.txt
	DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/skilltree.txt
	DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
