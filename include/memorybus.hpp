#include <stdint.h>

struct MemoryBus {
    uint8_t memory[0xFFFF];

    uint8_t read_byte(uint16_t addr) {
        return memory[addr];
    }

    void write_byte(uint16_t addr, uint8_t byte) {
        memory[addr] = byte;

        return;
    }
};