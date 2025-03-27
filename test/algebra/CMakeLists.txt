cmake_minimum_required(VERSION 3.0)

set(This Algebra)

set(Sources
    test_SIMD.cpp
    # test_solver_jacobi.cpp
    # test_linear_polynomial.cpp
    # test_multi_array.cpp
    # test_matrix_sparse.cpp
    # test_number.cpp
    # test_rational.cpp
)

add_executable(${This} ${Sources})

# target_link_libraries(${This} PUBLIC
#     gtest_main
#     OpenMP::OpenMP_CXX
# )
if(APPLE)
  include_directories("/opt/homebrew/opt/libomp/include/")
  link_directories("/opt/homebrew/opt/libomp/lib/")
  target_link_directories(${This} PUBLIC "/opt/homebrew/opt/libomp/lib/")
  target_link_libraries(${This} gtest_main omp)
endif()

add_test(
    NAME ${This}
    COMMAND ${This}
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)
