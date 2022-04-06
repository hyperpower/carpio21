cmake_minimum_required(VERSION 3.12)

set(This IO)

set(Sources
    test_plotly.cpp
    # test_ply.cpp
    # test_csv.cpp
    # test_textfile.cpp
)

# python =======
set(REQUIRED_PYTHON_VERSION 3.4)
# message(STATUS "======== LOOKING FOR PYTHON ${REQUIRED_PYTHON_VERSION} ========================")
find_package(Python COMPONENTS Interpreter Development)

IF(Python_FOUND)
  INCLUDE_DIRECTORIES("${Python_INCLUDE_DIRS}")
ELSE()
  MESSAGE(FATAL_ERROR "Unable to find PythonLibs.")
ENDIF()

add_executable(${This} ${Sources})

add_test(
    NAME ${This}
    COMMAND ${This}
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)
target_link_libraries(${This} PUBLIC
    gtest_main
)

if(Python_FOUND)
  target_link_libraries(${This} PUBLIC ${Python_LIBRARIES})
ELSE()
  MESSAGE(FATAL_ERROR "Unable to link python libraries")
ENDIF()






