cmake_minimum_required(VERSION 3.10)
project(segments_intersection)

include(${CMAKE_CURRENT_LIST_DIR}/../cmake/ExampleCommonSetup.cmake)

set(CMAKE_BUILD_TYPE "Release")

message(STATUS "--- LOOKING FOR PYTHON ---")
find_package(Python COMPONENTS Interpreter Development REQUIRED)
message(STATUS "Python Interpreter: ${Python_EXECUTABLE}")
message(STATUS "Python Include Dir: ${Python_INCLUDE_DIRS}")
message(STATUS "Python Libraries  : ${Python_LIBRARIES}")
include_directories("${Python_INCLUDE_DIRS}")

carpio_setup_example(
    TARGET main
    EXTRA_DEFINITIONS _DEBUG_MODE_
)

target_link_libraries(main PUBLIC ${Python_LIBRARIES})
