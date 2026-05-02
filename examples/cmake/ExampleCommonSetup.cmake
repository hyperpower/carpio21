include_guard(GLOBAL)

get_filename_component(CARPIO_EXAMPLE_COMMON_DIR "${CMAKE_CURRENT_LIST_DIR}" ABSOLUTE)
get_filename_component(CARPIO_EXAMPLES_DIR "${CARPIO_EXAMPLE_COMMON_DIR}/.." ABSOLUTE)
get_filename_component(CARPIO_REPO_ROOT "${CARPIO_EXAMPLES_DIR}/.." ABSOLUTE)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    message(STATUS "COMPILER is MSVC")
    add_compile_options(/Zc:__cplusplus)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    add_definitions(-DNOMINMAX)
    if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    endif()
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    message(STATUS "COMPILER is GCC")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    message(STATUS "COMPILER is Clang")
endif()

function(carpio_setup_example)
    set(options)
    set(one_value_args TARGET)
    set(multi_value_args SOURCES EXTRA_INCLUDE_DIRS EXTRA_DEFINITIONS EXTRA_LINK_LIBRARIES)
    cmake_parse_arguments(CARPIO_EXAMPLE
        "${options}"
        "${one_value_args}"
        "${multi_value_args}"
        ${ARGN}
    )

    if(CARPIO_EXAMPLE_UNPARSED_ARGUMENTS)
        message(FATAL_ERROR "Unknown carpio_setup_example arguments: ${CARPIO_EXAMPLE_UNPARSED_ARGUMENTS}")
    endif()

    if(NOT CARPIO_EXAMPLE_TARGET)
        set(CARPIO_EXAMPLE_TARGET main)
    endif()

    if(NOT CARPIO_EXAMPLE_SOURCES)
        set(CARPIO_EXAMPLE_SOURCES main.cpp)
    endif()

    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/)

    add_executable(${CARPIO_EXAMPLE_TARGET} ${CARPIO_EXAMPLE_SOURCES})
    set_target_properties(${CARPIO_EXAMPLE_TARGET} PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
    )

    target_include_directories(${CARPIO_EXAMPLE_TARGET} PUBLIC
        "${CARPIO_REPO_ROOT}/include"
        "${CARPIO_EXAMPLES_DIR}/include"
        ${CARPIO_EXAMPLE_EXTRA_INCLUDE_DIRS}
    )

    if(CARPIO_EXAMPLE_EXTRA_DEFINITIONS)
        target_compile_definitions(${CARPIO_EXAMPLE_TARGET} PUBLIC
            ${CARPIO_EXAMPLE_EXTRA_DEFINITIONS}
        )
    endif()

    if(CARPIO_EXAMPLE_EXTRA_LINK_LIBRARIES)
        target_link_libraries(${CARPIO_EXAMPLE_TARGET} PUBLIC
            ${CARPIO_EXAMPLE_EXTRA_LINK_LIBRARIES}
        )
    endif()
endfunction()
