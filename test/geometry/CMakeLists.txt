cmake_minimum_required(VERSION 2.8)

set(This Geometry)

include_directories(${PROJECT_SOURCE_DIR}/include)

set(Sources
    # test_bezier.cpp
    test_box.cpp
    # test_point.cpp
    # test_affine.cpp
    # test_point_chain.cpp
    # test_ts.cpp
    # test_tri_tri.cpp
    # test_seg_tri.cpp
    # test_predicate.cpp
)

add_executable(${This} ${Sources})

# IF(PYTHONLIBS_FOUND)
#   INCLUDE_DIRECTORIES("${PYTHON_INCLUDE_DIRS}")
# ELSE()
#   MESSAGE(FATAL_ERROR "Unable to find PythonLibs.")
# ENDIF()

target_link_libraries(${This} PUBLIC
    gtest_main
)

# if(PYTHON_LIBRARIES)
#   TARGET_LINK_LIBRARIES(${This} ${PYTHON_LIBRARIES})
# ELSE()
#   MESSAGE(FATAL_ERROR "Unable to link python libraries")
# ENDIF()

add_test(
    NAME ${This}
    COMMAND ${This}
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)
