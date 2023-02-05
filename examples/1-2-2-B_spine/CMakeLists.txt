cmake_minimum_required(VERSION 3.0)
project(segments_intersection)

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  set(CMAKE_CXX20_STANDARD_COMPILE_OPTION "-std:c++latest")
  set(CMAKE_CXX20_EXTENSION_COMPILE_OPTION "-std:c++latest")
else()
  set(CMAKE_CXX_STANDARD 20)
endif()
 
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/)

set(SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)


# location of header files
INCLUDE_DIRECTORIES(
    ${PROJECT_SOURCE_DIR}/../../include
)

add_executable(main ${SOURCE_FILES})
