#include <stdint.h>

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