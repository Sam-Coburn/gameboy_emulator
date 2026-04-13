cmake_minimum_required(VERSION 3.14)
project(MyTests)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.5.4
)

FetchContent_MakeAvailable(Catch2)

add_executable(tests
  tests/flags_register_tests.cpp
)

target_include_directories(tests PRIVATE
  ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)