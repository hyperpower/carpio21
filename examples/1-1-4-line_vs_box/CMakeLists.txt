cmake_minimum_required(VERSION 3.0)
project(segment_vs_box)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/)

set(SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

find_package(Python COMPONENTS Interpreter Development)

IF(Python_FOUND)
  INCLUDE_DIRECTORIES("${Python_INCLUDE_DIRS}")
ELSE()
  MESSAGE(FATAL_ERROR "Unable to find PythonLibs.")
ENDIF()
# location of header files
INCLUDE_DIRECTORIES(
    ${PROJECT_SOURCE_DIR}/../../include
)

add_executable(main ${SOURCE_FILES})

if(Python_FOUND)
  target_link_libraries(main PUBLIC ${Python_LIBRARIES})
ELSE()
  MESSAGE(FATAL_ERROR "Unable to link python libraries")
ENDIF()