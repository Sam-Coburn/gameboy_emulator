#include <registers.hpp>

#include <stdint.h>

enum class ArithmeticTarget {
    A, B, C, D, E, H, L
};

enum class InstructionType {
    ADD, ADC, SUB, SBC, AND,
    OR, XOR
};

struct Instruction {
    InstructionType type;
    ArithmeticTarget target;
};

class CPU {
    public:
        Registers registers;
        void execute(const Instruction& instruction) {
            switch (instruction.type) {

                /* 8-bit Arithmetic Operations */

                // ADD Instruction adds specified register contents to register A
                case InstructionType::ADD: {
                    uint8_t value = 0;

                    switch (instruction.target) {
                        case ArithmeticTarget::A:
                            value = registers.a;
                            break;
                        case ArithmeticTarget::B:
                            value = registers.b;
                            break;
                        case ArithmeticTarget::C:
                            value = registers.c;
                            break;
                        case ArithmeticTarget::D:
                            value = registers.d;
                            break;
                        case ArithmeticTarget::E:
                            value = registers.e;
                            break;
                        case ArithmeticTarget::H:
                            value = registers.h;
                            break;
                        case ArithmeticTarget::L:
                            value = registers.l;
                            break;
                        default:
                            break;
                    }

                    uint8_t new_value = add(value);
                    registers.a = new_value;

                    break;
                }

                // ADC Instruction adds specified register contents + current carry flag value to register A
                case InstructionType::ADC: {
                    uint8_t value = 0;

                    switch (instruction.target) {
                        case ArithmeticTarget::A:
                            value = registers.a;
                            break;
                        case ArithmeticTarget::B:
                            value = registers.b;
                            break;
                        case ArithmeticTarget::C:
                            value = registers.c;
                            break;
                        case ArithmeticTarget::D:
                            value = registers.d;
                            break;
                        case ArithmeticTarget::E:
                            value = registers.e;
                            break;
                        case ArithmeticTarget::H:
                            value = registers.h;
                            break;
                        case ArithmeticTarget::L:
                            value = registers.l;
                            break;
                    }

                    uint8_t new_value = adc(value);
                    registers.a = new_value;

                    break;
                }

                // SUB Instruction subtracts specfified register contents from register A
                case InstructionType::SUB: {
                    uint8_t value = 0;

                    switch (instruction.target) {
                        case ArithmeticTarget::A:
                            value = registers.a;
                            break;
                        case ArithmeticTarget::B:
                            value = registers.b;
                            break;
                        case ArithmeticTarget::C:
                            value = registers.c;
                            break;
                        case ArithmeticTarget::D:
                            value = registers.d;
                            break;
                        case ArithmeticTarget::E:
                            value = registers.e;
                            break;
                        case ArithmeticTarget::H:
                            value = registers.h;
                            break;
                        case ArithmeticTarget::L:
                            value = registers.l;
                            break;
                    }

                    uint8_t new_value = sub(value);
                    registers.a = new_value;

                    break;
                }

                // SBC Instruction substracts specified register contents + carry flag from register A
                case InstructionType::SBC: {
                    uint8_t value = 0;

                    switch (instruction.target) {
                        case ArithmeticTarget::A:
                            value = registers.a;
                            break;
                        case ArithmeticTarget::B:
                            value = registers.b;
                            break;
                        case ArithmeticTarget::C:
                            value = registers.c;
                            break;
                        case ArithmeticTarget::D:
                            value = registers.d;
                            break;
                        case ArithmeticTarget::E:
                            value = registers.e;
                            break;
                        case ArithmeticTarget::H:
                            value = registers.h;
                            break;
                        case ArithmeticTarget::L:
                            value = registers.l;
                            break;
                    }

                    uint8_t new_value = sbc(value);
                    registers.a = new_value;

                    break;
                }

                // AND Instruction performs bitwise AND operation using specified register contents with register A
                case InstructionType::AND: {
                    uint8_t value = 0;

                    switch (instruction.target) {
                        case ArithmeticTarget::A:
                            value = registers.a;
                            break;
                        case ArithmeticTarget::B:
                            value = registers.b;
                            break;
                        case ArithmeticTarget::C:
                            value = registers.c;
                            break;
                        case ArithmeticTarget::D:
                            value = registers.d;
                            break;
                        case ArithmeticTarget::E:
                            value = registers.e;
                            break;
                        case ArithmeticTarget::H:
                            value = registers.h;
                            break;
                        case ArithmeticTarget::L:
                            value = registers.l;
                            break;
                    }

                    uint8_t new_value = bitwise_and(value);
                    registers.a = new_value;

                    break;
                }

                // OR Instruction performs bitwise OR operation using specified register contents with register A
                case InstructionType::OR: {
                    uint8_t value = 0;

                    switch (instruction.target) {
                        case ArithmeticTarget::A:
                            value = registers.a;
                            break;
                        case ArithmeticTarget::B:
                            value = registers.b;
                            break;
                        case ArithmeticTarget::C:
                            value = registers.c;
                            break;
                        case ArithmeticTarget::D:
                            value = registers.d;
                            break;
                        case ArithmeticTarget::E:
                            value = registers.e;
                            break;
                        case ArithmeticTarget::H:
                            value = registers.h;
                            break;
                        case ArithmeticTarget::L:
                            value = registers.l;
                            break;
                    }

                    uint8_t new_value = bitwise_or(value);
                    registers.a = new_value;

                    break;
                }

                // XOR Instruction performs bitwise XOR operation using specified register contents with register A
                case InstructionType::XOR: {
                    uint8_t value = 0;

                    switch (instruction.target) {
                        case ArithmeticTarget::A:
                            value = registers.a;
                            break;
                        case ArithmeticTarget::B:
                            value = registers.b;
                            break;
                        case ArithmeticTarget::C:
                            value = registers.c;
                            break;
                        case ArithmeticTarget::D:
                            value = registers.d;
                            break;
                        case ArithmeticTarget::E:
                            value = registers.e;
                            break;
                        case ArithmeticTarget::H:
                            value = registers.h;
                            break;
                        case ArithmeticTarget::L:
                            value = registers.l;
                            break;
                    }

                    uint8_t new_value = bitwise_xor(value);
                    registers.a = new_value;

                    break;
                }

                default:
                    // TODO: support more instructions
                    break;
            }
        }

