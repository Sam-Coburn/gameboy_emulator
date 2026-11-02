#include <cpu.hpp>

#include <fstream>
#include <iostream>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <stdint.h>
#include <string>
#include <vector>

// Runs Blargg-style test ROMs headless. These ROMs report their results as text sent
// through the serial port, so the CPU can be tested before any graphics exist
struct TestRomRunner {
    // About 48 seconds of emulated time at 4.19 MHz, longer than any cpu_instrs test needs
    static constexpr uint64_t DEFAULT_MAX_CYCLES = 200'000'000;

    // Copies the ROM into the start of memory. This only works for ROMs of 32 KB or less;
    // bigger ROMs need bank switching from the cartridge (#18)
    static void load_rom(MemoryBus& bus, const std::string& path) {
        std::ifstream file(path, std::ios::binary);
        if (!file) {
            throw std::runtime_error("Could not open ROM: " + path);
        }

        std::vector<uint8_t> rom((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        if (rom.size() > 0x8000) {
            throw std::runtime_error("ROM is bigger than 32 KB and needs bank switching: " + path);
        }

        for (size_t i = 0; i < rom.size(); i++) {
            bus.memory[i] = rom[i];
        }
    }

    // Runs the ROM until its serial output says "Passed" or "Failed", or until max_cycles
    // T-cycles have run. Returns everything the ROM sent through the serial port. If echo
    // is true, serial output is also printed to stdout as it arrives
    static std::string run(const std::string& path, uint64_t max_cycles = DEFAULT_MAX_CYCLES, bool echo = false) {
        // make_unique value-initializes, so memory starts zeroed instead of holding garbage
        std::unique_ptr<CPU> cpu = std::make_unique<CPU>();
        load_rom(cpu->bus, path);

        const std::string& output = cpu->bus.serial_output;
        size_t printed = 0;
        uint64_t cycles = 0;

        while (cycles < max_cycles) {
            cycles = cycles + cpu->step();

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
