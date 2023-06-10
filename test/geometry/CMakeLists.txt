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
    # test_ts.cpp
    # test_tri_tri.cpp
    # test_seg_tri.cpp
    # test_predicate.cpp
    # test_polygon_partition.cpp
    # test_segment.cpp
    test_ben_ott.cpp
)
set(Sources_Bezier test_bezier.cpp)

find_package(Python COMPONENTS Interpreter Development)

file(WRITE "./out.txt" CMake_Version : ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}\n)
file(APPEND "./out.txt" System_Name : ${CMAKE_SYSTEM_NAME}\n)
file(APPEND "./out.txt" System_Verion : ${CMAKE_SYSTEM_VERSION}\n)
file(APPEND "./out.txt" System_Processor : ${CMAKE_HOST_SYSTEM_PROCESSOR}\n)
file(APPEND "./out.txt" CXX_Compiler : ${CMAKE_CXX_COMPILER}\n)
file(APPEND "./out.txt" CXX_Compiler_Version : ${CMAKE_CXX_COMPILER_VERSION}\n)
file(APPEND "./out.txt" CXX_Standard : ${CMAKE_CXX_STANDARD}\n)
# message(PROJECT_SOURCE_DIR)


cmake_host_system_information(RESULT DISTRO QUERY OS_NAME NUMBER_OF_PHYSICAL_CORES NUMBER_OF_LOGICAL_CORES TOTAL_PHYSICAL_MEMORY PROCESSOR_NAME PROCESSOR_DESCRIPTION)
foreach(VAR IN LISTS DISTRO)
  file(APPEND "./out.txt" ${VAR}=`${${VAR}}`\n)
endforeach()

IF(Python_FOUND)
  INCLUDE_DIRECTORIES("${Python_INCLUDE_DIRS}")
ELSE()
  MESSAGE(FATAL_ERROR "Unable to find PythonLibs.")
ENDIF()
 
add_executable(${This} ${Sources})
add_executable(${Bezier} ${Sources_Bezier})

target_link_libraries(${This} PUBLIC
    gtest_main
)
target_link_libraries(${Bezier} PUBLIC
    gtest_main
)

if(Python_FOUND)
  file(APPEND "./out.txt" Python_Lib : ${Python_LIBRARIES}\n)
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
