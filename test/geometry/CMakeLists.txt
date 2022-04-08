cmake_minimum_required(VERSION 3.4)

set(This Geometry)

include_directories(${PROJECT_SOURCE_DIR}/include)

set(Sources
    # test_bezier.cpp
    # test_box.cpp
    test_box3.cpp
    # test_point.cpp
    # test_affine.cpp
    # test_point_chain.cpp
    # test_ts.cpp
    # test_tri_tri.cpp
    # test_seg_tri.cpp
    # test_predicate.cpp
)
find_package(Python COMPONENTS Interpreter Development)

IF(Python_FOUND)
  INCLUDE_DIRECTORIES("${Python_INCLUDE_DIRS}")
ELSE()
  MESSAGE(FATAL_ERROR "Unable to find PythonLibs.")
ENDIF()

add_executable(${This} ${Sources})

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
