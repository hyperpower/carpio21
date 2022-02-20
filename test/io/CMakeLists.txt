cmake_minimum_required(VERSION 3.0)

set(This IO)

set(Sources
    # test_csv.cpp
    test_textfile.cpp
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
