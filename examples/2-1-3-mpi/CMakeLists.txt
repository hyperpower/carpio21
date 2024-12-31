cmake_minimum_required(VERSION 3.8)
project(segments_intersection)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/)

set(SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

find_package(MPI REQUIRED)


IF(MPI_CXX_FOUND)
  INCLUDE_DIRECTORIES("${MPI_CXX_INCLUDE_DIRS}")
ELSE()
  MESSAGE(FATAL_ERROR "Unable to find MPI Lib.")
ENDIF()


# location of header files
INCLUDE_DIRECTORIES(
    ${PROJECT_SOURCE_DIR}/../../include
)

add_executable(main ${SOURCE_FILES})


if(MPI_CXX_LIBRARIES)
  TARGET_LINK_LIBRARIES(main ${MPI_CXX_LIBRARIES})
ELSE()
  MESSAGE(FATAL_ERROR "Unable to link MPI libraries")
ENDIF()