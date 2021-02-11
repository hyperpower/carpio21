cmake_minimum_required(VERSION 2.8)

set(This Geometry)

set(Sources
    test_bezier.cpp
    # test_box.cpp
    test_point.cpp
    test_point_chain.cpp
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
