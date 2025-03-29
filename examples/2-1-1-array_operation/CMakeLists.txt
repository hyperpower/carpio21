cmake_minimum_required(VERSION 3.10)
project(array_operation)

message(STATUS "CXX COMPILER NAME    = ${CMAKE_CXX_COMPILER}")
message(STATUS "CXX COMPILER VERSION = ${CMAKE_CXX_COMPILER_VERSION}")

cmake_policy(SET CMP0074 NEW) 

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  set(CMAKE_CXX20_STANDARD_COMPILE_OPTION "-std:c++latest")
  set(CMAKE_CXX20_EXTENSION_COMPILE_OPTION "-std:c++latest")
else()
  set(CMAKE_CXX_STANDARD 20)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/)

set(SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

if(CMAKE_CXX_FLAGS_RELEASE MATCHES "-O[0-3]")
    string(REPLACE "-O0" "-O3" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
    string(REPLACE "-O1" "-O3" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
    string(REPLACE "-O2" "-O3" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
else()
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
endif()

find_package(benchmark REQUIRED)
find_package(Armadillo REQUIRED)

# location of header files
INCLUDE_DIRECTORIES(
    ${PROJECT_SOURCE_DIR}/../../include
    ${PROJECT_SOURCE_DIR}/../../external/eigen
)
add_executable(main ${SOURCE_FILES})

target_include_directories(main PRIVATE ${ARMADILLO_INCLUDE_DIRS})
target_link_libraries(main PRIVATE benchmark::benchmark ${ARMADILLO_LIBRARIES})

# add_custom_target(run_benchmark
#     COMMAND ./main --benchmark_out=results.json --benchmark_out_format=json
#     DEPENDS main
#     WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
#     COMMENT "Running benchmark and saving results to results.json"
# )
