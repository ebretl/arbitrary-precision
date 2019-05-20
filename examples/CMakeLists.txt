add_executable(calc_pi calc_pi.cpp)
target_link_libraries(calc_pi arbitrary_precision pthread)

add_executable(test test.cpp)
target_link_libraries(test arbitrary_precision)

set_property(TARGET calc_pi test PROPERTY COMPILE_FLAGS "-O3 -Wall -Wpedantic")
