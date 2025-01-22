cmake_minimum_required(VERSION 3.22)

set(This Structure)

include_directories(${PROJECT_SOURCE_DIR}/include)

set(Sources
    # test_bc_1d.cpp
    # test_sgrid_io.cpp
    # test_sorder.cpp
    test_sfield.cpp
    # test_sfield_face.cpp
    # test_sgrid.cpp
    # test_sinterpolate.cpp
    # test_sint_laplacian.cpp
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
