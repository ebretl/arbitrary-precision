add_library(arbitrary_precision
        UnsignedInteger.cpp
        Integer.cpp
        Float.cpp)
target_link_libraries(arbitrary_precision pthread)

set_property(TARGET arbitrary_precision PROPERTY COMPILE_FLAGS "-O3 -Wall -Wpedantic")
