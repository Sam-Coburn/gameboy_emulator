#include <registers.hpp>

#include <bit>
#include <bitset>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <stdint.h>
#include <string>

enum class Operand8 {
    A, B, C, D, E, H, L, HL_PTR, NUM
};

struct ALU8Data {
    Operand8 target;
};

struct BitOpData {
    Operand8 target;
    uint8_t bit_index;
};

enum class Operand16 {
    BC, DE, HL, SP
};

struct ALU16Data {
    Operand16 target;
};

enum class JumpTest {
    NotZero, Zero, NotCarry, Carry, Always
};

struct JumpData {
    JumpTest condition;
};

enum class InstructionType {
    // 8-bit ALU operations
    ADD, ADC, SUB, SBC, AND,
    OR, XOR, CP, INC, DEC,

    // 16-bit ALU operations
    ADDHL,

    // Miscelleaneous instructions
    SWAP, CPL, CCF, SCF,

    // Bit operations
    BIT, SET, RES,

    // Rotate and shift operations
    RLCA, RLA, RRCA, RRA, RLC,
    RL, RRC, RR, SLA, SRA,
    SRL,

    // Jump instructions
    JP, JR, JP_HL
};

struct Instruction {
    InstructionType type;

    union {
        ALU8Data alu8;
        ALU16Data alu16;
        BitOpData bitop;
        JumpData jump;
    };
};

