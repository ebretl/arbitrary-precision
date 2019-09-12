cmake_minimum_required(VERSION 3.2)
project(exact VERSION 0.2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_library(exact
        src/integer.cpp
        src/non_negative_integer.cpp
        )

add_executable(run_tests test/tests.cpp)
target_link_libraries(run_tests exact)
set_property(TARGET run_tests PROPERTY COMPILE_FLAGS "-O0")

add_library(arbitrary_precision
        src/UnsignedInteger.cpp
        src/Integer.cpp
        )
target_link_libraries(arbitrary_precision pthread)

set_property(TARGET exact arbitrary_precision PROPERTY COMPILE_FLAGS "-O3 -Wall -Wpedantic")

add_subdirectory(examples)
