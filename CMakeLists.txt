cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME Carpio)
project(${PROJECT_NAME} VERSION 0.1)

if (POLICY CMP0048)
  cmake_policy(SET CMP0048 NEW)
endif (POLICY CMP0048)

set(CMAKE_CXX_STANDARD 20)

# window pre-define
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    message(STATUS "COMPILER is MSVC")
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    # set(cmake_cxx_standard 20)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    # Force to always compile with W4
    if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    endif()
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    message(STATUS "COMPILER is GCC")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    message(STATUS "COMPILER is Clang")
endif()


# openmp
OPTION(ENABLE_OPENMP "Enable openmp" ON) # Enabled by default¢
if (ENABLE_OPENMP)
    message(STATUS "======== LOOKING FOR OPENMP ========================")
    find_package(OpenMP)
	if (OPENMP_FOUND)
        set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
        add_definitions(-DOPENMP)
	endif()
endif()


# external project download form github
#######################################
# Prevent overriding the parent project's compiler/linker
# settings on Windows
# Download and unpack googletest at configure time
# configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
# message(${CMAKE_BINARY_DIR})
# execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
    # WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/googletest-download"
# )
# execute_process(COMMAND "${CMAKE_COMMAND}" --build .
    # WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/googletest-download"
# )

# Add googletest directly to our build. This adds the following targets:
# gtest, gtest_main, gmock and gmock_main
add_subdirectory(${PROJECT_SOURCE_DIR}/external/googletest)

# The gtest/gmock targets carry header search path dependencies
# automatically when using CMake 2.8.11 or later. Otherwise we
# have to add them here ourselves.
# if(CMAKE_VERSION VERSION_LESS 2.8.11)
#     include_directories("${gtest_SOURCE_DIR}/include"
#                         "${gmock_SOURCE_DIR}/include"
#     )
# endif()

# include_directories(${PROJECT_SOURCE_DIR}/include)

enable_testing()

# test folder
add_subdirectory(${PROJECT_SOURCE_DIR}/test/)
