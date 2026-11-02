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

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Emulator core, shared by the tests and the gameboy executable
add_library(gameboy_core STATIC
  src/cartridge.cpp
  src/gameboy.cpp
  src/memorybus.cpp
  src/ppu.cpp
  src/serial.cpp
)

target_include_directories(gameboy_core PUBLIC
  ${CMAKE_SOURCE_DIR}/include
)

add_executable(tests
  tests/flags_register_tests.cpp
  tests/register_tests.cpp
  tests/cpu_tests.cpp
  tests/instruction_tests.cpp
  tests/memorybus_tests.cpp
  tests/testrom_tests.cpp
  tests/component_tests.cpp
  tests/gameboy_tests.cpp
)

target_include_directories(tests PRIVATE
  ${CMAKE_SOURCE_DIR}/tests
)

target_link_libraries(tests PRIVATE gameboy_core Catch2::Catch2WithMain)

# Test-ROM tests look for Blargg's ROMs here
target_compile_definitions(tests PRIVATE ROMS_DIR="${CMAKE_SOURCE_DIR}/roms")

# Headless runner: runs a test ROM and prints its serial output
add_executable(gameboy
  src/main.cpp
)

target_link_libraries(gameboy PRIVATE gameboy_core)