static std::optional<Instruction> from_byte_prefixed(uint8_t byte) {
    auto make_alu8 = [](InstructionType t, Operand8 op) -> std::optional<Instruction> {
        Instruction inst{};
        inst.type = t;
        inst.alu8.target = op;
        return inst;
    };

    auto make_bitop = [](InstructionType t, Operand8 op, uint8_t bit) -> std::optional<Instruction> {
        Instruction inst{};
        inst.type = t;
        inst.bitop.target = op;
        inst.bitop.bit_index = bit;
        return inst;
    };

    auto make_simple = [](InstructionType t) -> std::optional<Instruction> {
        Instruction inst{};
        inst.type = t;
        return inst;
    };

    switch (byte) {
        // --------------------
        // SWAP r
        // --------------------
        case 0x30: return make_alu8(InstructionType::SWAP, Operand8::B);
        case 0x31: return make_alu8(InstructionType::SWAP, Operand8::C);
        case 0x32: return make_alu8(InstructionType::SWAP, Operand8::D);
        case 0x33: return make_alu8(InstructionType::SWAP, Operand8::E);
        case 0x34: return make_alu8(InstructionType::SWAP, Operand8::H);
        case 0x35: return make_alu8(InstructionType::SWAP, Operand8::L);
        case 0x36: return make_alu8(InstructionType::SWAP, Operand8::HL_PTR);
        case 0x37: return make_alu8(InstructionType::SWAP, Operand8::A);

        // --------------------
        // BIT b, r
        // --------------------

        // BIT 0
        case 0x40: return make_bitop(InstructionType::BIT, Operand8::B, 0);
        case 0x41: return make_bitop(InstructionType::BIT, Operand8::C, 0);
        case 0x42: return make_bitop(InstructionType::BIT, Operand8::D, 0);
        case 0x43: return make_bitop(InstructionType::BIT, Operand8::E, 0);
        case 0x44: return make_bitop(InstructionType::BIT, Operand8::H, 0);
        case 0x45: return make_bitop(InstructionType::BIT, Operand8::L, 0);
        case 0x46: return make_bitop(InstructionType::BIT, Operand8::HL_PTR, 0);
        case 0x47: return make_bitop(InstructionType::BIT, Operand8::A, 0);

        // BIT 1
        case 0x48: return make_bitop(InstructionType::BIT, Operand8::B, 1);
        case 0x49: return make_bitop(InstructionType::BIT, Operand8::C, 1);
        case 0x4A: return make_bitop(InstructionType::BIT, Operand8::D, 1);
        case 0x4B: return make_bitop(InstructionType::BIT, Operand8::E, 1);
        case 0x4C: return make_bitop(InstructionType::BIT, Operand8::H, 1);
        case 0x4D: return make_bitop(InstructionType::BIT, Operand8::L, 1);
        case 0x4E: return make_bitop(InstructionType::BIT, Operand8::HL_PTR, 1);
        case 0x4F: return make_bitop(InstructionType::BIT, Operand8::A, 1);

        // BIT 2
        case 0x50: return make_bitop(InstructionType::BIT, Operand8::B, 2);
        case 0x51: return make_bitop(InstructionType::BIT, Operand8::C, 2);
        case 0x52: return make_bitop(InstructionType::BIT, Operand8::D, 2);
        case 0x53: return make_bitop(InstructionType::BIT, Operand8::E, 2);
        case 0x54: return make_bitop(InstructionType::BIT, Operand8::H, 2);
        case 0x55: return make_bitop(InstructionType::BIT, Operand8::L, 2);
        case 0x56: return make_bitop(InstructionType::BIT, Operand8::HL_PTR, 2);
        case 0x57: return make_bitop(InstructionType::BIT, Operand8::A, 2);

        // BIT 3
        case 0x58: return make_bitop(InstructionType::BIT, Operand8::B, 3);
        case 0x59: return make_bitop(InstructionType::BIT, Operand8::C, 3);
        case 0x5A: return make_bitop(InstructionType::BIT, Operand8::D, 3);
        case 0x5B: return make_bitop(InstructionType::BIT, Operand8::E, 3);
        case 0x5C: return make_bitop(InstructionType::BIT, Operand8::H, 3);
        case 0x5D: return make_bitop(InstructionType::BIT, Operand8::L, 3);
        case 0x5E: return make_bitop(InstructionType::BIT, Operand8::HL_PTR, 3);
        case 0x5F: return make_bitop(InstructionType::BIT, Operand8::A, 3);

        // BIT 4
        case 0x60: return make_bitop(InstructionType::BIT, Operand8::B, 4);
        case 0x61: return make_bitop(InstructionType::BIT, Operand8::C, 4);
        case 0x62: return make_bitop(InstructionType::BIT, Operand8::D, 4);
        case 0x63: return make_bitop(InstructionType::BIT, Operand8::E, 4);
        case 0x64: return make_bitop(InstructionType::BIT, Operand8::H, 4);
        case 0x65: return make_bitop(InstructionType::BIT, Operand8::L, 4);
        case 0x66: return make_bitop(InstructionType::BIT, Operand8::HL_PTR, 4);
        case 0x67: return make_bitop(InstructionType::BIT, Operand8::A, 4);

        // BIT 5
        case 0x68: return make_bitop(InstructionType::BIT, Operand8::B, 5);
        case 0x69: return make_bitop(InstructionType::BIT, Operand8::C, 5);
        case 0x6A: return make_bitop(InstructionType::BIT, Operand8::D, 5);
        case 0x6B: return make_bitop(InstructionType::BIT, Operand8::E, 5);
        case 0x6C: return make_bitop(InstructionType::BIT, Operand8::H, 5);
        case 0x6D: return make_bitop(InstructionType::BIT, Operand8::L, 5);
        case 0x6E: return make_bitop(InstructionType::BIT, Operand8::HL_PTR, 5);
        case 0x6F: return make_bitop(InstructionType::BIT, Operand8::A, 5);

        // BIT 6
        case 0x70: return make_bitop(InstructionType::BIT, Operand8::B, 6);
        case 0x71: return make_bitop(InstructionType::BIT, Operand8::C, 6);
        case 0x72: return make_bitop(InstructionType::BIT, Operand8::D, 6);
        case 0x73: return make_bitop(InstructionType::BIT, Operand8::E, 6);
        case 0x74: return make_bitop(InstructionType::BIT, Operand8::H, 6);
        case 0x75: return make_bitop(InstructionType::BIT, Operand8::L, 6);
        case 0x76: return make_bitop(InstructionType::BIT, Operand8::HL_PTR, 6);
        case 0x77: return make_bitop(InstructionType::BIT, Operand8::A, 6);

        // BIT 7
        case 0x78: return make_bitop(InstructionType::BIT, Operand8::B, 7);
        case 0x79: return make_bitop(InstructionType::BIT, Operand8::C, 7);
        case 0x7A: return make_bitop(InstructionType::BIT, Operand8::D, 7);
        case 0x7B: return make_bitop(InstructionType::BIT, Operand8::E, 7);
        case 0x7C: return make_bitop(InstructionType::BIT, Operand8::H, 7);
        case 0x7D: return make_bitop(InstructionType::BIT, Operand8::L, 7);
        case 0x7E: return make_bitop(InstructionType::BIT, Operand8::HL_PTR, 7);
        case 0x7F: return make_bitop(InstructionType::BIT, Operand8::A, 7);

        // --------------------
        // SET b, r
        // --------------------

        // SET 0
        case 0xC0: return make_bitop(InstructionType::SET, Operand8::B, 0);
        case 0xC1: return make_bitop(InstructionType::SET, Operand8::C, 0);
        case 0xC2: return make_bitop(InstructionType::SET, Operand8::D, 0);
        case 0xC3: return make_bitop(InstructionType::SET, Operand8::E, 0);
        case 0xC4: return make_bitop(InstructionType::SET, Operand8::H, 0);
        case 0xC5: return make_bitop(InstructionType::SET, Operand8::L, 0);
        case 0xC6: return make_bitop(InstructionType::SET, Operand8::HL_PTR, 0);
        case 0xC7: return make_bitop(InstructionType::SET, Operand8::A, 0);

        // SET 1
        case 0xC8: return make_bitop(InstructionType::SET, Operand8::B, 1);
        case 0xC9: return make_bitop(InstructionType::SET, Operand8::C, 1);
        case 0xCA: return make_bitop(InstructionType::SET, Operand8::D, 1);
        case 0xCB: return make_bitop(InstructionType::SET, Operand8::E, 1);
        case 0xCC: return make_bitop(InstructionType::SET, Operand8::H, 1);
        case 0xCD: return make_bitop(InstructionType::SET, Operand8::L, 1);
        case 0xCE: return make_bitop(InstructionType::SET, Operand8::HL_PTR, 1);
        case 0xCF: return make_bitop(InstructionType::SET, Operand8::A, 1);

        // SET 2
        case 0xD0: return make_bitop(InstructionType::SET, Operand8::B, 2);
        case 0xD1: return make_bitop(InstructionType::SET, Operand8::C, 2);
        case 0xD2: return make_bitop(InstructionType::SET, Operand8::D, 2);
        case 0xD3: return make_bitop(InstructionType::SET, Operand8::E, 2);
        case 0xD4: return make_bitop(InstructionType::SET, Operand8::H, 2);
        case 0xD5: return make_bitop(InstructionType::SET, Operand8::L, 2);
        case 0xD6: return make_bitop(InstructionType::SET, Operand8::HL_PTR, 2);
        case 0xD7: return make_bitop(InstructionType::SET, Operand8::A, 2);

        // SET 3
        case 0xD8: return make_bitop(InstructionType::SET, Operand8::B, 3);
        case 0xD9: return make_bitop(InstructionType::SET, Operand8::C, 3);
        case 0xDA: return make_bitop(InstructionType::SET, Operand8::D, 3);
        case 0xDB: return make_bitop(InstructionType::SET, Operand8::E, 3);
        case 0xDC: return make_bitop(InstructionType::SET, Operand8::H, 3);
        case 0xDD: return make_bitop(InstructionType::SET, Operand8::L, 3);
        case 0xDE: return make_bitop(InstructionType::SET, Operand8::HL_PTR, 3);
        case 0xDF: return make_bitop(InstructionType::SET, Operand8::A, 3);

        // SET 4
        case 0xE0: return make_bitop(InstructionType::SET, Operand8::B, 4);
        case 0xE1: return make_bitop(InstructionType::SET, Operand8::C, 4);
        case 0xE2: return make_bitop(InstructionType::SET, Operand8::D, 4);
        case 0xE3: return make_bitop(InstructionType::SET, Operand8::E, 4);
        case 0xE4: return make_bitop(InstructionType::SET, Operand8::H, 4);
        case 0xE5: return make_bitop(InstructionType::SET, Operand8::L, 4);
        case 0xE6: return make_bitop(InstructionType::SET, Operand8::HL_PTR, 4);
        case 0xE7: return make_bitop(InstructionType::SET, Operand8::A, 4);

        // SET 5
        case 0xE8: return make_bitop(InstructionType::SET, Operand8::B, 5);
        case 0xE9: return make_bitop(InstructionType::SET, Operand8::C, 5);
        case 0xEA: return make_bitop(InstructionType::SET, Operand8::D, 5);
        case 0xEB: return make_bitop(InstructionType::SET, Operand8::E, 5);
        case 0xEC: return make_bitop(InstructionType::SET, Operand8::H, 5);
        case 0xED: return make_bitop(InstructionType::SET, Operand8::L, 5);
        case 0xEE: return make_bitop(InstructionType::SET, Operand8::HL_PTR, 5);
        case 0xEF: return make_bitop(InstructionType::SET, Operand8::A, 5);

        // SET 6
        case 0xF0: return make_bitop(InstructionType::SET, Operand8::B, 6);
        case 0xF1: return make_bitop(InstructionType::SET, Operand8::C, 6);
        case 0xF2: return make_bitop(InstructionType::SET, Operand8::D, 6);
        case 0xF3: return make_bitop(InstructionType::SET, Operand8::E, 6);
        case 0xF4: return make_bitop(InstructionType::SET, Operand8::H, 6);
        case 0xF5: return make_bitop(InstructionType::SET, Operand8::L, 6);
        case 0xF6: return make_bitop(InstructionType::SET, Operand8::HL_PTR, 6);
        case 0xF7: return make_bitop(InstructionType::SET, Operand8::A, 6);

        // SET 7
        case 0xF8: return make_bitop(InstructionType::SET, Operand8::B, 7);
        case 0xF9: return make_bitop(InstructionType::SET, Operand8::C, 7);
        case 0xFA: return make_bitop(InstructionType::SET, Operand8::D, 7);
        case 0xFB: return make_bitop(InstructionType::SET, Operand8::E, 7);
        case 0xFC: return make_bitop(InstructionType::SET, Operand8::H, 7);
        case 0xFD: return make_bitop(InstructionType::SET, Operand8::L, 7);
        case 0xFE: return make_bitop(InstructionType::SET, Operand8::HL_PTR, 7);
        case 0xFF: return make_bitop(InstructionType::SET, Operand8::A, 7);

        // --------------------
        // RES b, r
        // --------------------

        // RES 0
        case 0x80: return make_bitop(InstructionType::RES, Operand8::B, 0);
        case 0x81: return make_bitop(InstructionType::RES, Operand8::C, 0);
        case 0x82: return make_bitop(InstructionType::RES, Operand8::D, 0);
        case 0x83: return make_bitop(InstructionType::RES, Operand8::E, 0);
        case 0x84: return make_bitop(InstructionType::RES, Operand8::H, 0);
        case 0x85: return make_bitop(InstructionType::RES, Operand8::L, 0);
        case 0x86: return make_bitop(InstructionType::RES, Operand8::HL_PTR, 0);
        case 0x87: return make_bitop(InstructionType::RES, Operand8::A, 0);

        // RES 1
        case 0x88: return make_bitop(InstructionType::RES, Operand8::B, 1);
        case 0x89: return make_bitop(InstructionType::RES, Operand8::C, 1);
        case 0x8A: return make_bitop(InstructionType::RES, Operand8::D, 1);
        case 0x8B: return make_bitop(InstructionType::RES, Operand8::E, 1);
        case 0x8C: return make_bitop(InstructionType::RES, Operand8::H, 1);
        case 0x8D: return make_bitop(InstructionType::RES, Operand8::L, 1);
        case 0x8E: return make_bitop(InstructionType::RES, Operand8::HL_PTR, 1);
        case 0x8F: return make_bitop(InstructionType::RES, Operand8::A, 1);

        // RES 2
        case 0x90: return make_bitop(InstructionType::RES, Operand8::B, 2);
        case 0x91: return make_bitop(InstructionType::RES, Operand8::C, 2);
        case 0x92: return make_bitop(InstructionType::RES, Operand8::D, 2);
        case 0x93: return make_bitop(InstructionType::RES, Operand8::E, 2);
        case 0x94: return make_bitop(InstructionType::RES, Operand8::H, 2);
        case 0x95: return make_bitop(InstructionType::RES, Operand8::L, 2);
        case 0x96: return make_bitop(InstructionType::RES, Operand8::HL_PTR, 2);
        case 0x97: return make_bitop(InstructionType::RES, Operand8::A, 2);

        // RES 3
        case 0x98: return make_bitop(InstructionType::RES, Operand8::B, 3);
        case 0x99: return make_bitop(InstructionType::RES, Operand8::C, 3);
        case 0x9A: return make_bitop(InstructionType::RES, Operand8::D, 3);
        case 0x9B: return make_bitop(InstructionType::RES, Operand8::E, 3);
        case 0x9C: return make_bitop(InstructionType::RES, Operand8::H, 3);
        case 0x9D: return make_bitop(InstructionType::RES, Operand8::L, 3);
        case 0x9E: return make_bitop(InstructionType::RES, Operand8::HL_PTR, 3);
        case 0x9F: return make_bitop(InstructionType::RES, Operand8::A, 3);

        // RES 4
        case 0xA0: return make_bitop(InstructionType::RES, Operand8::B, 4);
        case 0xA1: return make_bitop(InstructionType::RES, Operand8::C, 4);
        case 0xA2: return make_bitop(InstructionType::RES, Operand8::D, 4);
        case 0xA3: return make_bitop(InstructionType::RES, Operand8::E, 4);
        case 0xA4: return make_bitop(InstructionType::RES, Operand8::H, 4);
        case 0xA5: return make_bitop(InstructionType::RES, Operand8::L, 4);
        case 0xA6: return make_bitop(InstructionType::RES, Operand8::HL_PTR, 4);
        case 0xA7: return make_bitop(InstructionType::RES, Operand8::A, 4);

        // RES 5
        case 0xA8: return make_bitop(InstructionType::RES, Operand8::B, 5);
        case 0xA9: return make_bitop(InstructionType::RES, Operand8::C, 5);
        case 0xAA: return make_bitop(InstructionType::RES, Operand8::D, 5);
        case 0xAB: return make_bitop(InstructionType::RES, Operand8::E, 5);
        case 0xAC: return make_bitop(InstructionType::RES, Operand8::H, 5);
        case 0xAD: return make_bitop(InstructionType::RES, Operand8::L, 5);
        case 0xAE: return make_bitop(InstructionType::RES, Operand8::HL_PTR, 5);
        case 0xAF: return make_bitop(InstructionType::RES, Operand8::A, 5);

        // RES 6
        case 0xB0: return make_bitop(InstructionType::RES, Operand8::B, 6);
        case 0xB1: return make_bitop(InstructionType::RES, Operand8::C, 6);
        case 0xB2: return make_bitop(InstructionType::RES, Operand8::D, 6);
        case 0xB3: return make_bitop(InstructionType::RES, Operand8::E, 6);
        case 0xB4: return make_bitop(InstructionType::RES, Operand8::H, 6);
        case 0xB5: return make_bitop(InstructionType::RES, Operand8::L, 6);
        case 0xB6: return make_bitop(InstructionType::RES, Operand8::HL_PTR, 6);
        case 0xB7: return make_bitop(InstructionType::RES, Operand8::A, 6);

        // RES 7
        case 0xB8: return make_bitop(InstructionType::RES, Operand8::B, 7);
        case 0xB9: return make_bitop(InstructionType::RES, Operand8::C, 7);
        case 0xBA: return make_bitop(InstructionType::RES, Operand8::D, 7);
        case 0xBB: return make_bitop(InstructionType::RES, Operand8::E, 7);
        case 0xBC: return make_bitop(InstructionType::RES, Operand8::H, 7);
        case 0xBD: return make_bitop(InstructionType::RES, Operand8::L, 7);
        case 0xBE: return make_bitop(InstructionType::RES, Operand8::HL_PTR, 7);
        case 0xBF: return make_bitop(InstructionType::RES, Operand8::A, 7);

        // --------------------
        // RLC r
        // --------------------
        case 0x00: return make_alu8(InstructionType::RLC, Operand8::B);
        case 0x01: return make_alu8(InstructionType::RLC, Operand8::C);
        case 0x02: return make_alu8(InstructionType::RLC, Operand8::D);
        case 0x03: return make_alu8(InstructionType::RLC, Operand8::E);
        case 0x04: return make_alu8(InstructionType::RLC, Operand8::H);
        case 0x05: return make_alu8(InstructionType::RLC, Operand8::L);
        case 0x06: return make_alu8(InstructionType::RLC, Operand8::HL_PTR);
        case 0x07: return make_alu8(InstructionType::RLC, Operand8::A);

        // --------------------
        // RL r
        // --------------------
        case 0x10: return make_alu8(InstructionType::RL, Operand8::B);
        case 0x11: return make_alu8(InstructionType::RL, Operand8::C);
        case 0x12: return make_alu8(InstructionType::RL, Operand8::D);
        case 0x13: return make_alu8(InstructionType::RL, Operand8::E);
        case 0x14: return make_alu8(InstructionType::RL, Operand8::H);
        case 0x15: return make_alu8(InstructionType::RL, Operand8::L);
        case 0x16: return make_alu8(InstructionType::RL, Operand8::HL_PTR);
        case 0x17: return make_alu8(InstructionType::RL, Operand8::A);

        // --------------------
        // RRC r
        // --------------------
        case 0x08: return make_alu8(InstructionType::RRC, Operand8::B);
        case 0x09: return make_alu8(InstructionType::RRC, Operand8::C);
        case 0x0A: return make_alu8(InstructionType::RRC, Operand8::D);
        case 0x0B: return make_alu8(InstructionType::RRC, Operand8::E);
        case 0x0C: return make_alu8(InstructionType::RRC, Operand8::H);
        case 0x0D: return make_alu8(InstructionType::RRC, Operand8::L);
        case 0x0E: return make_alu8(InstructionType::RRC, Operand8::HL_PTR);
        case 0x0F: return make_alu8(InstructionType::RRC, Operand8::A);

        // --------------------
        // RR r
        // --------------------
        case 0x18: return make_alu8(InstructionType::RR, Operand8::B);
        case 0x19: return make_alu8(InstructionType::RR, Operand8::C);
        case 0x1A: return make_alu8(InstructionType::RR, Operand8::D);
        case 0x1B: return make_alu8(InstructionType::RR, Operand8::E);
        case 0x1C: return make_alu8(InstructionType::RR, Operand8::H);
        case 0x1D: return make_alu8(InstructionType::RR, Operand8::L);
        case 0x1E: return make_alu8(InstructionType::RR, Operand8::HL_PTR);
        case 0x1F: return make_alu8(InstructionType::RR, Operand8::A);

        // --------------------
        // SLA r
        // --------------------
        case 0x20: return make_alu8(InstructionType::SLA, Operand8::B);
        case 0x21: return make_alu8(InstructionType::SLA, Operand8::C);
        case 0x22: return make_alu8(InstructionType::SLA, Operand8::D);
        case 0x23: return make_alu8(InstructionType::SLA, Operand8::E);
        case 0x24: return make_alu8(InstructionType::SLA, Operand8::H);
        case 0x25: return make_alu8(InstructionType::SLA, Operand8::L);
        case 0x26: return make_alu8(InstructionType::SLA, Operand8::HL_PTR);
        case 0x27: return make_alu8(InstructionType::SLA, Operand8::A);

        // --------------------
        // SRA r
        // --------------------
        case 0x28: return make_alu8(InstructionType::SRA, Operand8::B);
        case 0x29: return make_alu8(InstructionType::SRA, Operand8::C);
        case 0x2A: return make_alu8(InstructionType::SRA, Operand8::D);
        case 0x2B: return make_alu8(InstructionType::SRA, Operand8::E);
        case 0x2C: return make_alu8(InstructionType::SRA, Operand8::H);
        case 0x2D: return make_alu8(InstructionType::SRA, Operand8::L);
        case 0x2E: return make_alu8(InstructionType::SRA, Operand8::HL_PTR);
        case 0x2F: return make_alu8(InstructionType::SRA, Operand8::A);

        // --------------------
        // SRL r
        // --------------------
        case 0x38: return make_alu8(InstructionType::SRL, Operand8::B);
        case 0x39: return make_alu8(InstructionType::SRL, Operand8::C);
        case 0x3A: return make_alu8(InstructionType::SRL, Operand8::D);
        case 0x3B: return make_alu8(InstructionType::SRL, Operand8::E);
        case 0x3C: return make_alu8(InstructionType::SRL, Operand8::H);
        case 0x3D: return make_alu8(InstructionType::SRL, Operand8::L);
        case 0x3E: return make_alu8(InstructionType::SRL, Operand8::HL_PTR);
        case 0x3F: return make_alu8(InstructionType::SRL, Operand8::A);

        default: return std::nullopt;
    }
}

