#pragma once

#include <gameboy.hpp>

#include <iostream>
#include <stdint.h>
#include <string>

// Runs Blargg-style test ROMs headless. These ROMs report their results as text sent
// through the serial port, so the CPU can be tested before any graphics exist
struct TestRomRunner {
    // About 48 seconds of emulated time at 4.19 MHz, longer than any cpu_instrs test needs
    static constexpr uint64_t DEFAULT_MAX_CYCLES = 200'000'000;

    // Runs the ROM until its serial output says "Passed" or "Failed", or until max_cycles
    // T-cycles have run. Returns everything the ROM sent through the serial port. If echo
    // is true, serial output is also printed to stdout as it arrives
    static std::string run(const std::string& path, uint64_t max_cycles = DEFAULT_MAX_CYCLES, bool echo = false) {
        GameBoy gameboy(Cartridge::from_file(path));

        const std::string& output = gameboy.bus.serial.output;
        size_t printed = 0;
        uint64_t cycles = 0;

        while (cycles < max_cycles) {
            cycles = cycles + gameboy.step();

            if (output.size() > printed) {
                if (echo) {
                    std::cout << output.substr(printed) << std::flush;
                }
                printed = output.size();

                if (output.find("Passed") != std::string::npos || output.find("Failed") != std::string::npos) {
                    break;
                }
            }
        }

        return output;
    }
};
