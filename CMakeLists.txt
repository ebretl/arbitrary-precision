cmake_minimum_required(VERSION 3.10)
project(arbitrary_precision)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_library(arbitrary_precision UnsignedInteger.cpp Integer.cpp Float.cpp)

add_executable(main main.cpp)
target_link_libraries(main arbitrary_precision pthread)

set_property(TARGET main arbitrary_precision PROPERTY COMPILE_FLAGS -O3)
