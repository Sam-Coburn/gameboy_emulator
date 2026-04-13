#include <registers.hpp>

#include <stdint.h>

enum class ArithmeticTarget {
    A, B, C, D, E, H, L
};

enum class InstructionType {
    ADD, ADC
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
};