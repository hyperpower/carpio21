cmake_minimum_required(VERSION 3.5)
project(jacobi_solver)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/)

set(SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

cmake_policy(SET CMP0074 NEW) 
# CMake will prefer to search for package configuration files
# in the locations specified by CMAKE_PREFIX_PATH, 
# but only if CMAKE_PREFIX_PATH is set by the user or through an environment variable. 
# This ensures that the package configuration file search respects user-specified paths first.

OPTION(ENABLE_OPENMP "Enable openmp" ON) # Enabled by default
IF(ENABLE_OPENMP)
  message(STATUS "========== LOOKING FOR OPENMP ==========")
  if(APPLE)
    message(STATUS "IN ======= MacOS ")
    if(CMAKE_C_COMPILER_ID MATCHES "Clang\$")
        set(OpenMP_C_FLAGS "-Xpreprocessor -fopenmp")
        set(OpenMP_C_LIB_NAMES "omp")
        set(OpenMP_omp_LIBRARY omp)
    endif()

    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang\$")
        set(OpenMP_CXX_FLAGS "-Xpreprocessor -fopenmp")
        set(OpenMP_CXX_LIB_NAMES "omp")
        set(OpenMP_omp_LIBRARY omp)
    endif()
  endif()

  find_package(OpenMP REQUIRED)
  if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    # set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
    add_definitions(-DOPENMP)
  endif(OPENMP_FOUND) 
ENDIF(ENABLE_OPENMP)


# location of header files
INCLUDE_DIRECTORIES(
    ${PROJECT_SOURCE_DIR}/../../include
)

add_executable(main ${SOURCE_FILES})

if(APPLE)
  include_directories("/opt/homebrew/opt/libomp/include/")
  # link_directories("/opt/homebrew/opt/libomp/lib/")
  # target_link_directories(main PUBLIC "/opt/homebrew/opt/libomp/lib/")
  target_link_libraries(main OpenMP::OpenMP_CXX)
endif()

