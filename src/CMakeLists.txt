cmake_minimum_required(VERSION 3.14)
project(test)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Werror -Wextra -g")
#set(CMAKE_CXX_FLAGS "-g")

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.13.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(
  test
  tests/tests.cc
  model/Object.cc
  model/Object.h
  model/Matrix_oop_operators.cc
  model/Matrix_oop_private.cc
  model/Matrix_oop.cc
  model/Matrix.h
)

target_link_libraries(
  test
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(test)
