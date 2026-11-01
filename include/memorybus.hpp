#include <stdint.h>

struct MemoryBus {
    uint8_t memory[0x10000];

    uint8_t read_byte(uint16_t addr) {
        return memory[addr];
    }

    void write_byte(uint16_t addr, uint8_t byte) {
        memory[addr] = byte;

        return;
    }

    // 16-bit values are stored little endian (low byte first). The high byte's address
    // wraps from 0xFFFF to 0x0000 rather than going past the end of memory
    uint16_t read_word(uint16_t addr) {
        uint16_t high_addr = addr + 1;
        return memory[addr] | (memory[high_addr] << 8);
    }

    void write_word(uint16_t addr, uint16_t word) {
        uint16_t high_addr = addr + 1;
        memory[addr] = word & 0xFF;
        memory[high_addr] = (word >> 8) & 0xFF;
    }
};
