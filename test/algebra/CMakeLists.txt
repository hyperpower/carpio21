cmake_minimum_required(VERSION 3.0)

set(This Algebra)

set(Sources
    # test_solver_jacobi.cpp
    # test_linear_polynomial.cpp
    test_multi_array.cpp
    test_matrix_sparse.cpp
    test_number.cpp
    # test_rational.cpp
)

add_executable(${This} ${Sources})

target_link_libraries(${This} PUBLIC
    gtest_main
    OpenMP::OpenMP_CXX
)

add_test(
    NAME ${This}
    COMMAND ${This}
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)
