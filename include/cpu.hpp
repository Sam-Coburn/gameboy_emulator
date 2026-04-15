#include <registers.hpp>

#include <stdint.h>
#include <iostream>
#include <bitset>

enum class ArithmeticTarget {
    A, B, C, D, E, H, L
};

enum class InstructionType {
    ADD, ADC, SUB, SBC, AND,
    OR, XOR, CP, INC, DEC,
    SWAP, SCF, CCF, CPL
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

                // CP Instruction subtracts specfified register contents from register A but doesnt store the result
                case InstructionType::CP: {
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

                    break;
                }

                // INC Instruction increments value of target register
                case InstructionType::INC: {
                    switch (instruction.target) {
                        case ArithmeticTarget::A:
                            registers.a = inc(registers.a);
                            break;
                        case ArithmeticTarget::B:
                            registers.b = inc(registers.b);
                            break;
                        case ArithmeticTarget::C:
                            registers.c = inc(registers.c);
                            break;
                        case ArithmeticTarget::D:
                            registers.d = inc(registers.d);
                            break;
                        case ArithmeticTarget::E:
                            registers.e = inc(registers.e);
                            break;
                        case ArithmeticTarget::H:
                            registers.h = inc(registers.h);
                            break;
                        case ArithmeticTarget::L:
                            registers.l = inc(registers.l);
                            break;
                    }

                    break;
                }

                // DEC Instruction increments value of target register
                case InstructionType::DEC: {
                    switch (instruction.target) {
                        case ArithmeticTarget::A:
                            registers.a = dec(registers.a);
                            break;
                        case ArithmeticTarget::B:
                            registers.b = dec(registers.b);
                            break;
                        case ArithmeticTarget::C:
                            registers.c = dec(registers.c);
                            break;
                        case ArithmeticTarget::D:
                            registers.d = dec(registers.d);
                            break;
                        case ArithmeticTarget::E:
                            registers.e = dec(registers.e);
                            break;
                        case ArithmeticTarget::H:
                            registers.h = dec(registers.h);
                            break;
                        case ArithmeticTarget::L:
                            registers.l = dec(registers.l);
                            break;
                    }

                    break;
                }

                // SWAP Instruction swaps the values of the upper and lower nibbles of the target register
                case InstructionType::SWAP: {
                    switch (instruction.target) {
                        case ArithmeticTarget::A:
                            registers.a = swap(registers.a);
                            break;
                        case ArithmeticTarget::B:
                            registers.b = swap(registers.b);
                            break;
                        case ArithmeticTarget::C:
                            registers.c = swap(registers.c);
                            break;
                        case ArithmeticTarget::D:
                            registers.d = swap(registers.d);
                            break;
                        case ArithmeticTarget::E:
                            registers.e = swap(registers.e);
                            break;
                        case ArithmeticTarget::H:
                            registers.h = swap(registers.h);
                            break;
                        case ArithmeticTarget::L:
                            registers.l = swap(registers.l);
                            break;
                    }

                    break;
                }

                // SCF Instruction sets the carry flag, i.e. changes its value to True
                case InstructionType::SCF: {
                    registers.f.carry = true;

                    registers.f.zero = registers.f.zero; // SCF instruction leaves zero flag unaffected
                    registers.f.subtract = false;
                    registers.f.half_carry = false;

                    break;
                }

                // CCF Instruction toggles the carry flag, e.g. C = True --> False, C = False --> True
                case InstructionType::CCF: {
                    if (registers.f.carry) {
                        registers.f.carry = false;
                    }
                    else {
                        registers.f.carry = true;
                    }

                    registers.f.zero = registers.f.zero; // CCF instruction leaves zero flag unaffected
                    registers.f.subtract = false;
                    registers.f.half_carry = false;

                    break;
                }

                // CPL Instruction toggles every bit of register A
                case InstructionType::CPL: {
                    registers.a = ~registers.a;

                    registers.f.zero = registers.f.zero; // CPL instruction leaves zero flag unaffected
                    registers.f.subtract = true;
                    registers.f.carry = registers.f.carry; // CPL instruction leaves carry flag unaffected
                    registers.f.half_carry = true;

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

        uint8_t inc(uint8_t value) {
            uint8_t result = value + 1;

            registers.f.zero = result == 0;
            registers.f.subtract = false;
            registers.f.carry = registers.f.carry; // INC doesnt affect the carry flag, it's left alone
            registers.f.half_carry = (value & 0xF) + (1 & 0xF) > 0xF;

            return result;
        }

        uint8_t dec(uint8_t value) {
            uint8_t result = value - 1;

            registers.f.zero = result == 0;
            registers.f.subtract = true;
            registers.f.carry = registers.f.carry; // DEC doesnt affect the carry flag, it's left alone
            registers.f.half_carry = (1 & 0xF) > (value & 0xF);

            return result;
        }

        uint8_t swap(uint8_t value) {
            uint8_t upper_nibble = value & 0b11110000;
            uint8_t lower_nibble = value & 0b00001111;
            uint8_t result = (lower_nibble << 4) | (upper_nibble >> 4);

            registers.f.zero = result == 0;
            registers.f.subtract = false;
            registers.f.carry = false;
            registers.f.half_carry = false;

            return result;
        }
};