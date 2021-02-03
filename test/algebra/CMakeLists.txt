cmake_minimum_required(VERSION 2.8)

set(This Algebra)

set(Sources
    test_solver_jacobi.cpp
    test_multi_array.cpp
    test_linear_polynomial.cpp
    test_matrix_sparse.cpp
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
