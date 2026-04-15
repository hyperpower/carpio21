cmake_minimum_required(VERSION 3.22)

set(Onode Onode)
set(Ocell Ocell)
set(Ogrid Ogrid)
set(Ostringify Ostringify)

include_directories(${PROJECT_SOURCE_DIR}/include)

set(SRC_onde test_onode.cpp)
set(SRC_ocell test_ocell.cpp)
set(SRC_ogrid test_ogrid.cpp)
set(SRC_ostringify test_ostringify.cpp)

add_executable(${Onode} ${SRC_onde})
add_executable(${Ocell} ${SRC_ocell})
add_executable(${Ogrid} ${SRC_ogrid})
add_executable(${Ostringify} ${SRC_ostringify})


target_link_libraries(${Onode} PUBLIC
    gtest gtest_main
)
target_link_libraries(${Ocell} PUBLIC
    gtest gtest_main
)
target_link_libraries(${Ogrid} PUBLIC
    gtest gtest_main
)
target_link_libraries(${Ostringify} PUBLIC
    gtest gtest_main
)
add_test(
    NAME ${Onode}
    COMMAND ${Onode} 
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)
add_test(
    NAME ${Ocell}
    COMMAND ${Ocell} 
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)
add_test(
    NAME ${Ogrid}
    COMMAND ${Ogrid} 
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)
add_test(
    NAME ${Ostringify}
    COMMAND ${Ostringify} 
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)
