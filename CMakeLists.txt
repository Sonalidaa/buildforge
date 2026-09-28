cmake_minimum_required(VERSION 3.16)
project(BuildForge CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pthread -O2")

include_directories(include)

add_executable(buildforge
    src/main.cpp
    src/ThreadPool.cpp
    src/BuildGraph.cpp
    src/Executor.cpp
)
