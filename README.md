# Game Boy Emulator

A Game Boy (DMG) emulator written in C++17. The goal is to run Pokémon Blue; see [docs/roadmap.md](docs/roadmap.md) for the plan and current progress.

> **Status:** the CPU (all opcodes, cycle timing, interrupts, HALT) is implemented and covered by unit tests. The emulator can't load or run a ROM yet. `src/main.cpp` is a placeholder and isn't part of the CMake build.

## Prerequisites

- A C++17 compiler (Apple Clang, GCC, or Clang)
- CMake 3.14 or newer
- Git and an internet connection for the first configure, because CMake downloads [Catch2](https://github.com/catchorg/Catch2) v3.5.4 with `FetchContent`

On macOS:

```sh
xcode-select --install   # compiler
brew install cmake
```

## Build

From the repo root:

```sh
cmake -S . -B build      # configure (only needed once, or after editing CMakeLists.txt)
cmake --build build      # compile
```

The first configure takes a little longer while Catch2 is fetched and built. After that, `cmake --build build` only recompiles what changed.

To start fresh, delete the build directory and configure again:

```sh
rm -rf build
```

## Run the tests

```sh
./build/tests
```

A passing run ends with something like:

```
All tests passed (1680 assertions in 79 test cases)
```

### Running a subset

Test cases are tagged (e.g. `[cpu]`, `[8-bit]`, `[add]`, `[instruction]`), so you can run only the ones you're working on:

```sh
./build/tests "[cpu]"                 # everything tagged [cpu]
./build/tests "[cpu][add]"            # tagged both [cpu] and [add]
./build/tests "ADD Unit Tests"        # a single test case by name
./build/tests --list-tests            # list every test case
./build/tests --list-tags             # list every tag
```

Useful flags:

```sh
./build/tests -s                      # show passing assertions too
./build/tests --success --durations yes
./build/tests --abort                 # stop at the first failure
```

### Adding a test file

New test files must be added to the `add_executable(tests ...)` list in [CMakeLists.txt](CMakeLists.txt), then rebuilt with `cmake --build build`.

## Test ROMs

Blargg's Game Boy test ROMs (`cpu_instrs`, `instr_timing`, `mem_timing`, and so on) go in `roms/gb-test-roms/`. They aren't committed to the repo. Get them from <https://github.com/retrio/gb-test-roms>:

```sh
git clone https://github.com/retrio/gb-test-roms roms/gb-test-roms
```

The emulator can't run them yet. They report results over the serial port, which is planned in [#22](https://github.com/Sam-Coburn/gameboy_emulator/issues/22).

## Editor setup (VS Code)

`CMAKE_EXPORT_COMPILE_COMMANDS` is on, so configuring writes `build/compile_commands.json`. `.vscode/settings.json` points the C/C++ extension at it, so IntelliSense picks up the include paths and C++ standard after the first configure.

## Project layout

```
include/    headers: CPU, registers, flags, instructions, cycle tables, memory bus
src/        main.cpp (placeholder entry point)
tests/      Catch2 unit tests
docs/       roadmap and reference links
roms/       test ROMs (not committed)
```