static std::optional<Instruction> from_byte_not_prefixed(uint8_t byte) {
    auto make_alu8 = [](InstructionType t, Operand8 op) -> std::optional<Instruction> {
        Instruction inst{};
        inst.type = t;
        inst.alu8.target = op;
        return inst;
    };

    auto make_alu16 = [](InstructionType t, Operand16 op) -> std::optional<Instruction> {
        Instruction inst{};
        inst.type = t;
        inst.alu16.target = op;
        return inst;
    };

    auto make_simple = [](InstructionType t) -> std::optional<Instruction> {
        Instruction inst{};
        inst.type = t;
        return inst;
    };

    auto make_jump = [](InstructionType t, JumpTest cond) -> std::optional<Instruction> {
        Instruction inst{};
        inst.type = t;
        inst.jump.condition = cond;
        return inst;
    };

    switch (byte) {
        // --------------------
        // ADD A, r
        // --------------------
        case 0x80: return make_alu8(InstructionType::ADD, Operand8::B);
        case 0x81: return make_alu8(InstructionType::ADD, Operand8::C);
        case 0x82: return make_alu8(InstructionType::ADD, Operand8::D);
        case 0x83: return make_alu8(InstructionType::ADD, Operand8::E);
        case 0x84: return make_alu8(InstructionType::ADD, Operand8::H);
        case 0x85: return make_alu8(InstructionType::ADD, Operand8::L);
        case 0x86: return make_alu8(InstructionType::ADD, Operand8::HL_PTR);
        case 0x87: return make_alu8(InstructionType::ADD, Operand8::A);
        case 0xC6: return make_alu8(InstructionType::ADD, Operand8::NUM);

        // --------------------
        // ADC A, r
        // --------------------
        case 0x88: return make_alu8(InstructionType::ADC, Operand8::B);
        case 0x89: return make_alu8(InstructionType::ADC, Operand8::C);
        case 0x8A: return make_alu8(InstructionType::ADC, Operand8::D);
        case 0x8B: return make_alu8(InstructionType::ADC, Operand8::E);
        case 0x8C: return make_alu8(InstructionType::ADC, Operand8::H);
        case 0x8D: return make_alu8(InstructionType::ADC, Operand8::L);
        case 0x8E: return make_alu8(InstructionType::ADC, Operand8::HL_PTR);
        case 0x8F: return make_alu8(InstructionType::ADC, Operand8::A);
        case 0xCE: return make_alu8(InstructionType::ADC, Operand8::NUM);

        // --------------------
        // SUB r
        // --------------------
        case 0x90: return make_alu8(InstructionType::SUB, Operand8::B);
        case 0x91: return make_alu8(InstructionType::SUB, Operand8::C);
        case 0x92: return make_alu8(InstructionType::SUB, Operand8::D);
        case 0x93: return make_alu8(InstructionType::SUB, Operand8::E);
        case 0x94: return make_alu8(InstructionType::SUB, Operand8::H);
        case 0x95: return make_alu8(InstructionType::SUB, Operand8::L);
        case 0x96: return make_alu8(InstructionType::SUB, Operand8::HL_PTR);
        case 0x97: return make_alu8(InstructionType::SUB, Operand8::A);
        case 0xD6: return make_alu8(InstructionType::SUB, Operand8::NUM);

        // --------------------
        // SBC r
        // --------------------
        case 0x98: return make_alu8(InstructionType::SBC, Operand8::B);
        case 0x99: return make_alu8(InstructionType::SBC, Operand8::C);
        case 0x9A: return make_alu8(InstructionType::SBC, Operand8::D);
        case 0x9B: return make_alu8(InstructionType::SBC, Operand8::E);
        case 0x9C: return make_alu8(InstructionType::SBC, Operand8::H);
        case 0x9D: return make_alu8(InstructionType::SBC, Operand8::L);
        case 0x9E: return make_alu8(InstructionType::SBC, Operand8::HL_PTR);
        case 0x9F: return make_alu8(InstructionType::SBC, Operand8::A);
        case 0xDE: return make_alu8(InstructionType::SBC, Operand8::NUM);

        // --------------------
        // AND r
        // --------------------
        case 0xA0: return make_alu8(InstructionType::AND, Operand8::B);
        case 0xA1: return make_alu8(InstructionType::AND, Operand8::C);
        case 0xA2: return make_alu8(InstructionType::AND, Operand8::D);
        case 0xA3: return make_alu8(InstructionType::AND, Operand8::E);
        case 0xA4: return make_alu8(InstructionType::AND, Operand8::H);
        case 0xA5: return make_alu8(InstructionType::AND, Operand8::L);
        case 0xA6: return make_alu8(InstructionType::AND, Operand8::HL_PTR);
        case 0xA7: return make_alu8(InstructionType::AND, Operand8::A);
        case 0xE6: return make_alu8(InstructionType::AND, Operand8::NUM);

        // --------------------
        // OR r
        // --------------------
        case 0xB0: return make_alu8(InstructionType::OR, Operand8::B);
        case 0xB1: return make_alu8(InstructionType::OR, Operand8::C);
        case 0xB2: return make_alu8(InstructionType::OR, Operand8::D);
        case 0xB3: return make_alu8(InstructionType::OR, Operand8::E);
        case 0xB4: return make_alu8(InstructionType::OR, Operand8::H);
        case 0xB5: return make_alu8(InstructionType::OR, Operand8::L);
        case 0xB6: return make_alu8(InstructionType::OR, Operand8::HL_PTR);
        case 0xB7: return make_alu8(InstructionType::OR, Operand8::A);
        case 0xF6: return make_alu8(InstructionType::OR, Operand8::NUM);

        // --------------------
        // XOR r
        // --------------------
        case 0xA8: return make_alu8(InstructionType::XOR, Operand8::B);
        case 0xA9: return make_alu8(InstructionType::XOR, Operand8::C);
        case 0xAA: return make_alu8(InstructionType::XOR, Operand8::D);
        case 0xAB: return make_alu8(InstructionType::XOR, Operand8::E);
        case 0xAC: return make_alu8(InstructionType::XOR, Operand8::H);
        case 0xAD: return make_alu8(InstructionType::XOR, Operand8::L);
        case 0xAE: return make_alu8(InstructionType::XOR, Operand8::HL_PTR);
        case 0xAF: return make_alu8(InstructionType::XOR, Operand8::A);
        case 0xEE: return make_alu8(InstructionType::XOR, Operand8::NUM);

        // --------------------
        // CP r (compare)
        // --------------------
        case 0xB8: return make_alu8(InstructionType::CP, Operand8::B);
        case 0xB9: return make_alu8(InstructionType::CP, Operand8::C);
        case 0xBA: return make_alu8(InstructionType::CP, Operand8::D);
        case 0xBB: return make_alu8(InstructionType::CP, Operand8::E);
        case 0xBC: return make_alu8(InstructionType::CP, Operand8::H);
        case 0xBD: return make_alu8(InstructionType::CP, Operand8::L);
        case 0xBE: return make_alu8(InstructionType::CP, Operand8::HL_PTR);
        case 0xBF: return make_alu8(InstructionType::CP, Operand8::A);
        case 0xFE: return make_alu8(InstructionType::CP, Operand8::NUM);


        // --------------------
        // INC r
        // --------------------
        case 0x3C: return make_alu8(InstructionType::INC, Operand8::A);
        case 0x04: return make_alu8(InstructionType::INC, Operand8::B);
        case 0x0C: return make_alu8(InstructionType::INC, Operand8::C);
        case 0x14: return make_alu8(InstructionType::INC, Operand8::D);
        case 0x1C: return make_alu8(InstructionType::INC, Operand8::E);
        case 0x24: return make_alu8(InstructionType::INC, Operand8::H);
        case 0x2C: return make_alu8(InstructionType::INC, Operand8::L);
        case 0x34: return make_alu8(InstructionType::INC, Operand8::HL_PTR);

        // --------------------
        // DEC r
        // --------------------
        case 0x3D: return make_alu8(InstructionType::DEC, Operand8::A);
        case 0x05: return make_alu8(InstructionType::DEC, Operand8::B);
        case 0x0D: return make_alu8(InstructionType::DEC, Operand8::C);
        case 0x15: return make_alu8(InstructionType::DEC, Operand8::D);
        case 0x1D: return make_alu8(InstructionType::DEC, Operand8::E);
        case 0x25: return make_alu8(InstructionType::DEC, Operand8::H);
        case 0x2D: return make_alu8(InstructionType::DEC, Operand8::L);
        case 0x35: return make_alu8(InstructionType::DEC, Operand8::HL_PTR);

        // --------------------
        // CPL
        // --------------------
        case 0x2F: return make_simple(InstructionType::CPL);

        // --------------------
        // CCF
        // --------------------
        case 0x3F: return make_simple(InstructionType::CCF);

        // --------------------
        // SCF
        // --------------------
        case 0x37: return make_simple(InstructionType::SCF);

        // --------------------
        // ADDHL HL, r
        // --------------------
        case 0x09: return make_alu16(InstructionType::ADDHL, Operand16::BC);
        case 0x19: return make_alu16(InstructionType::ADDHL, Operand16::DE);
        case 0x29: return make_alu16(InstructionType::ADDHL, Operand16::HL);
        case 0x39: return make_alu16(InstructionType::ADDHL, Operand16::SP);

        // --------------------
        // RLCA
        // --------------------
        case 0x07: return make_simple(InstructionType::RLCA);

        // --------------------
        // RLA
        // --------------------
        case 0x17: return make_simple(InstructionType::RLA);

        // --------------------
        // RRCA
        // --------------------
        case 0x0F: return make_simple(InstructionType::RRCA);

        // --------------------
        // RRA
        // --------------------
        case 0x1F: return make_simple(InstructionType::RRA);

        // --------------------
        // JP
        // --------------------
        case 0xC3: return make_jump(InstructionType::JP, JumpTest::Always);
        case 0xC2: return make_jump(InstructionType::JP, JumpTest::NotZero);
        case 0xCA: return make_jump(InstructionType::JP, JumpTest::Zero);
        case 0xD2: return make_jump(InstructionType::JP, JumpTest::NotCarry);
        case 0xDA: return make_jump(InstructionType::JP, JumpTest::Carry);

        // --------------------
        // JR
        // --------------------
        case 0x18: return make_jump(InstructionType::JR, JumpTest::Always);
        case 0x20: return make_jump(InstructionType::JR, JumpTest::NotZero);
        case 0x28: return make_jump(InstructionType::JR, JumpTest::Zero);
        case 0x30: return make_jump(InstructionType::JR, JumpTest::NotCarry);
        case 0x38: return make_jump(InstructionType::JR, JumpTest::Carry);

        // --------------------
        // JP_HL
        // --------------------
        case 0xE9: return make_simple(InstructionType::JP_HL);

        default: return std::nullopt;
    }
}

