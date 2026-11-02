# Game Boy Emulator

A Game Boy (DMG) emulator written in C++17. The goal is to run Pokémon Blue; see [docs/roadmap.md](docs/roadmap.md) for the plan and current progress.

> **Status:** the CPU (all opcodes, cycle timing, interrupts, HALT) and the serial port are implemented and covered by unit tests. There's no graphics yet, but the `gameboy` executable can run Blargg's test ROMs headless and print their results to the terminal. Every individual `cpu_instrs` ROM passes except `02-interrupts`, which needs the timer ([#19](https://github.com/Sam-Coburn/gameboy_emulator/issues/19)).

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

This builds two executables:

- `build/tests`: the Catch2 unit tests
- `build/gameboy`: the headless test-ROM runner

The first configure takes a little longer while Catch2 is fetched and built. After that, `cmake --build build` only recompiles what changed.

To start fresh, delete the build directory and configure again:

```sh
rm -rf build
```

## Run the tests

```sh
./build/tests
```

With the test ROMs downloaded (see [Test ROMs](#test-roms)), a passing run ends with something like:

```
test cases:   82 |   81 passed | 1 failed as expected
assertions: 1708 | 1707 passed | 1 failed as expected
```

The "failed as expected" case is `02-interrupts`, which is tagged `[!mayfail]` until the timer is implemented. Without the ROMs, the test-ROM tests are skipped instead:

```
test cases:   82 |   80 passed | 2 skipped
assertions: 1697 | 1697 passed
```

### Running a subset

Test cases are tagged (e.g. `[cpu]`, `[8-bit]`, `[add]`, `[instruction]`, `[testrom]`), so you can run only the ones you're working on:

```sh
./build/tests "[cpu]"                 # everything tagged [cpu]
./build/tests "[cpu][add]"            # tagged both [cpu] and [add]
./build/tests "ADD Unit Tests"        # a single test case by name
./build/tests "~[testrom]"            # everything except the (slower) test-ROM tests
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

Blargg's Game Boy test ROMs (`cpu_instrs`, `instr_timing`, `mem_timing`, and so on) go in `roms/gb-test-roms/`. They're gitignored, so they aren't committed to the repo. Get them from <https://github.com/retrio/gb-test-roms>:

```sh
git clone https://github.com/retrio/gb-test-roms roms/gb-test-roms
```

The ROMs report their results as text sent through the serial port (the link-cable hardware), so they can test the CPU before there are any graphics. `build/gameboy` runs one headless and streams that output to the terminal:

```sh
./build/gameboy "roms/gb-test-roms/cpu_instrs/individual/01-special.gb"
```

```
01-special


Passed
```

It stops as soon as the output contains `Passed` or `Failed` (or after about 48 seconds of emulated time). The exit code is `0` if the ROM passed and `1` otherwise, so it can be used in scripts:

```sh
for rom in roms/gb-test-roms/cpu_instrs/individual/*.gb; do
  ./build/gameboy "$rom" > /dev/null && echo "PASS $rom" || echo "FAIL $rom"
done
```

Limitations for now:

- Only ROMs of 32 KB or less load. That covers the individual `cpu_instrs` ROMs, but not the combined `cpu_instrs/cpu_instrs.gb` (64 KB), which needs MBC1 bank switching ([#18](https://github.com/Sam-Coburn/gameboy_emulator/issues/18)).
- `02-interrupts` fails until the timer is implemented ([#19](https://github.com/Sam-Coburn/gameboy_emulator/issues/19)).
- The link cable always behaves as if nothing is plugged in.

## Editor setup (VS Code)

`CMAKE_EXPORT_COMPILE_COMMANDS` is on, so configuring writes `build/compile_commands.json`. `.vscode/settings.json` points the C/C++ extension at it, so IntelliSense picks up the include paths and C++ standard after the first configure.

## Project layout

```
include/    headers: CPU, registers, flags, instructions, cycle tables, memory bus, test-ROM runner
src/        main.cpp (headless test-ROM runner entry point)
tests/      Catch2 unit tests and Blargg test-ROM tests
docs/       roadmap and reference links
roms/       test ROMs (not committed)
```
