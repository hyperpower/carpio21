cmake_minimum_required(VERSION 2.8)
project(segments_intersection)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/)

set(SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)



# python =======
set(REQUIRED_PYTHON_VERSION 3.4)
# message(STATUS "======== LOOKING FOR PYTHON ${REQUIRED_PYTHON_VERSION} ========================")
find_package ( PythonInterp ${REQUIRED_PYTHON_VERSION} REQUIRED)
find_package ( PythonLibs ${REQUIRED_PYTHON_VERSION} REQUIRED)

#message(STATUS "PYTHON_EXECUTABLE:FILEPATH=${PYTHON_EXECUTABLE}")
#message(STATUS "PYTHON_LIBRARY:FILEPATH=${PYTHON_LIBRARY}")
#message(STATUS "PYTHON_LIBRARIES:=${PYTHON_LIBRARIES}")
#message(STATUS "PYTHON_INCLUDE_DIR:FILEPATH=${PYTHON_INCLUDE_DIR}")
#message(STATUS "PYTHON_FRAMEWORK_INCLUDES=${PYTHON_FRAMEWORK_INCLUDES}")
#message(STATUS "PYTHONLIBS_VERSION_STRING=${PYTHONLIBS_VERSION_STRING}")
#message(STATUS "PYTHON_FRAMEWORKS=${Python_FRAMEWORKS}")
message(STATUS "PROJECT_SOURCE_DIR=${PROJECT_SOURCE_DIR}")

IF(PYTHONLIBS_FOUND)
  INCLUDE_DIRECTORIES("${PYTHON_INCLUDE_DIRS}")
ELSE()
  MESSAGE(FATAL_ERROR "Unable to find PythonLibs.")
ENDIF()
# end python ===

# location of header files
INCLUDE_DIRECTORIES(
    ${PROJECT_SOURCE_DIR}/../../include
)

add_executable(main ${SOURCE_FILES})

if(PYTHON_LIBRARIES)
  TARGET_LINK_LIBRARIES(main ${PYTHON_LIBRARIES})
ELSE()
  MESSAGE(FATAL_ERROR "Unable to link python libraries")
ENDIF()