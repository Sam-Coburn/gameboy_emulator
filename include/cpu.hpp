#include <registers.hpp>

#include <bit>
#include <bitset>
#include <iostream>
#include <stdint.h>

enum class ArithmeticTarget8Bit {
    A, B, C, D, E, H, L
};

enum class ArithmeticTarget16Bit {
    BC, DE, HL, SP
};

enum class InstructionType {
    ADD, ADC, SUB, SBC, AND,
    OR, XOR, CP, INC, DEC,
    SWAP, SCF, CCF, CPL, BIT,
    SET, RESET, ADDHL, RLCA, RLA,
    RRCA, RRA, RLC, RL, RRC,
    RR, SLA, SRA, SRL
};

struct Instruction {
    InstructionType type;
    ArithmeticTarget8Bit target_8bit;
    ArithmeticTarget16Bit target_16bit;
    uint8_t bit_index;
};

class MemoryBus {
    public:
        uint8_t memory[0xFFFF];

        uint8_t read_byte(uint16_t addr) {
            return memory[addr];
        }
};

class CPU {
    public:
        Registers registers;
        uint16_t pc;
        MemoryBus bus;

        uint16_t sp = 0xFFFE;

        void execute(const Instruction& instruction) {
            switch (instruction.type) {

                /* 8-bit Arithmetic Operations */

                // ADD Instruction adds specified register contents to register A
                case InstructionType::ADD: {
                    uint8_t value = 0;

                    switch (instruction.target_8bit) {
                        case ArithmeticTarget8Bit::A:
                            value = registers.a;
                            break;
                        case ArithmeticTarget8Bit::B:
                            value = registers.b;
                            break;
                        case ArithmeticTarget8Bit::C:
                            value = registers.c;
                            break;
                        case ArithmeticTarget8Bit::D:
                            value = registers.d;
                            break;
                        case ArithmeticTarget8Bit::E:
                            value = registers.e;
                            break;
                        case ArithmeticTarget8Bit::H:
                            value = registers.h;
                            break;
                        case ArithmeticTarget8Bit::L:
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

                    switch (instruction.target_8bit) {
                        case ArithmeticTarget8Bit::A:
                            value = registers.a;
                            break;
                        case ArithmeticTarget8Bit::B:
                            value = registers.b;
                            break;
                        case ArithmeticTarget8Bit::C:
                            value = registers.c;
                            break;
                        case ArithmeticTarget8Bit::D:
                            value = registers.d;
                            break;
                        case ArithmeticTarget8Bit::E:
                            value = registers.e;
                            break;
                        case ArithmeticTarget8Bit::H:
                            value = registers.h;
                            break;
                        case ArithmeticTarget8Bit::L:
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

                    switch (instruction.target_8bit) {
                        case ArithmeticTarget8Bit::A:
                            value = registers.a;
                            break;
                        case ArithmeticTarget8Bit::B:
                            value = registers.b;
                            break;
                        case ArithmeticTarget8Bit::C:
                            value = registers.c;
                            break;
                        case ArithmeticTarget8Bit::D:
                            value = registers.d;
                            break;
                        case ArithmeticTarget8Bit::E:
                            value = registers.e;
                            break;
                        case ArithmeticTarget8Bit::H:
                            value = registers.h;
                            break;
                        case ArithmeticTarget8Bit::L:
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

                    switch (instruction.target_8bit) {
                        case ArithmeticTarget8Bit::A:
                            value = registers.a;
                            break;
                        case ArithmeticTarget8Bit::B:
                            value = registers.b;
                            break;
                        case ArithmeticTarget8Bit::C:
                            value = registers.c;
                            break;
                        case ArithmeticTarget8Bit::D:
                            value = registers.d;
                            break;
                        case ArithmeticTarget8Bit::E:
                            value = registers.e;
                            break;
                        case ArithmeticTarget8Bit::H:
                            value = registers.h;
                            break;
                        case ArithmeticTarget8Bit::L:
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

                    switch (instruction.target_8bit) {
                        case ArithmeticTarget8Bit::A:
                            value = registers.a;
                            break;
                        case ArithmeticTarget8Bit::B:
                            value = registers.b;
                            break;
                        case ArithmeticTarget8Bit::C:
                            value = registers.c;
                            break;
                        case ArithmeticTarget8Bit::D:
                            value = registers.d;
                            break;
                        case ArithmeticTarget8Bit::E:
                            value = registers.e;
                            break;
                        case ArithmeticTarget8Bit::H:
                            value = registers.h;
                            break;
                        case ArithmeticTarget8Bit::L:
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

                    switch (instruction.target_8bit) {
                        case ArithmeticTarget8Bit::A:
                            value = registers.a;
                            break;
                        case ArithmeticTarget8Bit::B:
                            value = registers.b;
                            break;
                        case ArithmeticTarget8Bit::C:
                            value = registers.c;
                            break;
                        case ArithmeticTarget8Bit::D:
                            value = registers.d;
                            break;
                        case ArithmeticTarget8Bit::E:
                            value = registers.e;
                            break;
                        case ArithmeticTarget8Bit::H:
                            value = registers.h;
                            break;
                        case ArithmeticTarget8Bit::L:
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

                    switch (instruction.target_8bit) {
                        case ArithmeticTarget8Bit::A:
                            value = registers.a;
                            break;
                        case ArithmeticTarget8Bit::B:
                            value = registers.b;
                            break;
                        case ArithmeticTarget8Bit::C:
                            value = registers.c;
                            break;
                        case ArithmeticTarget8Bit::D:
                            value = registers.d;
                            break;
                        case ArithmeticTarget8Bit::E:
                            value = registers.e;
                            break;
                        case ArithmeticTarget8Bit::H:
                            value = registers.h;
                            break;
                        case ArithmeticTarget8Bit::L:
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

                    switch (instruction.target_8bit) {
                        case ArithmeticTarget8Bit::A:
                            value = registers.a;
                            break;
                        case ArithmeticTarget8Bit::B:
                            value = registers.b;
                            break;
                        case ArithmeticTarget8Bit::C:
                            value = registers.c;
                            break;
                        case ArithmeticTarget8Bit::D:
                            value = registers.d;
                            break;
                        case ArithmeticTarget8Bit::E:
                            value = registers.e;
                            break;
                        case ArithmeticTarget8Bit::H:
                            value = registers.h;
                            break;
                        case ArithmeticTarget8Bit::L:
                            value = registers.l;
                            break;
                    }

                    uint8_t new_value = sub(value);

                    break;
                }

                // INC Instruction increments value of target register
                case InstructionType::INC: {
                    switch (instruction.target_8bit) {
                        case ArithmeticTarget8Bit::A:
                            registers.a = inc(registers.a);
                            break;
                        case ArithmeticTarget8Bit::B:
                            registers.b = inc(registers.b);
                            break;
                        case ArithmeticTarget8Bit::C:
                            registers.c = inc(registers.c);
                            break;
                        case ArithmeticTarget8Bit::D:
                            registers.d = inc(registers.d);
                            break;
                        case ArithmeticTarget8Bit::E:
                            registers.e = inc(registers.e);
                            break;
                        case ArithmeticTarget8Bit::H:
                            registers.h = inc(registers.h);
                            break;
                        case ArithmeticTarget8Bit::L:
                            registers.l = inc(registers.l);
                            break;
                    }

                    break;
                }

                // DEC Instruction increments value of target register
                case InstructionType::DEC: {
                    switch (instruction.target_8bit) {
                        case ArithmeticTarget8Bit::A:
                            registers.a = dec(registers.a);
                            break;
                        case ArithmeticTarget8Bit::B:
                            registers.b = dec(registers.b);
                            break;
                        case ArithmeticTarget8Bit::C:
                            registers.c = dec(registers.c);
                            break;
                        case ArithmeticTarget8Bit::D:
                            registers.d = dec(registers.d);
                            break;
                        case ArithmeticTarget8Bit::E:
                            registers.e = dec(registers.e);
                            break;
                        case ArithmeticTarget8Bit::H:
                            registers.h = dec(registers.h);
                            break;
                        case ArithmeticTarget8Bit::L:
                            registers.l = dec(registers.l);
                            break;
                    }

                    break;
                }

                // SWAP Instruction swaps the values of the upper and lower nibbles of the target register
                case InstructionType::SWAP: {
                    switch (instruction.target_8bit) {
                        case ArithmeticTarget8Bit::A:
                            registers.a = swap(registers.a);
                            break;
                        case ArithmeticTarget8Bit::B:
                            registers.b = swap(registers.b);
                            break;
                        case ArithmeticTarget8Bit::C:
                            registers.c = swap(registers.c);
                            break;
                        case ArithmeticTarget8Bit::D:
                            registers.d = swap(registers.d);
                            break;
                        case ArithmeticTarget8Bit::E:
                            registers.e = swap(registers.e);
                            break;
                        case ArithmeticTarget8Bit::H:
                            registers.h = swap(registers.h);
                            break;
                        case ArithmeticTarget8Bit::L:
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

                // BIT Instruction tests the bit value for the given register at the given bit index
                case InstructionType::BIT: {
                    uint8_t bit_index = instruction.bit_index;
                    uint8_t value = 0;

                    switch (instruction.target_8bit) {
                        case ArithmeticTarget8Bit::A:
                            value = registers.a;
                            break;
                        case ArithmeticTarget8Bit::B:
                            value = registers.b;
                            break;
                        case ArithmeticTarget8Bit::C:
                            value = registers.c;
                            break;
                        case ArithmeticTarget8Bit::D:
                            value = registers.d;
                            break;
                        case ArithmeticTarget8Bit::E:
                            value = registers.e;
                            break;
                        case ArithmeticTarget8Bit::H:
                            value = registers.h;
                            break;
                        case ArithmeticTarget8Bit::L:
                            value = registers.l;
                            break;
                    }

                    uint8_t bit_value = (value >> bit_index) & (0b00000001);

                    registers.f.zero = bit_value == 0;
                    registers.f.subtract = false;
                    registers.f.carry = registers.f.carry; // BIT instruction leaves carry flag unaffected
                    registers.f.half_carry = true;

                    break;
                }

                // SET Instruction sets the bit value for the given register at the given bit index to 1
                case InstructionType::SET: {
                    uint8_t bit_index = instruction.bit_index;

                    switch (instruction.target_8bit) {
                        case ArithmeticTarget8Bit::A:
                            registers.a = registers.a | (1 << bit_index);
                            break;
                        case ArithmeticTarget8Bit::B:
                            registers.b = registers.b | (1 << bit_index);
                            break;
                        case ArithmeticTarget8Bit::C:
                            registers.c = registers.c | (1 << bit_index);
                            break;
                        case ArithmeticTarget8Bit::D:
                            registers.d = registers.d | (1 << bit_index);
                            break;
                        case ArithmeticTarget8Bit::E:
                            registers.e = registers.e | (1 << bit_index);
                            break;
                        case ArithmeticTarget8Bit::H:
                            registers.h = registers.h | (1 << bit_index);
                            break;
                        case ArithmeticTarget8Bit::L:
                            registers.l = registers.l | (1 << bit_index);
                            break;
                    }

                    break;
                }

                // RESET Instruction sets the bit value for the given register at the given bit index to 0
                case InstructionType::RESET: {
                    uint8_t bit_index = instruction.bit_index;

                    switch (instruction.target_8bit) {
                        case ArithmeticTarget8Bit::A:
                            registers.a = registers.a & ~(1 << bit_index);
                            break;
                        case ArithmeticTarget8Bit::B:
                            registers.b = registers.b & ~(1 << bit_index);
                            break;
                        case ArithmeticTarget8Bit::C:
                            registers.c = registers.c & ~(1 << bit_index);
                            break;
                        case ArithmeticTarget8Bit::D:
                            registers.d = registers.d & ~(1 << bit_index);
                            break;
                        case ArithmeticTarget8Bit::E:
                            registers.e = registers.e & ~(1 << bit_index);
                            break;
                        case ArithmeticTarget8Bit::H:
                            registers.h = registers.h & ~(1 << bit_index);
                            break;
                        case ArithmeticTarget8Bit::L:
                            registers.l = registers.l & ~(1 << bit_index);
                            break;
                    }

                    break;
                }

                // ADDHL Instruction adds the contents of the given 16-bit register to the HL register
                case InstructionType::ADDHL: {
                    uint16_t value = 0;

                    switch (instruction.target_16bit) {
                        case ArithmeticTarget16Bit::BC:
                            value = registers.get_bc();
                            break;
                        case ArithmeticTarget16Bit::DE:
                            value = registers.get_de();
                            break;
                        case ArithmeticTarget16Bit::HL:
                            value = registers.get_hl();
                            break;
                        case ArithmeticTarget16Bit::SP:
                            value = sp;
                            break;
                    }

                    uint16_t new_value = add_hl(value);
                    registers.set_hl(new_value);

                    break;
                }

                // RLCA Instruction rotates the contents of register A 1 bit to the left, the carry flag is set to old bit 7
                case InstructionType::RLCA: {
                    uint8_t value = registers.a;
                    bool new_carry = (value & 0b10000000) >> 7;

                    value = (value << 1) | (new_carry);

                    registers.a = value;

                    registers.f.zero = registers.a == 0;
                    registers.f.subtract = false;
                    registers.f.carry = new_carry;
                    registers.f.half_carry = false;

                    break;
                }

                // RLA Instruction rotates the contents of register A 1 bit to the left, the carry flag is set to old bit 7, bit 0 is set to old carry flag value
                case InstructionType::RLA: {
                    uint8_t value = registers.a;
                    bool old_carry = registers.f.carry;
                    bool new_carry = (value & 0b10000000) >> 7;

                    value = (value << 1) | (old_carry);

                    registers.a = value;

                    registers.f.zero = registers.a == 0;
                    registers.f.subtract = false;
                    registers.f.carry = new_carry;
                    registers.f.half_carry = false;

                    break;
                }

                // RRCA Instruction rotates the contents of register A 1 bit to the right, the carry flag is set to old bit 0
                case InstructionType::RRCA: {
                    uint8_t value = registers.a;
                    bool new_carry = value & 1;

                    value = (value >> 1) | (new_carry << 7);

                    registers.a = value;

                    registers.f.zero = registers.a == 0;
                    registers.f.subtract = false;
                    registers.f.carry = new_carry;
                    registers.f.half_carry = false;

                    break;
                }

                // RRA Instruction rotates the contents of register A 1 bit to the right, the carry flag is rotated in for bit 7 (leftmost bit) and bit 0 is rotated out to the carry flag value
                case InstructionType::RRA: {
                    uint8_t value = registers.a;
                    uint8_t old_carry = registers.f.carry;
                    bool new_carry = value & 1;

                    value = (value >> 1) | (old_carry << 7);

                    registers.a = value;

                    registers.f.zero = registers.a == 0;
                    registers.f.subtract = false;
                    registers.f.carry = new_carry;
                    registers.f.half_carry = false;

                    break;
                }

                // RLC Instruction rotates the contents of the proviced register 1 bit to the left, the carry flag is set to old bit 7
                case InstructionType::RLC: {
                    switch (instruction.target_8bit) {
                        case ArithmeticTarget8Bit::A:
                            registers.a = rlc(registers.a);
                            break;
                        case ArithmeticTarget8Bit::B:
                            registers.b = rlc(registers.b);
                            break;
                        case ArithmeticTarget8Bit::C:
                            registers.c = rlc(registers.c);
                            break;
                        case ArithmeticTarget8Bit::D:
                            registers.d = rlc(registers.d);
                            break;
                        case ArithmeticTarget8Bit::E:
                            registers.e = rlc(registers.e);
                            break;
                        case ArithmeticTarget8Bit::H:
                            registers.h = rlc(registers.h);
                            break;
                        case ArithmeticTarget8Bit::L:
                            registers.l = rlc(registers.l);
                            break;
                    }

                    break;
                }

                // RL Instruction rotates the contents of the provided register 1 bit to the left, the carry flag is set to old bit 7, bit 0 is set to old carry flag value
                case InstructionType::RL: {
                    switch (instruction.target_8bit) {
                        case ArithmeticTarget8Bit::A:
                            registers.a = rla(registers.a);
                            break;
                        case ArithmeticTarget8Bit::B:
                            registers.b = rla(registers.b);
                            break;
                        case ArithmeticTarget8Bit::C:
                            registers.c = rla(registers.c);
                            break;
                        case ArithmeticTarget8Bit::D:
                            registers.d = rla(registers.d);
                            break;
                        case ArithmeticTarget8Bit::E:
                            registers.e = rla(registers.e);
                            break;
                        case ArithmeticTarget8Bit::H:
                            registers.h = rla(registers.h);
                            break;
                        case ArithmeticTarget8Bit::L:
                            registers.l = rla(registers.l);
                            break;
                    }

                    break;
                }

                // RRC Instruction rotates the contents of the provided register 1 bit to the right, the carry flag is set to old bit 0
                case InstructionType::RRC: {
                    switch (instruction.target_8bit) {
                        case ArithmeticTarget8Bit::A:
                            registers.a = rrc(registers.a);
                            break;
                        case ArithmeticTarget8Bit::B:
                            registers.b = rrc(registers.b);
                            break;
                        case ArithmeticTarget8Bit::C:
                            registers.c = rrc(registers.c);
                            break;
                        case ArithmeticTarget8Bit::D:
                            registers.d = rrc(registers.d);
                            break;
                        case ArithmeticTarget8Bit::E:
                            registers.e = rrc(registers.e);
                            break;
                        case ArithmeticTarget8Bit::H:
                            registers.h = rrc(registers.h);
                            break;
                        case ArithmeticTarget8Bit::L:
                            registers.l = rrc(registers.l);
                            break;
                    }

                    break;
                }

                // RR Instruction rotates the contents of the provided register 1 bit to the right, the carry flag is rotated in for bit 7 (leftmost bit) and bit 0 is rotated out to the carry flag value
                case InstructionType::RR: {
                    switch (instruction.target_8bit) {
                        case ArithmeticTarget8Bit::A:
                            registers.a = rr(registers.a);
                            break;
                        case ArithmeticTarget8Bit::B:
                            registers.b = rr(registers.b);
                            break;
                        case ArithmeticTarget8Bit::C:
                            registers.c = rr(registers.c);
                            break;
                        case ArithmeticTarget8Bit::D:
                            registers.d = rr(registers.d);
                            break;
                        case ArithmeticTarget8Bit::E:
                            registers.e = rr(registers.e);
                            break;
                        case ArithmeticTarget8Bit::H:
                            registers.h = rr(registers.h);
                            break;
                        case ArithmeticTarget8Bit::L:
                            registers.l = rr(registers.l);
                            break;
                    }

                    break;
                }

                // SLA Instruction shifts the value of the provided register to the left by 1 bit and stores old bit 7 in carry
                case InstructionType::SLA: {
                    switch (instruction.target_8bit) {
                        case ArithmeticTarget8Bit::A:
                            registers.a = sla(registers.a);
                            break;
                        case ArithmeticTarget8Bit::B:
                            registers.b = sla(registers.b);
                            break;
                        case ArithmeticTarget8Bit::C:
                            registers.c = sla(registers.c);
                            break;
                        case ArithmeticTarget8Bit::D:
                            registers.d = sla(registers.d);
                            break;
                        case ArithmeticTarget8Bit::E:
                            registers.e = sla(registers.e);
                            break;
                        case ArithmeticTarget8Bit::H:
                            registers.h = sla(registers.h);
                            break;
                        case ArithmeticTarget8Bit::L:
                            registers.l = sla(registers.l);
                            break;
                    }

                    break;
                }

                // SRA Instruction shifts the value of the provided register to the right by 1 bit and stores old bit 0 in carry, the MSB remains the same (don't shift in 0 automatically)
                case InstructionType::SRA: {
                    switch (instruction.target_8bit) {
                        case ArithmeticTarget8Bit::A:
                            registers.a = sra(registers.a);
                            break;
                        case ArithmeticTarget8Bit::B:
                            registers.b = sra(registers.b);
                            break;
                        case ArithmeticTarget8Bit::C:
                            registers.c = sra(registers.c);
                            break;
                        case ArithmeticTarget8Bit::D:
                            registers.d = sra(registers.d);
                            break;
                        case ArithmeticTarget8Bit::E:
                            registers.e = sra(registers.e);
                            break;
                        case ArithmeticTarget8Bit::H:
                            registers.h = sra(registers.h);
                            break;
                        case ArithmeticTarget8Bit::L:
                            registers.l = sra(registers.l);
                            break;
                    }

                    break;
                }

                // SRL Instruction shifts the value of the provided register to the right by 1 bit and stores old bit 0 in carry
                case InstructionType::SRL: {
                    switch (instruction.target_8bit) {
                        case ArithmeticTarget8Bit::A:
                            registers.a = srl(registers.a);
                            break;
                        case ArithmeticTarget8Bit::B:
                            registers.b = srl(registers.b);
                            break;
                        case ArithmeticTarget8Bit::C:
                            registers.c = srl(registers.c);
                            break;
                        case ArithmeticTarget8Bit::D:
                            registers.d = srl(registers.d);
                            break;
                        case ArithmeticTarget8Bit::E:
                            registers.e = srl(registers.e);
                            break;
                        case ArithmeticTarget8Bit::H:
                            registers.h = srl(registers.h);
                            break;
                        case ArithmeticTarget8Bit::L:
                            registers.l = srl(registers.l);
                            break;
                    }

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

        uint16_t add_hl(uint16_t value) {
            uint16_t result = registers.get_hl() + value;
            bool overflowed = result < registers.get_hl();

            registers.f.zero = registers.f.zero; // ADDHL doesnt affect the zero flag, it's left alone
            registers.f.subtract = false;
            registers.f.carry = overflowed;
            registers.f.half_carry = (registers.get_hl() & 0xFFF) + (value & 0xFFF) > 0xFFF;

            return result;
        }

        uint8_t rlc(uint8_t value) {
            bool new_carry = (value & 0b10000000) >> 7;

            uint8_t result = (value << 1) | (new_carry);

            registers.f.zero = result == 0;
            registers.f.subtract = false;
            registers.f.carry = new_carry;
            registers.f.half_carry = false;

            return result;
        }

        uint8_t rla(uint8_t value) {
            bool old_carry = registers.f.carry;
            bool new_carry = (value & 0b10000000) >> 7;

            uint8_t result = (value << 1) | (old_carry);

            registers.f.zero = result == 0;
            registers.f.subtract = false;
            registers.f.carry = new_carry;
            registers.f.half_carry = false;

            return result;
        }

        uint8_t rrc(uint8_t value) {
            bool new_carry = value & 1;

            uint8_t result = (value >> 1) | (new_carry << 7);

            registers.f.zero = result == 0;
            registers.f.subtract = false;
            registers.f.carry = new_carry;
            registers.f.half_carry = false;

            return result;
        }

        uint8_t rr(uint8_t value) {
            uint8_t old_carry = registers.f.carry;
            bool new_carry = value & 1;

            uint8_t result = (value >> 1) | (old_carry << 7);

            registers.f.zero = result == 0;
            registers.f.subtract = false;
            registers.f.carry = new_carry;
            registers.f.half_carry = false;

            return result;
        }

        uint8_t sla(uint8_t value) {
            bool new_carry = (value & 0b10000000) >> 7;
            uint8_t result = value << 1;

            registers.f.zero = result == 0;
            registers.f.subtract = false;
            registers.f.carry = new_carry;
            registers.f.half_carry = false;

            return result;
        }

        uint8_t sra(uint8_t value) {
            bool new_carry = value & 1;
            bool msb = (value & 0b10000000) >> 7;

            uint8_t result = (value >> 1) | (msb << 7);

            registers.f.zero = result == 0;
            registers.f.subtract = false;
            registers.f.carry = new_carry;
            registers.f.half_carry = false;

            return result;
        }

        uint8_t srl(uint8_t value) {
            bool new_carry = value & 1;

            uint8_t result = value >> 1;

            registers.f.zero = result == 0;
            registers.f.subtract = false;
            registers.f.carry = new_carry;
            registers.f.half_carry = false;

            return result;
        }
};