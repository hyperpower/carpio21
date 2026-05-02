cmake_minimum_required(VERSION 3.10)
project(array_operation)

include(${CMAKE_CURRENT_LIST_DIR}/../cmake/ExampleCommonSetup.cmake)

cmake_policy(SET CMP0074 NEW)

if(CMAKE_CXX_FLAGS_RELEASE MATCHES "-O[0-3]")
    string(REPLACE "-O0" "-O3" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
    string(REPLACE "-O1" "-O3" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
    string(REPLACE "-O2" "-O3" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
else()
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
endif()

find_package(benchmark REQUIRED)
find_package(Armadillo REQUIRED)

carpio_setup_example(
    TARGET main
    EXTRA_INCLUDE_DIRS
        "${CARPIO_REPO_ROOT}/external/eigen"
        ${ARMADILLO_INCLUDE_DIRS}
    EXTRA_LINK_LIBRARIES
        benchmark::benchmark
        ${ARMADILLO_LIBRARIES}
)
