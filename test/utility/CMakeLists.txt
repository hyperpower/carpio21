cmake_minimum_required(VERSION 3.20)

set(This Utility)

set(Sources
    test_tree.cpp
    test_clock.cpp
    test_any.cpp
    test_big_int.cpp
)

add_executable(${This} ${Sources})

target_link_libraries(${This} PUBLIC
    gtest_main
)

add_test(
    NAME ${This}
    COMMAND ${This}
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)
