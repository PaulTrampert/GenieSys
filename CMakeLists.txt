cmake_minimum_required(VERSION 3.17)
project(GenieSys)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)


add_subdirectory(src)
add_subdirectory(app)
add_subdirectory(test)