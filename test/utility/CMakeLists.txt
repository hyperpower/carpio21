cmake_minimum_required(VERSION 3.0)

set(This Utility)

set(Sources
    test_tree.cpp
    # test_clock.cpp
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
