cmake_minimum_required(VERSION 3.10)

set(This Algebra)

set(Sources
    # test_SIMD.cpp
    # test_solver_jacobi.cpp
    # test_linear_polynomial.cpp
    test_multi_array.cpp
    # test_matrix_sparse.cpp
    test_number.cpp
    # test_interp.cpp
    # test_rational.cpp
)

find_package(Armadillo)
if(APPLE)
    find_library(ACCELERATE_LIBRARY Accelerate)
elseif(UNIX AND NOT APPLE)
    message("Running on Ubuntu")
else()
    message(FATAL_ERROR "This project only supports macOS!")
endif()


add_executable(${This} ${Sources})

target_include_directories(${This} PRIVATE ${ARMADILLO_INCLUDE_DIRS})
# target_include_directories(${This} PRIVATE ${ACCELERATE_INCLUDE_DIRS})
# target_link_libraries(${This} PRIVATE ${ARMADILLO_LIBRARIES})

# target_link_libraries(${This} PUBLIC
#     gtest_main
#     OpenMP::OpenMP_CXX
# )
if(APPLE)
  include_directories("/opt/homebrew/opt/libomp/include/")
  link_directories("/opt/homebrew/opt/libomp/lib/")
  target_link_directories(${This} PUBLIC "/opt/homebrew/opt/libomp/lib/")
endif()
if(APPLE)
    target_link_libraries(${This} gtest_main omp ${ARMADILLO_LIBRARIES})
elseif(UNIX AND NOT APPLE)
    target_link_libraries(${This} gtest_main OpenMP::OpenMP_CXX ${ARMADILLO_LIBRARIES})
endif()
add_test(
    NAME ${This}
    COMMAND ${This}
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)
