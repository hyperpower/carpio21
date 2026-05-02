cmake_minimum_required(VERSION 3.10)
project(mpi)

include(${CMAKE_CURRENT_LIST_DIR}/../cmake/ExampleCommonSetup.cmake)

find_package(MPI REQUIRED)

if(MPI_CXX_FOUND)
    include_directories("${MPI_CXX_INCLUDE_DIRS}")
else()
    message(FATAL_ERROR "Unable to find MPI Lib.")
endif()

carpio_setup_example(TARGET main)

if(MPI_CXX_LIBRARIES)
    target_link_libraries(main PUBLIC ${MPI_CXX_LIBRARIES})
else()
    message(FATAL_ERROR "Unable to link MPI libraries")
endif()
