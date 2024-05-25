cmake_minimum_required(VERSION 3.0)
project(segments_intersection)

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  set(CMAKE_CXX20_STANDARD_COMPILE_OPTION "-std:c++latest")
  set(CMAKE_CXX20_EXTENSION_COMPILE_OPTION "-std:c++latest")
  # add_definitions(-DNOMINMAX)
else()
  set(CMAKE_CXX_STANDARD 20)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/)

set(SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

SET(CMAKE_BUILD_TYPE "Release")

# python =======
set(REQUIRED_PYTHON_VERSION 3.4)
message(STATUS "--- LOOKING FOR PYTHON ---")
find_package(Python COMPONENTS Interpreter Development)

IF(Python_FOUND)
  message(STATUS "Python Interpreter: ${Python_EXECUTABLE}")
  message(STATUS "Python Include Dir: ${Python_INCLUDE_DIRS}")
  message(STATUS "Python Libraries  : ${Python_LIBRARIES}")
  INCLUDE_DIRECTORIES("${Python_INCLUDE_DIRS}")
ELSE()
  MESSAGE(FATAL_ERROR "Unable to find Python.")
ENDIF()
# end python ===

# location of header files
INCLUDE_DIRECTORIES(
  ${PROJECT_SOURCE_DIR}/../../include
)

add_definitions(-D_DEBUG_MODE_)
add_executable(main ${SOURCE})


if(Python_FOUND)
  TARGET_LINK_LIBRARIES(main PUBLIC ${PYTHON_LIBRARIES})
ELSE()
  MESSAGE(FATAL_ERROR "Unable to link python libraries")
ENDIF()



  