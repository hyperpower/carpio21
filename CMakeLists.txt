cmake_minimum_required(VERSION 2.8)

set(PROJECT_NAME Carpio)
if (POLICY CMP0048)
  cmake_policy(SET CMP0048 NEW)
endif (POLICY CMP0048)
project(${PROJECT_NAME} VERSION 0.1)

# window pre-define
if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    # Force to always compile with W4
    if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  endif()
endif()


# openmp
OPTION(ENABLE_OPENMP "Enable openmp" ON) # Enabled by default
IF(ENABLE_OPENMP)
    message(STATUS "======== LOOKING FOR OPENMP ========================")
    add_definitions(-DOPENMP)
    find_package(OpenMP)
	if (OPENMP_FOUND)
        set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
	endif()
ENDIF(ENABLE_OPENMP)

# python
# set(REQUIRED_PYTHON_VERSION 3.4)
find_package ( PythonInterp ${REQUIRED_PYTHON_VERSION} REQUIRED)
find_package ( PythonLibs ${REQUIRED_PYTHON_VERSION} REQUIRED)

set(cmake_cxx_standard 11)
# third-party folder
####################
# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)



include_directories(${PROJECT_SOURCE_DIR}/include)

add_subdirectory(${PROJECT_SOURCE_DIR}/third-party/googletest)
enable_testing()

# test folder
add_subdirectory(${PROJECT_SOURCE_DIR}/test/)
