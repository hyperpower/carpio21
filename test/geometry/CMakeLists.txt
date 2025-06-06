cmake_minimum_required(VERSION 3.4)

set(This Geometry)
set(Bezier Bezier)

include_directories(${PROJECT_SOURCE_DIR}/include)

set(Sources
    # test_box.cpp
    # test_box3.cpp
    # test_point.cpp
    # test_affine.cpp
    # test_point_chain.cpp
    test_ts.cpp
    # test_tri_tri.cpp
    # test_seg_tri.cpp
    # test_predicate.cpp
    # test_polygon_partition.cpp
    # test_segment.cpp
    # test_ben_ott2.cpp
)
set(Sources_Bezier test_bezier.cpp)



find_package(Python COMPONENTS Interpreter Development)

find_package(CGAL)

IF(Python_FOUND)
  INCLUDE_DIRECTORIES("${Python_INCLUDE_DIRS}")
ELSE()
  MESSAGE(FATAL_ERROR "Unable to find PythonLibs.")
ENDIF()
 
add_executable(${This} ${Sources})
add_executable(${Bezier} ${Sources_Bezier})

target_link_libraries(${This} PUBLIC
    gtest_main
    CGAL::CGAL
)
target_link_libraries(${Bezier} PUBLIC
    gtest_main
)

if(Python_FOUND)
  # file(APPEND "./out.txt" Python_Lib : ${Python_LIBRARIES}\n)
  target_link_libraries(${This} PUBLIC ${Python_LIBRARIES})
  target_link_libraries(${Bezier} PUBLIC ${Python_LIBRARIES})
ELSE()
  MESSAGE(FATAL_ERROR "Unable to link python libraries")
ENDIF()

add_test(
    NAME ${This}
    COMMAND ${This}
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)
add_test(
    NAME ${Bezier}
    COMMAND ${Bezier}
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)
