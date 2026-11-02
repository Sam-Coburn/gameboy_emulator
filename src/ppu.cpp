#include <ppu.hpp>

uint8_t Ppu::read(uint16_t addr) const {
    if (addr >= 0x8000 && addr <= 0x9FFF) {
        return vram[addr - 0x8000];
    }

    if (addr >= 0xFE00 && addr <= 0xFE9F) {
        return oam[addr - 0xFE00];
    }

    return 0xFF;
}

void Ppu::write(uint16_t addr, uint8_t byte) {
    if (addr >= 0x8000 && addr <= 0x9FFF) {
        vram[addr - 0x8000] = byte;
    } else if (addr >= 0xFE00 && addr <= 0xFE9F) {
        oam[addr - 0xFE00] = byte;
    }
}
