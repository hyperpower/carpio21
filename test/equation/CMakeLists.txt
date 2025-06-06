cmake_minimum_required(VERSION 3.3)

set(This Equation)

set(Sources
    # test_equation_base.cpp
    # test_poisson.cpp
    # test_laplacian.cpp
    # test_laplacian_vertex.cpp
    test_poisson_vertex.cpp
    # test_advection.cpp
)

add_executable(${This} ${Sources})

find_package(Python COMPONENTS Interpreter Development)

IF(Python_FOUND)
  INCLUDE_DIRECTORIES("${Python_INCLUDE_DIRS}")
ELSE()
  MESSAGE(FATAL_ERROR "Unable to find PythonLibs.")
ENDIF()

target_link_libraries(${This} PUBLIC
    gtest_main
)

if(Python_FOUND)
  target_link_libraries(${This} PUBLIC ${Python_LIBRARIES})
ELSE()
  MESSAGE(FATAL_ERROR "Unable to link python libraries")
ENDIF()

add_test(
    NAME ${This}
    COMMAND ${This}
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)