static std::optional<Instruction> from_byte(uint8_t byte, bool prefixed) {
    if (prefixed) {
        return from_byte_prefixed(byte);
    }
    else {
        return from_byte_not_prefixed(byte);
    }
}

struct MemoryBus {
    uint8_t memory[0xFFFF];

    uint8_t read_byte(uint16_t addr) {
        return memory[addr];
    }

    void write_byte(uint16_t addr, uint8_t byte) {
        memory[addr] = byte;

        return;
    }
};

class CPU {
    public:
        Registers registers;
        MemoryBus bus;

        uint16_t pc = 0x100;
        uint16_t sp = 0xFFFE;

        void step() {
            // Use program counter to read instruction byte from memory
            uint8_t instruction_byte = bus.read_byte(pc);
            bool prefixed = instruction_byte == 0xCB;

            if (prefixed) {
                instruction_byte = bus.read_byte(pc + 1);
            }

            // Decode instruction byte into an instruction
            std::optional<Instruction> opt_instruct = from_byte(instruction_byte, prefixed);
            if (opt_instruct.has_value()) {
                Instruction instruct = opt_instruct.value();
                execute(instruct);
            }
            else {
                std::string description = "";

                if (prefixed) {
                    description = "Unkown instruction found for: 0xCB" + std::to_string(instruction_byte);
                }
                else {
                    description = "Unkown instruction found for: 0x" + std::to_string(instruction_byte);
                }

                throw std::runtime_error("Unkown instruction found for: 0x" + std::to_string(instruction_byte));
            }
        }

