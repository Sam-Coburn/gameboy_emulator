#pragma once

#include <array>
#include <stdint.h>

// The graphics chip. For now it only owns its memory: VRAM (8000-9FFF), which holds tile data
// and tile maps, and OAM (FE00-FE9F), which holds the 40 sprite entries. Takes full bus
// addresses. Rendering, LCD registers and the V-Blank/STAT interrupts come in #20
struct Ppu {
    std::array<uint8_t, 0x2000> vram{};
    std::array<uint8_t, 0xA0> oam{};

    uint8_t read(uint16_t addr) const;
    void write(uint16_t addr, uint8_t byte);
};
