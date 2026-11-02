#pragma once

#include <array>
#include <stdint.h>
#include <string>
#include <vector>

// The game cartridge: ROM at 0000-7FFF and external RAM at A000-BFFF. Takes full bus addresses.
// For now this only handles ROMs of 32 KB or less with no MBC. Bank switching comes in #18
struct Cartridge {
    std::vector<uint8_t> rom;
    std::array<uint8_t, 0x2000> ram{};

    // An empty cartridge reads 0xFF everywhere in ROM, like an empty cartridge slot
    Cartridge() = default;
    explicit Cartridge(std::vector<uint8_t> rom);

    static Cartridge from_file(const std::string& path);

    uint8_t read(uint16_t addr) const;

    // Writes to ROM addresses are MBC commands on real cartridges, so they never change ROM.
    // With no MBC they are ignored
    void write(uint16_t addr, uint8_t byte);
};
