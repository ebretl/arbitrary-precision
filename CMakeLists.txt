cmake_minimum_required(VERSION 3.2)
project(simple_bignum VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_subdirectory(src)
add_subdirectory(examples)
