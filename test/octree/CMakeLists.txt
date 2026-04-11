cmake_minimum_required(VERSION 3.22)

set(This Octree)

include_directories(${PROJECT_SOURCE_DIR}/include)

set(SRC_onde test_onode.cpp)

add_executable(${This} ${SRC_onde})

target_link_libraries(${This} PUBLIC
    gtest gtest_main
)

add_test(
    NAME ${This}
    COMMAND ${This} 
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)
