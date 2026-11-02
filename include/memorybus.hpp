#include <stdint.h>
#include <string>

struct MemoryBus {
    uint8_t memory[0x10000];

    static constexpr uint16_t SB_ADDRESS = 0xFF01; 
    static constexpr uint16_t SC_ADDRESS = 0xFF02;
    static constexpr uint16_t IF_ADDRESS = 0xFF0F; // Interrupt Flag register

    // Every byte sent out through the serial port, in order. Test ROMs print their results here
    std::string serial_output;

    uint8_t read_byte(uint16_t addr) {
        return memory[addr];
    }

    void write_byte(uint16_t addr, uint8_t byte) {
        memory[addr] = byte;

        if (addr == SC_ADDRESS) {
            if ((memory[SC_ADDRESS] & 0x81) == 0x81) {
                serial_output += read_byte(SB_ADDRESS);
                memory[SB_ADDRESS] = 0xFF;
                memory[SC_ADDRESS] = memory[SC_ADDRESS] & 0x7F;
                memory[IF_ADDRESS] |= 0x08; // Set the serial interrupt flag (IF bit 3)
            }
        }

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