        void execute(const Instruction& instruction) {
            switch (instruction.type) {

                /* 8-bit Arithmetic Operations */

                // ADD Instruction adds specified register contents to register A
                case InstructionType::ADD: {
                    uint8_t value = 0;
                    bool inc_pc_by_two = false;

                    switch (instruction.alu8.target) {
                        case Operand8::A:
                            value = registers.a;
                            break;
                        case Operand8::B:
                            value = registers.b;
                            break;
                        case Operand8::C:
                            value = registers.c;
                            break;
                        case Operand8::D:
                            value = registers.d;
                            break;
                        case Operand8::E:
                            value = registers.e;
                            break;
                        case Operand8::H:
                            value = registers.h;
                            break;
                        case Operand8::L:
                            value = registers.l;
                            break;
                        case Operand8::HL_PTR: {
                            uint16_t addr = registers.get_hl();
                            value = bus.read_byte(addr);
                            break;
                        }
                        case Operand8::NUM:
                            value = bus.read_byte(pc + 1);
                            inc_pc_by_two = true;
                            break;
                        default:
                            break;
                    }

                    uint8_t new_value = add(value);
                    registers.a = new_value;

                    if (inc_pc_by_two) {
                        pc = pc + 2;
                    }
                    else {
                        pc = pc + 1;
                    }

                    break;
                }

                // ADC Instruction adds specified register contents + current carry flag value to register A
                case InstructionType::ADC: {
                    uint8_t value = 0;
                    bool inc_pc_by_two = false;

                    switch (instruction.alu8.target) {
                        case Operand8::A:
                            value = registers.a;
                            break;
                        case Operand8::B:
                            value = registers.b;
                            break;
                        case Operand8::C:
                            value = registers.c;
                            break;
                        case Operand8::D:
                            value = registers.d;
                            break;
                        case Operand8::E:
                            value = registers.e;
                            break;
                        case Operand8::H:
                            value = registers.h;
                            break;
                        case Operand8::L:
                            value = registers.l;
                            break;
                        case Operand8::HL_PTR: {
                            uint16_t addr = registers.get_hl();
                            value = bus.read_byte(addr);
                            break;
                        }
                        case Operand8::NUM:
                            value = bus.read_byte(pc + 1);
                            inc_pc_by_two = true;
                            break;
                        default:
                            break;
                    }

                    uint8_t new_value = adc(value);
                    registers.a = new_value;

                    if (inc_pc_by_two) {
                        pc = pc + 2;
                    }
                    else {
                        pc = pc + 1;
                    }

                    break;
                }

                // SUB Instruction subtracts specfified register contents from register A
                case InstructionType::SUB: {
                    uint8_t value = 0;
                    bool inc_pc_by_two = false;

                    switch (instruction.alu8.target) {
                        case Operand8::A:
                            value = registers.a;
                            break;
                        case Operand8::B:
                            value = registers.b;
                            break;
                        case Operand8::C:
                            value = registers.c;
                            break;
                        case Operand8::D:
                            value = registers.d;
                            break;
                        case Operand8::E:
                            value = registers.e;
                            break;
                        case Operand8::H:
                            value = registers.h;
                            break;
                        case Operand8::L:
                            value = registers.l;
                            break;
                        case Operand8::HL_PTR: {
                            uint16_t addr = registers.get_hl();
                            value = bus.read_byte(addr);
                            break;
                        }
                        case Operand8::NUM:
                            value = bus.read_byte(pc + 1);
                            inc_pc_by_two = true;
                            break;
                        default:
                            break;
                    }

                    uint8_t new_value = sub(value);
                    registers.a = new_value;

                    if (inc_pc_by_two) {
                        pc = pc + 2;
                    }
                    else {
                        pc = pc + 1;
                    }

                    break;
                }

                // SBC Instruction substracts specified register contents + carry flag from register A
                case InstructionType::SBC: {
                    uint8_t value = 0;
                    bool inc_pc_by_two = false;

                    switch (instruction.alu8.target) {
                        case Operand8::A:
                            value = registers.a;
                            break;
                        case Operand8::B:
                            value = registers.b;
                            break;
                        case Operand8::C:
                            value = registers.c;
                            break;
                        case Operand8::D:
                            value = registers.d;
                            break;
                        case Operand8::E:
                            value = registers.e;
                            break;
                        case Operand8::H:
                            value = registers.h;
                            break;
                        case Operand8::L:
                            value = registers.l;
                            break;
                        case Operand8::HL_PTR: {
                            uint16_t addr = registers.get_hl();
                            value = bus.read_byte(addr);
                            break;
                        }
                        case Operand8::NUM:
                            value = bus.read_byte(pc + 1);
                            inc_pc_by_two = true;
                            break;
                        default:
                            break;
                    }

                    uint8_t new_value = sbc(value);
                    registers.a = new_value;

                    if (inc_pc_by_two) {
                        pc = pc + 2;
                    }
                    else {
                        pc = pc + 1;
                    }

                    break;
                }

                // AND Instruction performs bitwise AND operation using specified register contents with register A
                case InstructionType::AND: {
                    uint8_t value = 0;
                    bool inc_pc_by_two = false;

                    switch (instruction.alu8.target) {
                        case Operand8::A:
                            value = registers.a;
                            break;
                        case Operand8::B:
                            value = registers.b;
                            break;
                        case Operand8::C:
                            value = registers.c;
                            break;
                        case Operand8::D:
                            value = registers.d;
                            break;
                        case Operand8::E:
                            value = registers.e;
                            break;
                        case Operand8::H:
                            value = registers.h;
                            break;
                        case Operand8::L:
                            value = registers.l;
                            break;
                        case Operand8::HL_PTR: {
                            uint16_t addr = registers.get_hl();
                            value = bus.read_byte(addr);
                            break;
                        }
                        case Operand8::NUM:
                            value = bus.read_byte(pc + 1);
                            inc_pc_by_two = true;
                            break;
                        default:
                            break;
                    }

                    uint8_t new_value = bitwise_and(value);
                    registers.a = new_value;

                    if (inc_pc_by_two) {
                        pc = pc + 2;
                    }
                    else {
                        pc = pc + 1;
                    }

                    break;
                }

                // OR Instruction performs bitwise OR operation using specified register contents with register A
                case InstructionType::OR: {
                    uint8_t value = 0;
                    bool inc_pc_by_two = false;

                    switch (instruction.alu8.target) {
                        case Operand8::A:
                            value = registers.a;
                            break;
                        case Operand8::B:
                            value = registers.b;
                            break;
                        case Operand8::C:
                            value = registers.c;
                            break;
                        case Operand8::D:
                            value = registers.d;
                            break;
                        case Operand8::E:
                            value = registers.e;
                            break;
                        case Operand8::H:
                            value = registers.h;
                            break;
                        case Operand8::L:
                            value = registers.l;
                            break;
                        case Operand8::HL_PTR: {
                            uint16_t addr = registers.get_hl();
                            value = bus.read_byte(addr);
                            break;
                        }
                        case Operand8::NUM:
                            value = bus.read_byte(pc + 1);
                            inc_pc_by_two = true;
                            break;
                        default:
                            break;
                    }

                    uint8_t new_value = bitwise_or(value);
                    registers.a = new_value;

                    if (inc_pc_by_two) {
                        pc = pc + 2;
                    }
                    else {
                        pc = pc + 1;
                    }

                    break;
                }

                // XOR Instruction performs bitwise XOR operation using specified register contents with register A
                case InstructionType::XOR: {
                    uint8_t value = 0;
                    bool inc_pc_by_two = false;

                    switch (instruction.alu8.target) {
                        case Operand8::A:
                            value = registers.a;
                            break;
                        case Operand8::B:
                            value = registers.b;
                            break;
                        case Operand8::C:
                            value = registers.c;
                            break;
                        case Operand8::D:
                            value = registers.d;
                            break;
                        case Operand8::E:
                            value = registers.e;
                            break;
                        case Operand8::H:
                            value = registers.h;
                            break;
                        case Operand8::L:
                            value = registers.l;
                            break;
                        case Operand8::HL_PTR: {
                            uint16_t addr = registers.get_hl();
                            value = bus.read_byte(addr);
                            break;
                        }
                        case Operand8::NUM:
                            value = bus.read_byte(pc + 1);
                            inc_pc_by_two = true;
                            break;
                        default:
                            break;
                    }

                    uint8_t new_value = bitwise_xor(value);
                    registers.a = new_value;

                    if (inc_pc_by_two) {
                        pc = pc + 2;
                    }
                    else {
                        pc = pc + 1;
                    }

                    break;
                }

                // CP Instruction subtracts specfified register contents from register A but doesnt store the result
                case InstructionType::CP: {
                    uint8_t value = 0;
                    bool inc_pc_by_two = false;

                    switch (instruction.alu8.target) {
                        case Operand8::A:
                            value = registers.a;
                            break;
                        case Operand8::B:
                            value = registers.b;
                            break;
                        case Operand8::C:
                            value = registers.c;
                            break;
                        case Operand8::D:
                            value = registers.d;
                            break;
                        case Operand8::E:
                            value = registers.e;
                            break;
                        case Operand8::H:
                            value = registers.h;
                            break;
                        case Operand8::L:
                            value = registers.l;
                            break;
                        case Operand8::HL_PTR: {
                            uint16_t addr = registers.get_hl();
                            value = bus.read_byte(addr);
                            break;
                        }
                        case Operand8::NUM:
                            value = bus.read_byte(pc + 1);
                            inc_pc_by_two = true;
                            break;
                        default:
                            break;
                    }

                    uint8_t new_value = sub(value);

                    if (inc_pc_by_two) {
                        pc = pc + 2;
                    }
                    else {
                        pc = pc + 1;
                    }

                    break;
                }

                // INC Instruction increments value of target register
                case InstructionType::INC: {
                    switch (instruction.alu8.target) {
                        case Operand8::A:
                            registers.a = inc(registers.a);
                            break;
                        case Operand8::B:
                            registers.b = inc(registers.b);
                            break;
                        case Operand8::C:
                            registers.c = inc(registers.c);
                            break;
                        case Operand8::D:
                            registers.d = inc(registers.d);
                            break;
                        case Operand8::E:
                            registers.e = inc(registers.e);
                            break;
                        case Operand8::H:
                            registers.h = inc(registers.h);
                            break;
                        case Operand8::L:
                            registers.l = inc(registers.l);
                            break;
                        case Operand8::HL_PTR: {
                            uint16_t addr = registers.get_hl();
                            uint8_t value = bus.read_byte(addr);
                            uint8_t result = inc(value);

                            bus.write_byte(addr, result);

                            break;
                        }
                        case Operand8::NUM:
                            break;
                        default:
                            break;
                    }

                    pc = pc + 1;

                    break;
                }

                // DEC Instruction increments value of target register
                case InstructionType::DEC: {
                    switch (instruction.alu8.target) {
                        case Operand8::A:
                            registers.a = dec(registers.a);
                            break;
                        case Operand8::B:
                            registers.b = dec(registers.b);
                            break;
                        case Operand8::C:
                            registers.c = dec(registers.c);
                            break;
                        case Operand8::D:
                            registers.d = dec(registers.d);
                            break;
                        case Operand8::E:
                            registers.e = dec(registers.e);
                            break;
                        case Operand8::H:
                            registers.h = dec(registers.h);
                            break;
                        case Operand8::L:
                            registers.l = dec(registers.l);
                            break;
                        case Operand8::HL_PTR: {
                            uint16_t addr = registers.get_hl();
                            uint8_t value = bus.read_byte(addr);
                            uint8_t result = dec(value);

                            bus.write_byte(addr, result);

                            break;
                        }
                        case Operand8::NUM:
                            break;
                        default:
                            break;
                    }

                    pc = pc + 1;

                    break;
                }

                /* Misc CPU Instructions */

                // SWAP Instruction swaps the values of the upper and lower nibbles of the target register
                case InstructionType::SWAP: {
                    switch (instruction.alu8.target) {
                        case Operand8::A:
                            registers.a = swap(registers.a);
                            break;
                        case Operand8::B:
                            registers.b = swap(registers.b);
                            break;
                        case Operand8::C:
                            registers.c = swap(registers.c);
                            break;
                        case Operand8::D:
                            registers.d = swap(registers.d);
                            break;
                        case Operand8::E:
                            registers.e = swap(registers.e);
                            break;
                        case Operand8::H:
                            registers.h = swap(registers.h);
                            break;
                        case Operand8::L:
                            registers.l = swap(registers.l);
                            break;
                        case Operand8::HL_PTR: {
                            uint16_t addr = registers.get_hl();
                            uint8_t value = bus.read_byte(addr);
                            uint8_t result = swap(value);

                            bus.write_byte(addr, result);

                            break;
                        }
                        case Operand8::NUM:
                            break;
                        default:
                            break;
                    }

                    pc = pc + 2;

                    break;
                }

                // SCF Instruction sets the carry flag, i.e. changes its value to True
                case InstructionType::SCF: {
                    registers.f.carry = true;

                    registers.f.zero = registers.f.zero; // SCF instruction leaves zero flag unaffected
                    registers.f.subtract = false;
                    registers.f.half_carry = false;

                    pc = pc + 1;

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

                    pc = pc + 1;

                    break;
                }

                // CPL Instruction toggles every bit of register A
                case InstructionType::CPL: {
                    registers.a = ~registers.a;

                    registers.f.zero = registers.f.zero; // CPL instruction leaves zero flag unaffected
                    registers.f.subtract = true;
                    registers.f.carry = registers.f.carry; // CPL instruction leaves carry flag unaffected
                    registers.f.half_carry = true;

                    pc = pc + 1;

                    break;
                }

                /* Bitwise CPU Instructions */

                // BIT Instruction tests the bit value for the given register at the given bit index
                case InstructionType::BIT: {
                    uint8_t bit_index = instruction.bitop.bit_index;
                    uint8_t value = 0;

                    switch (instruction.bitop.target) {
                        case Operand8::A:
                            value = registers.a;
                            break;
                        case Operand8::B:
                            value = registers.b;
                            break;
                        case Operand8::C:
                            value = registers.c;
                            break;
                        case Operand8::D:
                            value = registers.d;
                            break;
                        case Operand8::E:
                            value = registers.e;
                            break;
                        case Operand8::H:
                            value = registers.h;
                            break;
                        case Operand8::L:
                            value = registers.l;
                            break;
                        case Operand8::HL_PTR: {
                            uint16_t addr = registers.get_hl();
                            value = bus.read_byte(addr);
                            break;
                        }
                        case Operand8::NUM:
                            break;
                        default:
                            break;
                    }

                    uint8_t bit_value = (value >> bit_index) & (0b00000001);

                    registers.f.zero = bit_value == 0;
                    registers.f.subtract = false;
                    registers.f.carry = registers.f.carry; // BIT instruction leaves carry flag unaffected
                    registers.f.half_carry = true;

                    pc = pc + 2;

                    break;
                }

                // SET Instruction sets the bit value for the given register at the given bit index to 1
                case InstructionType::SET: {
                    uint8_t bit_index = instruction.bitop.bit_index;

                    switch (instruction.bitop.target) {
                        case Operand8::A:
                            registers.a = registers.a | (1 << bit_index);
                            break;
                        case Operand8::B:
                            registers.b = registers.b | (1 << bit_index);
                            break;
                        case Operand8::C:
                            registers.c = registers.c | (1 << bit_index);
                            break;
                        case Operand8::D:
                            registers.d = registers.d | (1 << bit_index);
                            break;
                        case Operand8::E:
                            registers.e = registers.e | (1 << bit_index);
                            break;
                        case Operand8::H:
                            registers.h = registers.h | (1 << bit_index);
                            break;
                        case Operand8::L:
                            registers.l = registers.l | (1 << bit_index);
                            break;
                        case Operand8::HL_PTR: {
                            uint16_t addr = registers.get_hl();
                            uint8_t value = bus.read_byte(addr);

                            uint8_t result = value | (1 << bit_index);

                            bus.write_byte(addr, result);

                            break;
                        }
                        case Operand8::NUM:
                            break;
                        default:
                            break;
                    }

                    pc = pc + 2;

                    break;
                }

                // RES Instruction sets the bit value for the given register at the given bit index to 0
                case InstructionType::RES: {
                    uint8_t bit_index = instruction.bitop.bit_index;

                    switch (instruction.bitop.target) {
                        case Operand8::A:
                            registers.a = registers.a & ~(1 << bit_index);
                            break;
                        case Operand8::B:
                            registers.b = registers.b & ~(1 << bit_index);
                            break;
                        case Operand8::C:
                            registers.c = registers.c & ~(1 << bit_index);
                            break;
                        case Operand8::D:
                            registers.d = registers.d & ~(1 << bit_index);
                            break;
                        case Operand8::E:
                            registers.e = registers.e & ~(1 << bit_index);
                            break;
                        case Operand8::H:
                            registers.h = registers.h & ~(1 << bit_index);
                            break;
                        case Operand8::L:
                            registers.l = registers.l & ~(1 << bit_index);
                            break;
                        case Operand8::HL_PTR: {
                            uint16_t addr = registers.get_hl();
                            uint8_t value = bus.read_byte(addr);

                            uint8_t result = value & ~(1 << bit_index);

                            bus.write_byte(addr, result);

                            break;
                        }
                        case Operand8::NUM:
                            break;
                        default:
                            break;
                    }

                    pc = pc + 2;

                    break;
                }

                /* 16-bit Arithmetic Operations */

                // ADDHL Instruction adds the contents of the given 16-bit register to the HL register
                case InstructionType::ADDHL: {
                    uint16_t value = 0;

                    switch (instruction.alu16.target) {
                        case Operand16::BC:
                            value = registers.get_bc();
                            break;
                        case Operand16::DE:
                            value = registers.get_de();
                            break;
                        case Operand16::HL:
                            value = registers.get_hl();
                            break;
                        case Operand16::SP:
                            value = sp;
                            break;
                    }

                    uint16_t new_value = add_hl(value);
                    registers.set_hl(new_value);

                    pc = pc + 1;

                    break;
                }

                /* Rotate & Shift CPU Instructions */

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

                    pc = pc + 1;

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

                    pc = pc + 1;

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

                    pc = pc + 1;

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

                    pc = pc + 1;

                    break;
                }

                // RLC Instruction rotates the contents of the proviced register 1 bit to the left, the carry flag is set to old bit 7
                case InstructionType::RLC: {
                    switch (instruction.alu8.target) {
                        case Operand8::A:
                            registers.a = rlc(registers.a);
                            break;
                        case Operand8::B:
                            registers.b = rlc(registers.b);
                            break;
                        case Operand8::C:
                            registers.c = rlc(registers.c);
                            break;
                        case Operand8::D:
                            registers.d = rlc(registers.d);
                            break;
                        case Operand8::E:
                            registers.e = rlc(registers.e);
                            break;
                        case Operand8::H:
                            registers.h = rlc(registers.h);
                            break;
                        case Operand8::L:
                            registers.l = rlc(registers.l);
                            break;
                        case Operand8::HL_PTR: {
                            uint16_t addr = registers.get_hl();
                            uint8_t value = bus.read_byte(addr);
                            uint8_t result = rlc(value);

                            bus.write_byte(addr, result);

                            break;
                        }
                        case Operand8::NUM:
                            break;
                        default:
                            break;
                    }

                    pc = pc + 2;

                    break;
                }

                // RL Instruction rotates the contents of the provided register 1 bit to the left, the carry flag is set to old bit 7, bit 0 is set to old carry flag value
                case InstructionType::RL: {
                    switch (instruction.alu8.target) {
                        case Operand8::A:
                            registers.a = rl(registers.a);
                            break;
                        case Operand8::B:
                            registers.b = rl(registers.b);
                            break;
                        case Operand8::C:
                            registers.c = rl(registers.c);
                            break;
                        case Operand8::D:
                            registers.d = rl(registers.d);
                            break;
                        case Operand8::E:
                            registers.e = rl(registers.e);
                            break;
                        case Operand8::H:
                            registers.h = rl(registers.h);
                            break;
                        case Operand8::L:
                            registers.l = rl(registers.l);
                            break;
                        case Operand8::HL_PTR: {
                            uint16_t addr = registers.get_hl();
                            uint8_t value = bus.read_byte(addr);
                            uint8_t result = rl(value);

                            bus.write_byte(addr, result);

                            break;
                        }
                        case Operand8::NUM:
                            break;
                        default:
                            break;
                    }

                    pc = pc + 2;

                    break;
                }

                // RRC Instruction rotates the contents of the provided register 1 bit to the right, the carry flag is set to old bit 0
                case InstructionType::RRC: {
                    switch (instruction.alu8.target) {
                        case Operand8::A:
                            registers.a = rrc(registers.a);
                            break;
                        case Operand8::B:
                            registers.b = rrc(registers.b);
                            break;
                        case Operand8::C:
                            registers.c = rrc(registers.c);
                            break;
                        case Operand8::D:
                            registers.d = rrc(registers.d);
                            break;
                        case Operand8::E:
                            registers.e = rrc(registers.e);
                            break;
                        case Operand8::H:
                            registers.h = rrc(registers.h);
                            break;
                        case Operand8::L:
                            registers.l = rrc(registers.l);
                            break;
                        case Operand8::HL_PTR: {
                            uint16_t addr = registers.get_hl();
                            uint8_t value = bus.read_byte(addr);
                            uint8_t result = rrc(value);

                            bus.write_byte(addr, result);

                            break;
                        }
                        case Operand8::NUM:
                            break;
                        default:
                            break;
                    }

                    pc = pc + 2;

                    break;
                }

                // RR Instruction rotates the contents of the provided register 1 bit to the right, the carry flag is rotated in for bit 7 (leftmost bit) and bit 0 is rotated out to the carry flag value
                case InstructionType::RR: {
                    switch (instruction.alu8.target) {
                        case Operand8::A:
                            registers.a = rr(registers.a);
                            break;
                        case Operand8::B:
                            registers.b = rr(registers.b);
                            break;
                        case Operand8::C:
                            registers.c = rr(registers.c);
                            break;
                        case Operand8::D:
                            registers.d = rr(registers.d);
                            break;
                        case Operand8::E:
                            registers.e = rr(registers.e);
                            break;
                        case Operand8::H:
                            registers.h = rr(registers.h);
                            break;
                        case Operand8::L:
                            registers.l = rr(registers.l);
                            break;
                        case Operand8::HL_PTR: {
                            uint16_t addr = registers.get_hl();
                            uint8_t value = bus.read_byte(addr);
                            uint8_t result = rr(value);

                            bus.write_byte(addr, result);

                            break;
                        }
                        case Operand8::NUM:
                            break;
                        default:
                            break;
                    }

                    pc = pc + 2;

                    break;
                }

                // SLA Instruction shifts the value of the provided register to the left by 1 bit and stores old bit 7 in carry
                case InstructionType::SLA: {
                    switch (instruction.alu8.target) {
                        case Operand8::A:
                            registers.a = sla(registers.a);
                            break;
                        case Operand8::B:
                            registers.b = sla(registers.b);
                            break;
                        case Operand8::C:
                            registers.c = sla(registers.c);
                            break;
                        case Operand8::D:
                            registers.d = sla(registers.d);
                            break;
                        case Operand8::E:
                            registers.e = sla(registers.e);
                            break;
                        case Operand8::H:
                            registers.h = sla(registers.h);
                            break;
                        case Operand8::L:
                            registers.l = sla(registers.l);
                            break;
                        case Operand8::HL_PTR: {
                            uint16_t addr = registers.get_hl();
                            uint8_t value = bus.read_byte(addr);
                            uint8_t result = sla(value);

                            bus.write_byte(addr, result);

                            break;
                        }
                        case Operand8::NUM:
                            break;
                        default:
                            break;
                    }

                    pc = pc + 2;

                    break;
                }

                // SRA Instruction shifts the value of the provided register to the right by 1 bit and stores old bit 0 in carry, the MSB remains the same (don't shift in 0 automatically)
                case InstructionType::SRA: {
                    switch (instruction.alu8.target) {
                        case Operand8::A:
                            registers.a = sra(registers.a);
                            break;
                        case Operand8::B:
                            registers.b = sra(registers.b);
                            break;
                        case Operand8::C:
                            registers.c = sra(registers.c);
                            break;
                        case Operand8::D:
                            registers.d = sra(registers.d);
                            break;
                        case Operand8::E:
                            registers.e = sra(registers.e);
                            break;
                        case Operand8::H:
                            registers.h = sra(registers.h);
                            break;
                        case Operand8::L:
                            registers.l = sra(registers.l);
                            break;
                        case Operand8::HL_PTR: {
                            uint16_t addr = registers.get_hl();
                            uint8_t value = bus.read_byte(addr);
                            uint8_t result = sra(value);

                            bus.write_byte(addr, result);

                            break;
                        }
                        case Operand8::NUM:
                            break;
                        default:
                            break;
                    }

                    pc = pc + 2;

                    break;
                }

                // SRL Instruction shifts the value of the provided register to the right by 1 bit and stores old bit 0 in carry
                case InstructionType::SRL: {
                    switch (instruction.alu8.target) {
                        case Operand8::A:
                            registers.a = srl(registers.a);
                            break;
                        case Operand8::B:
                            registers.b = srl(registers.b);
                            break;
                        case Operand8::C:
                            registers.c = srl(registers.c);
                            break;
                        case Operand8::D:
                            registers.d = srl(registers.d);
                            break;
                        case Operand8::E:
                            registers.e = srl(registers.e);
                            break;
                        case Operand8::H:
                            registers.h = srl(registers.h);
                            break;
                        case Operand8::L:
                            registers.l = srl(registers.l);
                            break;
                        case Operand8::HL_PTR: {
                            uint16_t addr = registers.get_hl();
                            uint8_t value = bus.read_byte(addr);
                            uint8_t result = srl(value);

                            bus.write_byte(addr, result);

                            break;
                        }
                        case Operand8::NUM:
                            break;
                        default:
                            break;
                    }

                    pc = pc + 2;

                    break;
                }

                /* Jump Instructions */

                case InstructionType::JP: {
                    bool jump_condition = false;

                    switch (instruction.jump.condition) {
                        case JumpTest::NotZero:
                            jump_condition = !registers.f.zero;
                            break;

                        case JumpTest::NotCarry:
                            jump_condition = !registers.f.carry;
                            break;

                        case JumpTest::Zero:
                            jump_condition = registers.f.zero;
                            break;

                        case JumpTest::Carry:
                            jump_condition = registers.f.carry;
                            break;

                        case JumpTest::Always:
                            jump_condition = true;
                            break;
                    }

                    pc = jump(jump_condition);

                    break;
                }

                case InstructionType::JR: {
                    bool jump_condition = false;

                    switch (instruction.jump.condition) {
                        case JumpTest::NotZero:
                            jump_condition = !registers.f.zero;
                            break;

                        case JumpTest::NotCarry:
                            jump_condition = !registers.f.carry;
                            break;

                        case JumpTest::Zero:
                            jump_condition = registers.f.zero;
                            break;

                        case JumpTest::Carry:
                            jump_condition = registers.f.carry;
                            break;

                        case JumpTest::Always:
                            jump_condition = true;
                            break;
                    }

                    pc = jump_relative(jump_condition);

                    break;
                }

                case InstructionType::JP_HL: {
                    pc = registers.get_hl();

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

        uint8_t rl(uint8_t value) {
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

        uint16_t jump(bool should_jump) {
            if (should_jump) {
                // Gameboy is little endian so read pc + 2 as most significant bit
                // and pc + 1 as least significant bit
                uint16_t least_significant_byte = bus.read_byte(pc + 1);
                uint16_t most_significant_byte = bus.read_byte(pc + 2);
                return (most_significant_byte << 8) | least_significant_byte;
            }
            else {
                // If we don't jump we need to still move the program
                // counter forward by 3 since the jump instruction is
                // 3 bytes wide (1 byte for tag and 2 bytes for jump address)
                return pc + 3;
            }
        }

        uint16_t jump_relative(bool should_jump) {
            int8_t offset = static_cast<int8_t>(bus.read_byte(pc + 1)); // Offset stored 1 byte after JR instruction

            // Always jump 2 after the instruction
            if (should_jump) {
                return static_cast<uint16_t>(pc + 2 + offset);
            } else {
                return pc + 2;
            }
        }
};