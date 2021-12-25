cmake_minimum_required(VERSION 3.22)

# Testing
################################
  # This adds another subdirectory, which has 'project(gtest)'
  #   add_subdirectory(${PROJECT_SOURCE_DIR}/third-party/googletest)

  enable_testing()

  # Include the gtest library. gtest_SOURCE_DIR is available due to
  # 'project(gtest)' above.
  include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
  include_directories(${PROJECT_SOURCE_DIR}/include)
  ##############
  # Unit Tests -- Add test_gtest
  ##############
  set(Source
      test_gtest.cpp
  )
  add_executable(TestGTest ${Source})

  # Standard linking to gtest stuff.
  target_link_libraries(TestGTest PUBLIC gtest gtest_main)

  # Extra linking for the project.
  # target_link_libraries(TestGTest add)

  # This is so you can do 'make test' to see all your tests run, instead of
  # manually running the executable runUnitTests to see those specific tests.
  add_test(NAME TestGTest COMMAND TestGTest)

  ## Uncomment here to run the test
  # add_subdirectory(algebra)
  # add_subdirectory(io)
  # add_subdirectory(utility)
  # add_subdirectory(substance)
  # add_subdirectory(geometry)
  # add_subdirectory(structure)
  # add_subdirectory(equation)
  # add_subdirectory(domain)
  add_subdirectory(equation)