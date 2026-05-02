cmake_minimum_required(VERSION 3.10)
project(rotational_one_gate)

include(${CMAKE_CURRENT_LIST_DIR}/../cmake/ExampleCommonSetup.cmake)

find_package(Python COMPONENTS Interpreter Development REQUIRED)
include_directories("${Python_INCLUDE_DIRS}")

carpio_setup_example(TARGET main)

target_link_libraries(main PUBLIC ${Python_LIBRARIES})
