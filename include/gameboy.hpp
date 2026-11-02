#pragma once

#include <cartridge.hpp>
#include <cpu.hpp>
#include <memorybus.hpp>

#include <stdint.h>

// The whole machine, like the motherboard: it owns the bus (and through it every component)
// and the CPU
class GameBoy {
    public:
        // Declared before cpu because the CPU holds a reference to it. Members are constructed
        // in declaration order and destroyed in reverse, so the bus outlives the CPU
        MemoryBus bus;
        CPU cpu;

        // Starts in the state the boot ROM leaves behind, ready to run the game at 0x0100
        explicit GameBoy(Cartridge cartridge);

        // Runs one CPU instruction, then advances the rest of the hardware by the same number
        // of T-cycles. Returns the T-cycles taken
        uint8_t step();
};
