#pragma once

#include <cpu.hpp>
#include <memorybus.hpp>

#include <stdint.h>
#include <vector>

// Holds the bus for TestCpu. It's a separate base class because base classes are constructed
// before members and before later bases, in declaration order. Listing it before CPU means the
// bus exists before CPU's constructor binds a reference to it ("base-from-member" idiom)
struct TestBusOwner {
    MemoryBus test_bus;

    // A blank 32 KB ROM, so tests can place programs at ROM addresses with poke()
    TestBusOwner() {
        test_bus.cartridge = Cartridge(std::vector<uint8_t>(0x8000, 0));
    }
};

// A CPU with its own bus, so tests can write `TestCpu c;`. Unlike GameBoy, it starts with
// every register at zero instead of the post-boot state, so tests control all of it
struct TestCpu : private TestBusOwner, public CPU {
    TestCpu() : CPU(test_bus) {}

    // Writes a byte for a test to read back. ROM is read-only from the CPU's side (writes are
    // MBC commands), so ROM addresses write straight into the cartridge's ROM, as if the program
    // had been flashed onto it. Every other address goes through the bus
    void poke(uint16_t addr, uint8_t byte) {
        if (addr < 0x8000) {
            test_bus.cartridge.rom[addr] = byte;
        } else {
            bus.write_byte(addr, byte);
        }
    }
};
