#include <iostream>
#include <stdint.h>

using namespace std;

class Registers {
    public:
        // 8 registers representing 8-bit system
        uint8_t a;
        uint8_t b;
        uint8_t c;
        uint8_t d;
        uint8_t e;
        uint8_t f;
        uint8_t h;
        uint8_t l;


    // get and set virtual register functions for physical register pairs
    uint16_t get_af() {
        return (static_cast<uint16_t>(a) << 8) | static_cast<uint16_t>(f);
    }

    void set_af(uint16_t value) {
        a = static_cast<uint8_t>((value & 0xFF00) >> 8);
        f = static_cast<uint8_t>(value & 0xFF);
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