#include <interrupts.hpp>
#include <serial.hpp>

uint8_t Serial::read(uint16_t addr) const {
    if (addr == SB_ADDRESS) {
        return sb;
    }

    if (addr == SC_ADDRESS) {
        return sc;
    }

    return 0xFF;
}

uint8_t Serial::write(uint16_t addr, uint8_t byte) {
    if (addr == SB_ADDRESS) {
        sb = byte;
        return 0;
    }

    if (addr == SC_ADDRESS) {
        sc = byte;

        // Bit 7 requests a transfer and bit 0 selects the internal clock. With no cable
        // connected, an internal-clock transfer completes at once and shifts in all 1s.
        // Real hardware takes 8 bit-times; this instant version is enough for test ROMs
        if ((sc & 0x81) == 0x81) {
            output += static_cast<char>(sb);
            sb = 0xFF;
            sc = sc & 0x7F;
            return Interrupt::SERIAL;
        }
    }

    return 0;
}
