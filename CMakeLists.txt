cmake_minimum_required(VERSION 3.30)
project(OS_1)

set(CMAKE_CXX_STANDARD 14)

include_directories(shell/include)
include_directories(shell/src)

add_executable(OS_1
        shell/include/commands.hpp
        shell/include/shell.hpp
        shell/src/commands.cpp
        shell/src/shell.cpp)
