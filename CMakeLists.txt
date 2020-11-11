cmake_minimum_required(VERSION 3.16)
project(GenieSys)

set(CMAKE_CXX_STANDARD 20)

add_executable(GenieSys main.cpp bus.cpp m68000cpu.cpp)