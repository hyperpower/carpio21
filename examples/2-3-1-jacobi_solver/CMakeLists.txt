cmake_minimum_required(VERSION 3.10)
project(jacobi_solver)

include(${CMAKE_CURRENT_LIST_DIR}/../cmake/ExampleCommonSetup.cmake)

cmake_policy(SET CMP0074 NEW)

option(ENABLE_OPENMP "Enable openmp" ON)
if(ENABLE_OPENMP)
    message(STATUS "========== LOOKING FOR OPENMP ==========")
    if(APPLE)
        message(STATUS "IN ======= MacOS ")
        if(CMAKE_C_COMPILER_ID MATCHES "Clang$")
            set(OpenMP_C_FLAGS "-Xpreprocessor -fopenmp")
            set(OpenMP_C_LIB_NAMES "omp")
            set(OpenMP_omp_LIBRARY omp)
        endif()

        if(CMAKE_CXX_COMPILER_ID MATCHES "Clang$")
            set(OpenMP_CXX_FLAGS "-Xpreprocessor -fopenmp")
            set(OpenMP_CXX_LIB_NAMES "omp")
            set(OpenMP_omp_LIBRARY omp)
        endif()
    endif()

    find_package(OpenMP REQUIRED)
    if(OPENMP_FOUND)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
        add_definitions(-DOPENMP)
    endif()
endif()

carpio_setup_example(TARGET main)

if(APPLE)
    include_directories("/opt/homebrew/opt/libomp/include/")
    target_link_libraries(main PUBLIC OpenMP::OpenMP_CXX)
endif()
