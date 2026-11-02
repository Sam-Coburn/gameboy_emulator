#pragma once

#include <stdint.h>
#include <string>

// The serial port (link cable) with no cable connected. Takes full bus addresses
struct Serial {
    static constexpr uint16_t SB_ADDRESS = 0xFF01; // Serial transfer data
    static constexpr uint16_t SC_ADDRESS = 0xFF02; // Serial transfer control

    uint8_t sb = 0;
    uint8_t sc = 0;

    // Every byte sent out through the serial port, in order. Test ROMs print their results here
    std::string output;

    uint8_t read(uint16_t addr) const;

    // Returns the interrupt bits this write requests (see interrupts.hpp), or 0 for none.
    // The serial port never touches IF itself; the bus merges the bits in
    uint8_t write(uint16_t addr, uint8_t byte);
};
