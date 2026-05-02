cmake_minimum_required(VERSION 3.10)
project(unit_sine_square)

include(${CMAKE_CURRENT_LIST_DIR}/../cmake/ExampleCommonSetup.cmake)

find_package(Python COMPONENTS Interpreter Development REQUIRED)
include_directories("${Python_INCLUDE_DIRS}")

carpio_setup_example(TARGET main)

target_link_libraries(main PUBLIC ${Python_LIBRARIES})
