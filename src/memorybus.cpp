#include <memorybus.hpp>

uint8_t MemoryBus::read_byte(uint16_t addr) {
    if (addr <= 0x7FFF) {
        return cartridge.read(addr);
    }
    else if ((addr >= 0x8000) && (addr <= 0x9FFF)) {
        return ppu.read(addr);
    }
    else if ((addr >= 0xA000) && (addr <= 0xBFFF)) {
        return cartridge.read(addr);
    }
    else if ((addr >= 0xC000) && (addr <= 0xDFFF)) {
        return wram[addr - 0xC000];
    }
    else if ((addr >= 0xE000) && (addr <= 0xFDFF)) {
        return wram[addr - 0xE000];
    }
    else if ((addr >= 0xFE00) && (addr <= 0xFE9F)) {
        return ppu.read(addr);
    }
    else if ((addr >= 0xFEA0) && (addr <= 0xFEFF)) {
        return 0xFF; // Unusable
    }
    else if ((addr >= 0xFF00) && (addr <= 0xFF7F)) {
        if ((addr == Serial::SB_ADDRESS) || (addr == Serial::SC_ADDRESS)) {
            return serial.read(addr);
        }
        else if (addr == IF_ADDRESS) {
            return interrupt_flag;
        }
        else {
            return io[addr - 0xFF00];
        }
    }
    else if ((addr >= 0xFF80) && (addr <= 0xFFFE)) {
        return hram[addr - 0xFF80];
    }
    else {
        return interrupt_enable;
    }
}

void MemoryBus::write_byte(uint16_t addr, uint8_t byte) {
    if (addr <= 0x7FFF) {
        cartridge.write(addr, byte);
    }
    else if ((addr >= 0x8000) && (addr <= 0x9FFF)) {
        ppu.write(addr, byte);
    }
    else if ((addr >= 0xA000) && (addr <= 0xBFFF)) {
        cartridge.write(addr, byte);
    }
    else if ((addr >= 0xC000) && (addr <= 0xDFFF)) {
        wram[addr - 0xC000] = byte;
    }
    else if ((addr >= 0xE000) && (addr <= 0xFDFF)) {
        wram[addr - 0xE000] = byte;
    }
    else if ((addr >= 0xFE00) && (addr <= 0xFE9F)) {
        ppu.write(addr, byte);
    }
    else if ((addr >= 0xFEA0) && (addr <= 0xFEFF)) {
        // Unusable: writes are ignored
    }
    else if ((addr >= 0xFF00) && (addr <= 0xFF7F)) {
        if ((addr == Serial::SB_ADDRESS) || (addr == Serial::SC_ADDRESS)) {
            // Merge in the interrupts serial requests without clearing any already requested
            interrupt_flag |= serial.write(addr, byte);
        }
        else if (addr == IF_ADDRESS) {
            interrupt_flag = byte;
        }
        else {
            io[addr - 0xFF00] = byte;
        }
    }
    else if ((addr >= 0xFF80) && (addr <= 0xFFFE)) {
        hram[addr - 0xFF80] = byte;
    }
    else {
        interrupt_enable = byte;
    }
}

// 16-bit values are stored little endian (low byte first). The high byte's address wraps from
// 0xFFFF to 0x0000. Both bytes go through the decoder, since they can land in different regions
uint16_t MemoryBus::read_word(uint16_t addr) {
    uint16_t high_addr = addr + 1;
    return read_byte(addr) | (read_byte(high_addr) << 8);
}

void MemoryBus::write_word(uint16_t addr, uint16_t word) {
    uint16_t high_addr = addr + 1;
    write_byte(addr, word & 0xFF);
    write_byte(high_addr, (word >> 8) & 0xFF);
}

void MemoryBus::tick(uint8_t cycles) {
    (void)cycles;
}

// Values from Pan Docs, Power Up Sequence (DMG column). Set directly rather than through
// write_byte, so no write side effects (like starting a serial transfer) can fire
void MemoryBus::apply_post_boot_state() {
    io.fill(0);

    io[0x00] = 0xCF; // P1 (joypad)
    serial.sb = 0x00;
    serial.sc = 0x7E;
    io[0x04] = 0xAB; // DIV
    io[0x05] = 0x00; // TIMA
    io[0x06] = 0x00; // TMA
    io[0x07] = 0xF8; // TAC
    interrupt_flag = 0xE1; // IF: the boot ROM leaves V-Blank requested

    io[0x10] = 0x80; // NR10
    io[0x11] = 0xBF; // NR11
    io[0x12] = 0xF3; // NR12
    io[0x13] = 0xFF; // NR13
    io[0x14] = 0xBF; // NR14
    io[0x16] = 0x3F; // NR21
    io[0x17] = 0x00; // NR22
    io[0x18] = 0xFF; // NR23
    io[0x19] = 0xBF; // NR24
    io[0x1A] = 0x7F; // NR30
    io[0x1B] = 0xFF; // NR31
    io[0x1C] = 0x9F; // NR32
    io[0x1D] = 0xFF; // NR33
    io[0x1E] = 0xBF; // NR34
    io[0x20] = 0xFF; // NR41
    io[0x21] = 0x00; // NR42
    io[0x22] = 0x00; // NR43
    io[0x23] = 0xBF; // NR44
    io[0x24] = 0x77; // NR50
    io[0x25] = 0xF3; // NR51
    io[0x26] = 0xF1; // NR52

    io[0x40] = 0x91; // LCDC: LCD on, background on
    io[0x41] = 0x85; // STAT
    io[0x42] = 0x00; // SCY
    io[0x43] = 0x00; // SCX
    io[0x44] = 0x00; // LY
    io[0x45] = 0x00; // LYC
    io[0x46] = 0xFF; // DMA
    io[0x47] = 0xFC; // BGP
    io[0x48] = 0xFF; // OBP0: left uninitialised by the boot ROM
    io[0x49] = 0xFF; // OBP1: left uninitialised by the boot ROM
    io[0x4A] = 0x00; // WY
    io[0x4B] = 0x00; // WX

    interrupt_enable = 0x00;
}
