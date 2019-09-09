cmake_minimum_required(VERSION 3.2)
project(exact VERSION 0.2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_library(exact
        src/integer.cpp
        src/non_negative_integer.cpp
)
set_property(TARGET exact PROPERTY COMPILE_FLAGS "-O0 -Wall -Wpedantic")

add_executable(run_tests test/tests.cpp)
target_link_libraries(run_tests exact)

#add_library(arbitrary_precision
#        UnsignedInteger.cpp
#        Integer.cpp
#        )
#target_link_libraries(arbitrary_precision pthread)
