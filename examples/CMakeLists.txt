add_executable(calc_pi calc_pi.cpp)
target_link_libraries(calc_pi arbitrary_precision)

add_executable(test_program test.cpp)
target_link_libraries(test_program arbitrary_precision)

set_property(TARGET calc_pi test_program PROPERTY COMPILE_FLAGS "-O3 -Wall -Wpedantic")


add_executable(speed_test speed_test.cpp)
target_link_libraries(speed_test arbitrary_precision exact)
set_property(TARGET speed_test PROPERTY COMPILE_FLAGS "-O3 -Wall -Wpedantic")
