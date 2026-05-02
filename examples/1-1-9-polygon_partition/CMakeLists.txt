cmake_minimum_required(VERSION 3.10)
project(polygon_partition)

include(${CMAKE_CURRENT_LIST_DIR}/../cmake/ExampleCommonSetup.cmake)

find_package(Python COMPONENTS Interpreter Development REQUIRED)
include_directories("${Python_INCLUDE_DIRS}")

carpio_setup_example(
    TARGET main
    EXTRA_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(main PUBLIC ${Python_LIBRARIES})
