#include <iostream>
#include <stdint.h>

using namespace std;

struct FlagsRegister {
    bool zero;
    bool subtract;
    bool half_carry;
    bool carry;

    static constexpr uint8_t ZERO_FLAG_BYTE_POSITION = 7;
    static constexpr uint8_t SUBTRACT_FLAG_BYTE_POSITION = 6;
    static constexpr uint8_t HALF_CARRY_FLAG_BYTE_POSITION = 5;
    static constexpr uint8_t CARRY_FLAG_BYTE_POSITION = 4;

    // Convert FlagsRegister -> uint8_t
    operator uint8_t() const {
        return (zero       ? 1 : 0) << ZERO_FLAG_BYTE_POSITION |
               (subtract   ? 1 : 0) << SUBTRACT_FLAG_BYTE_POSITION |
               (half_carry ? 1 : 0) << HALF_CARRY_FLAG_BYTE_POSITION |
               (carry      ? 1 : 0) << CARRY_FLAG_BYTE_POSITION;
    }

    // Convert uint8_t -> FlagsRegister
    FlagsRegister(uint8_t byte) {
        zero       = ((byte >> ZERO_FLAG_BYTE_POSITION) & 0x1) != 0;
        subtract   = ((byte >> SUBTRACT_FLAG_BYTE_POSITION) & 0x1) != 0;
        half_carry = ((byte >> HALF_CARRY_FLAG_BYTE_POSITION) & 0x1) != 0;
        carry      = ((byte >> CARRY_FLAG_BYTE_POSITION) & 0x1) != 0;
    }

    // Default
    FlagsRegister() : zero(false), subtract(false), half_carry(false), carry(false) {}
};

class Registers {
    public:
        // 8 registers representing 8-bit system
        uint8_t a;
        uint8_t b;
        uint8_t c;
        uint8_t d;
        uint8_t e;
        FlagsRegister f;
        uint8_t h;
        uint8_t l;

    // get and set virtual register functions for physical register pairs
    uint16_t get_af() {
        return (static_cast<uint16_t>(a) << 8) | f;
    }

    void set_af(uint16_t value) {
        a = static_cast<uint8_t>((value & 0xFF00) >> 8);
        f = FlagsRegister(static_cast<uint8_t>(value & 0xFF) & 0xF0); // Adding masking for lower 4 bits (always 0 for flags register)
    }

    uint16_t get_bc() {
        return (static_cast<uint16_t>(b) << 8) | static_cast<uint16_t>(c);
    }

    void set_bc(uint16_t value) {
        b = static_cast<uint8_t>((value & 0xFF00) >> 8);
        c = static_cast<uint8_t>(value & 0xFF);
    }

    uint16_t get_de() {
        return (static_cast<uint16_t>(d) << 8) | static_cast<uint16_t>(e);
    }

    void set_de(uint16_t value) {
        d = static_cast<uint8_t>((value & 0xFF00) >> 8);
        e = static_cast<uint8_t>(value & 0xFF);
    }

    uint16_t get_hl() {
        return (static_cast<uint16_t>(h) << 8) | static_cast<uint16_t>(l);
    }

    void set_hl(uint16_t value) {
        h = static_cast<uint8_t>((value & 0xFF00) >> 8);
        l = static_cast<uint8_t>(value & 0xFF);
    }
};

int main() {
    Registers regs;
    regs.b = 12;
    regs.c = 4;

    uint16_t result = regs.get_bc();

    cout << result << endl;

    return 0;
}