        uint8_t add(uint8_t value) {
            uint8_t result = registers.a + value;
            bool overflowed = result < registers.a;

            registers.f.zero = result == 0;
            registers.f.subtract = false;
            registers.f.carry = overflowed;

            // Half Carry is set if adding the lower nibbles of the value and register A
            // together result in a value bigger than 0xF. If the result is larger than 0xF
            // than the addition caused a carry from the lower nibble to the upper nibble.
            registers.f.half_carry = (registers.a & 0xF) + (value & 0xF) > 0xF;

            return result;
        }

        uint8_t adc(uint8_t value) {
            uint8_t result = registers.a + value + registers.f.carry;
            bool overflowed = result < registers.a;

            registers.f.zero = result == 0;
            registers.f.subtract = false;
            registers.f.carry = overflowed;
            registers.f.half_carry = (registers.a & 0xF) + (value & 0xF) > 0xF; // Same logic as for add()

            return result;
        }

        uint8_t sub(uint8_t value) {
            uint8_t result = registers.a - value;
            bool underflowed = value > registers.a;

            registers.f.zero = result == 0;
            registers.f.subtract = true;
            registers.f.carry = underflowed;
            registers.f.half_carry = (value & 0xF) > (registers.a & 0xF);

            return result;
        }

        uint8_t sbc(uint8_t value) {
            uint8_t result = registers.a - (value + registers.f.carry);
            bool underflowed = (value + registers.f.carry) > registers.a;

            registers.f.zero = result == 0;
            registers.f.subtract = true;
            registers.f.carry = underflowed;
            registers.f.half_carry = ((value + registers.f.carry) & 0xF) > (registers.a & 0xF);

            return result;
        }

        uint8_t bitwise_and(uint8_t value) {
            uint8_t result = registers.a & value;

            registers.f.zero = result == 0;
            registers.f.subtract = false;
            registers.f.carry = false;
            registers.f.half_carry = true;

            return result;
        }

        uint8_t bitwise_or(uint8_t value) {
            uint8_t result = registers.a & value;

            registers.f.zero = result == 0;
            registers.f.subtract = false;
            registers.f.carry = false;
            registers.f.half_carry = false;

            return result;
        }

        uint8_t bitwise_xor(uint8_t value) {
            uint8_t result = registers.a ^ value;

            registers.f.zero = result == 0;
            registers.f.subtract = false;
            registers.f.carry = false;
            registers.f.half_carry = false;

            return result;
        }
};