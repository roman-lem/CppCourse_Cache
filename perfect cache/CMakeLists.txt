cmake_minimum_required (VERSION 3.11)
project (2QCache LANGUAGES CXX)

set(SRC_MAIN ./PerfCache.cpp)

include_directories("include/")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wextra -O3")

add_executable (PerfCache ${SRC_MAIN})
target_sources(PerfCache PRIVATE ${SOURCE_EXE})
