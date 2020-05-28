cmake_minimum_required(VERSION 3.2)
project(exact VERSION 0.2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_library(exact
        src/integer.cpp
        src/unsigned_integer.cpp
        src/rational.cpp
        src/positive_integer.cpp
        )
set_property(TARGET exact PROPERTY COMPILE_FLAGS "-O2 -g -Wall -Wpedantic")

add_executable(run_tests
        test/test_main.cpp
        test/test_non_negative_and_normal_ints.cpp
        test/test_rational.cpp
        test/test_positive_int.cpp
        )
target_link_libraries(run_tests exact)
set_property(TARGET run_tests PROPERTY COMPILE_FLAGS "-Og")

add_subdirectory(examples)
