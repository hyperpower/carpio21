cmake_minimum_required(VERSION 3.10)
project(segments_intersection_benchmark)

include(${CMAKE_CURRENT_LIST_DIR}/../cmake/ExampleCommonSetup.cmake)

set(CMAKE_BUILD_TYPE "Release")

find_package(CGAL)

find_package(Python COMPONENTS Interpreter Development REQUIRED)
include_directories("${Python_INCLUDE_DIRS}")

carpio_setup_example(TARGET main)

target_link_libraries(main PUBLIC ${Python_LIBRARIES})

if(CGAL_FOUND)
    target_link_libraries(main PUBLIC CGAL::CGAL)
else()
    message(FATAL_ERROR "Unable to link CGAL libraries")
endif()
