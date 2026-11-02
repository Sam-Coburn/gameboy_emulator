#pragma once

#include <cartridge.hpp>
#include <ppu.hpp>
#include <serial.hpp>

#include <array>
#include <stdint.h>

// The address decoder. The CPU reads and writes addresses without knowing what's behind them;
// the bus forwards each access to whichever component owns that address. Components never
// hold a reference back to the bus: they report interrupt requests as return values, and the
// bus ORs them into IF
//
//   0000-7FFF  Cartridge ROM (writes are MBC commands)
//   8000-9FFF  VRAM (PPU)
//   A000-BFFF  Cartridge RAM
//   C000-DFFF  Work RAM
//   E000-FDFF  Echo RAM: mirrors C000-DDFF
//   FE00-FE9F  OAM (PPU)
//   FEA0-FEFF  Unusable: reads 0xFF, writes ignored
//   FF00-FF7F  I/O registers
//   FF80-FFFE  High RAM
//   FFFF       IE
struct MemoryBus {
    static constexpr uint16_t IF_ADDRESS = 0xFF0F; // Interrupt Flag register
    static constexpr uint16_t IE_ADDRESS = 0xFFFF; // Interrupt Enable register

    Cartridge cartridge;
    Ppu ppu;
    Serial serial;

    std::array<uint8_t, 0x2000> wram{}; // C000-DFFF
    std::array<uint8_t, 0x7F> hram{};   // FF80-FFFE

    uint8_t interrupt_flag = 0;   // IF
    uint8_t interrupt_enable = 0; // IE

    // Plain storage for I/O registers (FF00-FF7F) whose component doesn't exist yet: joypad,
    // timer, audio and LCD registers. Each component takes over its addresses as it's added
    std::array<uint8_t, 0x80> io{};

    uint8_t read_byte(uint16_t addr);
    void write_byte(uint16_t addr, uint8_t byte);

    uint16_t read_word(uint16_t addr);
    void write_word(uint16_t addr, uint16_t word);

    // Advances every component by the given number of T-cycles and collects the interrupts
    // they request. Nothing ticks yet: the timer (#19) and PPU (#20) plug in here
    void tick(uint8_t cycles);

    // Sets the I/O registers to the values the DMG boot ROM leaves behind
    void apply_post_boot_state();
};
