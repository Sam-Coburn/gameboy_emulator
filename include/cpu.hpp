#include <registers.hpp>

#include <bit>
#include <bitset>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <stdint.h>
#include <string>

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
    SET, RES, ADDHL, RLCA, RLA,
    RRCA, RRA, RLC, RL, RRC,
    RR, SLA, SRA, SRL
};

struct Instruction {
    InstructionType type;
    ArithmeticTarget8Bit target_8bit;
    ArithmeticTarget16Bit target_16bit;
    uint8_t bit_index;
};

static std::optional<Instruction> from_byte_prefixed(uint8_t byte) {
    switch (byte) {
        // --------------------
        // SWAP r
        // --------------------
        case 0x37: return Instruction{InstructionType::SWAP, ArithmeticTarget8Bit::A};
        case 0x30: return Instruction{InstructionType::SWAP, ArithmeticTarget8Bit::B};
        case 0x31: return Instruction{InstructionType::SWAP, ArithmeticTarget8Bit::C};
        case 0x32: return Instruction{InstructionType::SWAP, ArithmeticTarget8Bit::D};
        case 0x33: return Instruction{InstructionType::SWAP, ArithmeticTarget8Bit::E};
        case 0x34: return Instruction{InstructionType::SWAP, ArithmeticTarget8Bit::H};
        case 0x35: return Instruction{InstructionType::SWAP, ArithmeticTarget8Bit::L};

        // --------------------
        // BIT b, r
        // --------------------

        // BIT 0
        case 0x40: return Instruction{InstructionType::BIT, ArithmeticTarget8Bit::B, ArithmeticTarget16Bit::BC, 0};
        case 0x41: return Instruction{InstructionType::BIT, ArithmeticTarget8Bit::C, ArithmeticTarget16Bit::BC, 0};
        case 0x42: return Instruction{InstructionType::BIT, ArithmeticTarget8Bit::D, ArithmeticTarget16Bit::BC, 0};
        case 0x43: return Instruction{InstructionType::BIT, ArithmeticTarget8Bit::E, ArithmeticTarget16Bit::BC, 0};
        case 0x44: return Instruction{InstructionType::BIT, ArithmeticTarget8Bit::H, ArithmeticTarget16Bit::BC, 0};
        case 0x45: return Instruction{InstructionType::BIT, ArithmeticTarget8Bit::L, ArithmeticTarget16Bit::BC, 0};
        case 0x47: return Instruction{InstructionType::BIT, ArithmeticTarget8Bit::A, ArithmeticTarget16Bit::BC, 0};

        // BIT 1
        case 0x48: return Instruction{InstructionType::BIT, ArithmeticTarget8Bit::B, ArithmeticTarget16Bit::BC, 1};
        case 0x49: return Instruction{InstructionType::BIT, ArithmeticTarget8Bit::C, ArithmeticTarget16Bit::BC, 1};
        case 0x4A: return Instruction{InstructionType::BIT, ArithmeticTarget8Bit::D, ArithmeticTarget16Bit::BC, 1};
        case 0x4B: return Instruction{InstructionType::BIT, ArithmeticTarget8Bit::E, ArithmeticTarget16Bit::BC, 1};
        case 0x4C: return Instruction{InstructionType::BIT, ArithmeticTarget8Bit::H, ArithmeticTarget16Bit::BC, 1};
        case 0x4D: return Instruction{InstructionType::BIT, ArithmeticTarget8Bit::L, ArithmeticTarget16Bit::BC, 1};
        case 0x4F: return Instruction{InstructionType::BIT, ArithmeticTarget8Bit::A, ArithmeticTarget16Bit::BC, 1};

        // BIT 2
        case 0x50: return Instruction{InstructionType::BIT, ArithmeticTarget8Bit::B, ArithmeticTarget16Bit::BC, 2};
        case 0x51: return Instruction{InstructionType::BIT, ArithmeticTarget8Bit::C, ArithmeticTarget16Bit::BC, 2};
        case 0x52: return Instruction{InstructionType::BIT, ArithmeticTarget8Bit::D, ArithmeticTarget16Bit::BC, 2};
        case 0x53: return Instruction{InstructionType::BIT, ArithmeticTarget8Bit::E, ArithmeticTarget16Bit::BC, 2};
        case 0x54: return Instruction{InstructionType::BIT, ArithmeticTarget8Bit::H, ArithmeticTarget16Bit::BC, 2};
        case 0x55: return Instruction{InstructionType::BIT, ArithmeticTarget8Bit::L, ArithmeticTarget16Bit::BC, 2};
        case 0x57: return Instruction{InstructionType::BIT, ArithmeticTarget8Bit::A, ArithmeticTarget16Bit::BC, 2};

        // BIT 3
        case 0x58: return Instruction{InstructionType::BIT, ArithmeticTarget8Bit::B, ArithmeticTarget16Bit::BC, 3};
        case 0x59: return Instruction{InstructionType::BIT, ArithmeticTarget8Bit::C, ArithmeticTarget16Bit::BC, 3};
        case 0x5A: return Instruction{InstructionType::BIT, ArithmeticTarget8Bit::D, ArithmeticTarget16Bit::BC, 3};
        case 0x5B: return Instruction{InstructionType::BIT, ArithmeticTarget8Bit::E, ArithmeticTarget16Bit::BC, 3};
        case 0x5C: return Instruction{InstructionType::BIT, ArithmeticTarget8Bit::H, ArithmeticTarget16Bit::BC, 3};
        case 0x5D: return Instruction{InstructionType::BIT, ArithmeticTarget8Bit::L, ArithmeticTarget16Bit::BC, 3};
        case 0x5F: return Instruction{InstructionType::BIT, ArithmeticTarget8Bit::A, ArithmeticTarget16Bit::BC, 3};

        // BIT 4
        case 0x60: return Instruction{InstructionType::BIT, ArithmeticTarget8Bit::B, ArithmeticTarget16Bit::BC, 4};
        case 0x61: return Instruction{InstructionType::BIT, ArithmeticTarget8Bit::C, ArithmeticTarget16Bit::BC, 4};
        case 0x62: return Instruction{InstructionType::BIT, ArithmeticTarget8Bit::D, ArithmeticTarget16Bit::BC, 4};
        case 0x63: return Instruction{InstructionType::BIT, ArithmeticTarget8Bit::E, ArithmeticTarget16Bit::BC, 4};
        case 0x64: return Instruction{InstructionType::BIT, ArithmeticTarget8Bit::H, ArithmeticTarget16Bit::BC, 4};
        case 0x65: return Instruction{InstructionType::BIT, ArithmeticTarget8Bit::L, ArithmeticTarget16Bit::BC, 4};
        case 0x67: return Instruction{InstructionType::BIT, ArithmeticTarget8Bit::A, ArithmeticTarget16Bit::BC, 4};

        // BIT 5
        case 0x68: return Instruction{InstructionType::BIT, ArithmeticTarget8Bit::B, ArithmeticTarget16Bit::BC, 5};
        case 0x69: return Instruction{InstructionType::BIT, ArithmeticTarget8Bit::C, ArithmeticTarget16Bit::BC, 5};
        case 0x6A: return Instruction{InstructionType::BIT, ArithmeticTarget8Bit::D, ArithmeticTarget16Bit::BC, 5};
        case 0x6B: return Instruction{InstructionType::BIT, ArithmeticTarget8Bit::E, ArithmeticTarget16Bit::BC, 5};
        case 0x6C: return Instruction{InstructionType::BIT, ArithmeticTarget8Bit::H, ArithmeticTarget16Bit::BC, 5};
        case 0x6D: return Instruction{InstructionType::BIT, ArithmeticTarget8Bit::L, ArithmeticTarget16Bit::BC, 5};
        case 0x6F: return Instruction{InstructionType::BIT, ArithmeticTarget8Bit::A, ArithmeticTarget16Bit::BC, 5};

        // BIT 6
        case 0x70: return Instruction{InstructionType::BIT, ArithmeticTarget8Bit::B, ArithmeticTarget16Bit::BC, 6};
        case 0x71: return Instruction{InstructionType::BIT, ArithmeticTarget8Bit::C, ArithmeticTarget16Bit::BC, 6};
        case 0x72: return Instruction{InstructionType::BIT, ArithmeticTarget8Bit::D, ArithmeticTarget16Bit::BC, 6};
        case 0x73: return Instruction{InstructionType::BIT, ArithmeticTarget8Bit::E, ArithmeticTarget16Bit::BC, 6};
        case 0x74: return Instruction{InstructionType::BIT, ArithmeticTarget8Bit::H, ArithmeticTarget16Bit::BC, 6};
        case 0x75: return Instruction{InstructionType::BIT, ArithmeticTarget8Bit::L, ArithmeticTarget16Bit::BC, 6};
        case 0x77: return Instruction{InstructionType::BIT, ArithmeticTarget8Bit::A, ArithmeticTarget16Bit::BC, 6};

        // BIT 7
        case 0x78: return Instruction{InstructionType::BIT, ArithmeticTarget8Bit::B, ArithmeticTarget16Bit::BC, 7};
        case 0x79: return Instruction{InstructionType::BIT, ArithmeticTarget8Bit::C, ArithmeticTarget16Bit::BC, 7};
        case 0x7A: return Instruction{InstructionType::BIT, ArithmeticTarget8Bit::D, ArithmeticTarget16Bit::BC, 7};
        case 0x7B: return Instruction{InstructionType::BIT, ArithmeticTarget8Bit::E, ArithmeticTarget16Bit::BC, 7};
        case 0x7C: return Instruction{InstructionType::BIT, ArithmeticTarget8Bit::H, ArithmeticTarget16Bit::BC, 7};
        case 0x7D: return Instruction{InstructionType::BIT, ArithmeticTarget8Bit::L, ArithmeticTarget16Bit::BC, 7};
        case 0x7F: return Instruction{InstructionType::BIT, ArithmeticTarget8Bit::A, ArithmeticTarget16Bit::BC, 7};

        // --------------------
        // SET b, r
        // --------------------

        // SET 0
        case 0xC0: return Instruction{InstructionType::SET, ArithmeticTarget8Bit::B, ArithmeticTarget16Bit::BC, 0};
        case 0xC1: return Instruction{InstructionType::SET, ArithmeticTarget8Bit::C, ArithmeticTarget16Bit::BC, 0};
        case 0xC2: return Instruction{InstructionType::SET, ArithmeticTarget8Bit::D, ArithmeticTarget16Bit::BC, 0};
        case 0xC3: return Instruction{InstructionType::SET, ArithmeticTarget8Bit::E, ArithmeticTarget16Bit::BC, 0};
        case 0xC4: return Instruction{InstructionType::SET, ArithmeticTarget8Bit::H, ArithmeticTarget16Bit::BC, 0};
        case 0xC5: return Instruction{InstructionType::SET, ArithmeticTarget8Bit::L, ArithmeticTarget16Bit::BC, 0};
        case 0xC7: return Instruction{InstructionType::SET, ArithmeticTarget8Bit::A, ArithmeticTarget16Bit::BC, 0};

        // SET 1
        case 0xC8: return Instruction{InstructionType::SET, ArithmeticTarget8Bit::B, ArithmeticTarget16Bit::BC, 1};
        case 0xC9: return Instruction{InstructionType::SET, ArithmeticTarget8Bit::C, ArithmeticTarget16Bit::BC, 1};
        case 0xCA: return Instruction{InstructionType::SET, ArithmeticTarget8Bit::D, ArithmeticTarget16Bit::BC, 1};
        case 0xCB: return Instruction{InstructionType::SET, ArithmeticTarget8Bit::E, ArithmeticTarget16Bit::BC, 1};
        case 0xCC: return Instruction{InstructionType::SET, ArithmeticTarget8Bit::H, ArithmeticTarget16Bit::BC, 1};
        case 0xCD: return Instruction{InstructionType::SET, ArithmeticTarget8Bit::L, ArithmeticTarget16Bit::BC, 1};
        case 0xCF: return Instruction{InstructionType::SET, ArithmeticTarget8Bit::A, ArithmeticTarget16Bit::BC, 1};

        // SET 2
        case 0xD0: return Instruction{InstructionType::SET, ArithmeticTarget8Bit::B, ArithmeticTarget16Bit::BC, 2};
        case 0xD1: return Instruction{InstructionType::SET, ArithmeticTarget8Bit::C, ArithmeticTarget16Bit::BC, 2};
        case 0xD2: return Instruction{InstructionType::SET, ArithmeticTarget8Bit::D, ArithmeticTarget16Bit::BC, 2};
        case 0xD3: return Instruction{InstructionType::SET, ArithmeticTarget8Bit::E, ArithmeticTarget16Bit::BC, 2};
        case 0xD4: return Instruction{InstructionType::SET, ArithmeticTarget8Bit::H, ArithmeticTarget16Bit::BC, 2};
        case 0xD5: return Instruction{InstructionType::SET, ArithmeticTarget8Bit::L, ArithmeticTarget16Bit::BC, 2};
        case 0xD7: return Instruction{InstructionType::SET, ArithmeticTarget8Bit::A, ArithmeticTarget16Bit::BC, 2};

        // SET 3
        case 0xD8: return Instruction{InstructionType::SET, ArithmeticTarget8Bit::B, ArithmeticTarget16Bit::BC, 3};
        case 0xD9: return Instruction{InstructionType::SET, ArithmeticTarget8Bit::C, ArithmeticTarget16Bit::BC, 3};
        case 0xDA: return Instruction{InstructionType::SET, ArithmeticTarget8Bit::D, ArithmeticTarget16Bit::BC, 3};
        case 0xDB: return Instruction{InstructionType::SET, ArithmeticTarget8Bit::E, ArithmeticTarget16Bit::BC, 3};
        case 0xDC: return Instruction{InstructionType::SET, ArithmeticTarget8Bit::H, ArithmeticTarget16Bit::BC, 3};
        case 0xDD: return Instruction{InstructionType::SET, ArithmeticTarget8Bit::L, ArithmeticTarget16Bit::BC, 3};
        case 0xDF: return Instruction{InstructionType::SET, ArithmeticTarget8Bit::A, ArithmeticTarget16Bit::BC, 3};

        // SET 4
        case 0xE0: return Instruction{InstructionType::SET, ArithmeticTarget8Bit::B, ArithmeticTarget16Bit::BC, 4};
        case 0xE1: return Instruction{InstructionType::SET, ArithmeticTarget8Bit::C, ArithmeticTarget16Bit::BC, 4};
        case 0xE2: return Instruction{InstructionType::SET, ArithmeticTarget8Bit::D, ArithmeticTarget16Bit::BC, 4};
        case 0xE3: return Instruction{InstructionType::SET, ArithmeticTarget8Bit::E, ArithmeticTarget16Bit::BC, 4};
        case 0xE4: return Instruction{InstructionType::SET, ArithmeticTarget8Bit::H, ArithmeticTarget16Bit::BC, 4};
        case 0xE5: return Instruction{InstructionType::SET, ArithmeticTarget8Bit::L, ArithmeticTarget16Bit::BC, 4};
        case 0xE7: return Instruction{InstructionType::SET, ArithmeticTarget8Bit::A, ArithmeticTarget16Bit::BC, 4};

        // SET 5
        case 0xE8: return Instruction{InstructionType::SET, ArithmeticTarget8Bit::B, ArithmeticTarget16Bit::BC, 5};
        case 0xE9: return Instruction{InstructionType::SET, ArithmeticTarget8Bit::C, ArithmeticTarget16Bit::BC, 5};
        case 0xEA: return Instruction{InstructionType::SET, ArithmeticTarget8Bit::D, ArithmeticTarget16Bit::BC, 5};
        case 0xEB: return Instruction{InstructionType::SET, ArithmeticTarget8Bit::E, ArithmeticTarget16Bit::BC, 5};
        case 0xEC: return Instruction{InstructionType::SET, ArithmeticTarget8Bit::H, ArithmeticTarget16Bit::BC, 5};
        case 0xED: return Instruction{InstructionType::SET, ArithmeticTarget8Bit::L, ArithmeticTarget16Bit::BC, 5};
        case 0xEF: return Instruction{InstructionType::SET, ArithmeticTarget8Bit::A, ArithmeticTarget16Bit::BC, 5};

        // SET 6
        case 0xF0: return Instruction{InstructionType::SET, ArithmeticTarget8Bit::B, ArithmeticTarget16Bit::BC, 6};
        case 0xF1: return Instruction{InstructionType::SET, ArithmeticTarget8Bit::C, ArithmeticTarget16Bit::BC, 6};
        case 0xF2: return Instruction{InstructionType::SET, ArithmeticTarget8Bit::D, ArithmeticTarget16Bit::BC, 6};
        case 0xF3: return Instruction{InstructionType::SET, ArithmeticTarget8Bit::E, ArithmeticTarget16Bit::BC, 6};
        case 0xF4: return Instruction{InstructionType::SET, ArithmeticTarget8Bit::H, ArithmeticTarget16Bit::BC, 6};
        case 0xF5: return Instruction{InstructionType::SET, ArithmeticTarget8Bit::L, ArithmeticTarget16Bit::BC, 6};
        case 0xF7: return Instruction{InstructionType::SET, ArithmeticTarget8Bit::A, ArithmeticTarget16Bit::BC, 6};

        // SET 7
        case 0xF8: return Instruction{InstructionType::SET, ArithmeticTarget8Bit::B, ArithmeticTarget16Bit::BC, 7};
        case 0xF9: return Instruction{InstructionType::SET, ArithmeticTarget8Bit::C, ArithmeticTarget16Bit::BC, 7};
        case 0xFA: return Instruction{InstructionType::SET, ArithmeticTarget8Bit::D, ArithmeticTarget16Bit::BC, 7};
        case 0xFB: return Instruction{InstructionType::SET, ArithmeticTarget8Bit::E, ArithmeticTarget16Bit::BC, 7};
        case 0xFC: return Instruction{InstructionType::SET, ArithmeticTarget8Bit::H, ArithmeticTarget16Bit::BC, 7};
        case 0xFD: return Instruction{InstructionType::SET, ArithmeticTarget8Bit::L, ArithmeticTarget16Bit::BC, 7};
        case 0xFF: return Instruction{InstructionType::SET, ArithmeticTarget8Bit::A, ArithmeticTarget16Bit::BC, 7};

        // --------------------
        // RES b, r
        // --------------------

        // RES 0
        case 0x80: return Instruction{InstructionType::RES, ArithmeticTarget8Bit::B, ArithmeticTarget16Bit::BC, 0};
        case 0x81: return Instruction{InstructionType::RES, ArithmeticTarget8Bit::C, ArithmeticTarget16Bit::BC, 0};
        case 0x82: return Instruction{InstructionType::RES, ArithmeticTarget8Bit::D, ArithmeticTarget16Bit::BC, 0};
        case 0x83: return Instruction{InstructionType::RES, ArithmeticTarget8Bit::E, ArithmeticTarget16Bit::BC, 0};
        case 0x84: return Instruction{InstructionType::RES, ArithmeticTarget8Bit::H, ArithmeticTarget16Bit::BC, 0};
        case 0x85: return Instruction{InstructionType::RES, ArithmeticTarget8Bit::L, ArithmeticTarget16Bit::BC, 0};
        case 0x87: return Instruction{InstructionType::RES, ArithmeticTarget8Bit::A, ArithmeticTarget16Bit::BC, 0};

        // RES 1
        case 0x88: return Instruction{InstructionType::RES, ArithmeticTarget8Bit::B, ArithmeticTarget16Bit::BC, 1};
        case 0x89: return Instruction{InstructionType::RES, ArithmeticTarget8Bit::C, ArithmeticTarget16Bit::BC, 1};
        case 0x8A: return Instruction{InstructionType::RES, ArithmeticTarget8Bit::D, ArithmeticTarget16Bit::BC, 1};
        case 0x8B: return Instruction{InstructionType::RES, ArithmeticTarget8Bit::E, ArithmeticTarget16Bit::BC, 1};
        case 0x8C: return Instruction{InstructionType::RES, ArithmeticTarget8Bit::H, ArithmeticTarget16Bit::BC, 1};
        case 0x8D: return Instruction{InstructionType::RES, ArithmeticTarget8Bit::L, ArithmeticTarget16Bit::BC, 1};
        case 0x8F: return Instruction{InstructionType::RES, ArithmeticTarget8Bit::A, ArithmeticTarget16Bit::BC, 1};

        // RES 2
        case 0x90: return Instruction{InstructionType::RES, ArithmeticTarget8Bit::B, ArithmeticTarget16Bit::BC, 2};
        case 0x91: return Instruction{InstructionType::RES, ArithmeticTarget8Bit::C, ArithmeticTarget16Bit::BC, 2};
        case 0x92: return Instruction{InstructionType::RES, ArithmeticTarget8Bit::D, ArithmeticTarget16Bit::BC, 2};
        case 0x93: return Instruction{InstructionType::RES, ArithmeticTarget8Bit::E, ArithmeticTarget16Bit::BC, 2};
        case 0x94: return Instruction{InstructionType::RES, ArithmeticTarget8Bit::H, ArithmeticTarget16Bit::BC, 2};
        case 0x95: return Instruction{InstructionType::RES, ArithmeticTarget8Bit::L, ArithmeticTarget16Bit::BC, 2};
        case 0x97: return Instruction{InstructionType::RES, ArithmeticTarget8Bit::A, ArithmeticTarget16Bit::BC, 2};

        // RES 3
        case 0x98: return Instruction{InstructionType::RES, ArithmeticTarget8Bit::B, ArithmeticTarget16Bit::BC, 3};
        case 0x99: return Instruction{InstructionType::RES, ArithmeticTarget8Bit::C, ArithmeticTarget16Bit::BC, 3};
        case 0x9A: return Instruction{InstructionType::RES, ArithmeticTarget8Bit::D, ArithmeticTarget16Bit::BC, 3};
        case 0x9B: return Instruction{InstructionType::RES, ArithmeticTarget8Bit::E, ArithmeticTarget16Bit::BC, 3};
        case 0x9C: return Instruction{InstructionType::RES, ArithmeticTarget8Bit::H, ArithmeticTarget16Bit::BC, 3};
        case 0x9D: return Instruction{InstructionType::RES, ArithmeticTarget8Bit::L, ArithmeticTarget16Bit::BC, 3};
        case 0x9F: return Instruction{InstructionType::RES, ArithmeticTarget8Bit::A, ArithmeticTarget16Bit::BC, 3};

        // RES 4
        case 0xA0: return Instruction{InstructionType::RES, ArithmeticTarget8Bit::B, ArithmeticTarget16Bit::BC, 4};
        case 0xA1: return Instruction{InstructionType::RES, ArithmeticTarget8Bit::C, ArithmeticTarget16Bit::BC, 4};
        case 0xA2: return Instruction{InstructionType::RES, ArithmeticTarget8Bit::D, ArithmeticTarget16Bit::BC, 4};
        case 0xA3: return Instruction{InstructionType::RES, ArithmeticTarget8Bit::E, ArithmeticTarget16Bit::BC, 4};
        case 0xA4: return Instruction{InstructionType::RES, ArithmeticTarget8Bit::H, ArithmeticTarget16Bit::BC, 4};
        case 0xA5: return Instruction{InstructionType::RES, ArithmeticTarget8Bit::L, ArithmeticTarget16Bit::BC, 4};
        case 0xA7: return Instruction{InstructionType::RES, ArithmeticTarget8Bit::A, ArithmeticTarget16Bit::BC, 4};

        // RES 5
        case 0xA8: return Instruction{InstructionType::RES, ArithmeticTarget8Bit::B, ArithmeticTarget16Bit::BC, 5};
        case 0xA9: return Instruction{InstructionType::RES, ArithmeticTarget8Bit::C, ArithmeticTarget16Bit::BC, 5};
        case 0xAA: return Instruction{InstructionType::RES, ArithmeticTarget8Bit::D, ArithmeticTarget16Bit::BC, 5};
        case 0xAB: return Instruction{InstructionType::RES, ArithmeticTarget8Bit::E, ArithmeticTarget16Bit::BC, 5};
        case 0xAC: return Instruction{InstructionType::RES, ArithmeticTarget8Bit::H, ArithmeticTarget16Bit::BC, 5};
        case 0xAD: return Instruction{InstructionType::RES, ArithmeticTarget8Bit::L, ArithmeticTarget16Bit::BC, 5};
        case 0xAF: return Instruction{InstructionType::RES, ArithmeticTarget8Bit::A, ArithmeticTarget16Bit::BC, 5};

        // RES 6
        case 0xB0: return Instruction{InstructionType::RES, ArithmeticTarget8Bit::B, ArithmeticTarget16Bit::BC, 6};
        case 0xB1: return Instruction{InstructionType::RES, ArithmeticTarget8Bit::C, ArithmeticTarget16Bit::BC, 6};
        case 0xB2: return Instruction{InstructionType::RES, ArithmeticTarget8Bit::D, ArithmeticTarget16Bit::BC, 6};
        case 0xB3: return Instruction{InstructionType::RES, ArithmeticTarget8Bit::E, ArithmeticTarget16Bit::BC, 6};
        case 0xB4: return Instruction{InstructionType::RES, ArithmeticTarget8Bit::H, ArithmeticTarget16Bit::BC, 6};
        case 0xB5: return Instruction{InstructionType::RES, ArithmeticTarget8Bit::L, ArithmeticTarget16Bit::BC, 6};
        case 0xB7: return Instruction{InstructionType::RES, ArithmeticTarget8Bit::A, ArithmeticTarget16Bit::BC, 6};

        // RES 7
        case 0xB8: return Instruction{InstructionType::RES, ArithmeticTarget8Bit::B, ArithmeticTarget16Bit::BC, 7};
        case 0xB9: return Instruction{InstructionType::RES, ArithmeticTarget8Bit::C, ArithmeticTarget16Bit::BC, 7};
        case 0xBA: return Instruction{InstructionType::RES, ArithmeticTarget8Bit::D, ArithmeticTarget16Bit::BC, 7};
        case 0xBB: return Instruction{InstructionType::RES, ArithmeticTarget8Bit::E, ArithmeticTarget16Bit::BC, 7};
        case 0xBC: return Instruction{InstructionType::RES, ArithmeticTarget8Bit::H, ArithmeticTarget16Bit::BC, 7};
        case 0xBD: return Instruction{InstructionType::RES, ArithmeticTarget8Bit::L, ArithmeticTarget16Bit::BC, 7};
        case 0xBF: return Instruction{InstructionType::RES, ArithmeticTarget8Bit::A, ArithmeticTarget16Bit::BC, 7};

        // --------------------
        // RLC r
        // --------------------
        case 0x00: return Instruction{InstructionType::RLC, ArithmeticTarget8Bit::B};
        case 0x01: return Instruction{InstructionType::RLC, ArithmeticTarget8Bit::C};
        case 0x02: return Instruction{InstructionType::RLC, ArithmeticTarget8Bit::D};
        case 0x03: return Instruction{InstructionType::RLC, ArithmeticTarget8Bit::E};
        case 0x04: return Instruction{InstructionType::RLC, ArithmeticTarget8Bit::H};
        case 0x05: return Instruction{InstructionType::RLC, ArithmeticTarget8Bit::L};
        case 0x07: return Instruction{InstructionType::RLC, ArithmeticTarget8Bit::A};

        // --------------------
        // RL r
        // --------------------
        case 0x10: return Instruction{InstructionType::RL, ArithmeticTarget8Bit::B};
        case 0x11: return Instruction{InstructionType::RL, ArithmeticTarget8Bit::C};
        case 0x12: return Instruction{InstructionType::RL, ArithmeticTarget8Bit::D};
        case 0x13: return Instruction{InstructionType::RL, ArithmeticTarget8Bit::E};
        case 0x14: return Instruction{InstructionType::RL, ArithmeticTarget8Bit::H};
        case 0x15: return Instruction{InstructionType::RL, ArithmeticTarget8Bit::L};
        case 0x17: return Instruction{InstructionType::RL, ArithmeticTarget8Bit::A};

        // --------------------
        // RRC r
        // --------------------
        case 0x08: return Instruction{InstructionType::RRC, ArithmeticTarget8Bit::B};
        case 0x09: return Instruction{InstructionType::RRC, ArithmeticTarget8Bit::C};
        case 0x0A: return Instruction{InstructionType::RRC, ArithmeticTarget8Bit::D};
        case 0x0B: return Instruction{InstructionType::RRC, ArithmeticTarget8Bit::E};
        case 0x0C: return Instruction{InstructionType::RRC, ArithmeticTarget8Bit::H};
        case 0x0D: return Instruction{InstructionType::RRC, ArithmeticTarget8Bit::L};
        case 0x0F: return Instruction{InstructionType::RRC, ArithmeticTarget8Bit::A};

        // --------------------
        // RR r
        // --------------------
        case 0x18: return Instruction{InstructionType::RR, ArithmeticTarget8Bit::B};
        case 0x19: return Instruction{InstructionType::RR, ArithmeticTarget8Bit::C};
        case 0x1A: return Instruction{InstructionType::RR, ArithmeticTarget8Bit::D};
        case 0x1B: return Instruction{InstructionType::RR, ArithmeticTarget8Bit::E};
        case 0x1C: return Instruction{InstructionType::RR, ArithmeticTarget8Bit::H};
        case 0x1D: return Instruction{InstructionType::RR, ArithmeticTarget8Bit::L};
        case 0x1F: return Instruction{InstructionType::RR, ArithmeticTarget8Bit::A};

        // --------------------
        // SLA r
        // --------------------
        case 0x20: return Instruction{InstructionType::SLA, ArithmeticTarget8Bit::B};
        case 0x21: return Instruction{InstructionType::SLA, ArithmeticTarget8Bit::C};
        case 0x22: return Instruction{InstructionType::SLA, ArithmeticTarget8Bit::D};
        case 0x23: return Instruction{InstructionType::SLA, ArithmeticTarget8Bit::E};
        case 0x24: return Instruction{InstructionType::SLA, ArithmeticTarget8Bit::H};
        case 0x25: return Instruction{InstructionType::SLA, ArithmeticTarget8Bit::L};
        case 0x27: return Instruction{InstructionType::SLA, ArithmeticTarget8Bit::A};

        // --------------------
        // SRA r
        // --------------------
        case 0x28: return Instruction{InstructionType::SRA, ArithmeticTarget8Bit::B};
        case 0x29: return Instruction{InstructionType::SRA, ArithmeticTarget8Bit::C};
        case 0x2A: return Instruction{InstructionType::SRA, ArithmeticTarget8Bit::D};
        case 0x2B: return Instruction{InstructionType::SRA, ArithmeticTarget8Bit::E};
        case 0x2C: return Instruction{InstructionType::SRA, ArithmeticTarget8Bit::H};
        case 0x2D: return Instruction{InstructionType::SRA, ArithmeticTarget8Bit::L};
        case 0x2F: return Instruction{InstructionType::SRA, ArithmeticTarget8Bit::A};

        // --------------------
        // SRL r
        // --------------------
        case 0x38: return Instruction{InstructionType::SRL, ArithmeticTarget8Bit::B};
        case 0x39: return Instruction{InstructionType::SRL, ArithmeticTarget8Bit::C};
        case 0x3A: return Instruction{InstructionType::SRL, ArithmeticTarget8Bit::D};
        case 0x3B: return Instruction{InstructionType::SRL, ArithmeticTarget8Bit::E};
        case 0x3C: return Instruction{InstructionType::SRL, ArithmeticTarget8Bit::H};
        case 0x3D: return Instruction{InstructionType::SRL, ArithmeticTarget8Bit::L};
        case 0x3F: return Instruction{InstructionType::SRL, ArithmeticTarget8Bit::A};

        default: return std::nullopt;
    }
}

static std::optional<Instruction> from_byte_not_prefixed(uint8_t byte) {
    switch (byte) {
        // --------------------
        // ADD A, r
        // --------------------
        case 0x87: return Instruction{InstructionType::ADD, ArithmeticTarget8Bit::A};
        case 0x80: return Instruction{InstructionType::ADD, ArithmeticTarget8Bit::B};
        case 0x81: return Instruction{InstructionType::ADD, ArithmeticTarget8Bit::C};
        case 0x82: return Instruction{InstructionType::ADD, ArithmeticTarget8Bit::D};
        case 0x83: return Instruction{InstructionType::ADD, ArithmeticTarget8Bit::E};
        case 0x84: return Instruction{InstructionType::ADD, ArithmeticTarget8Bit::H};
        case 0x85: return Instruction{InstructionType::ADD, ArithmeticTarget8Bit::L};

        // --------------------
        // ADC A, r
        // --------------------
        case 0x8F: return Instruction{InstructionType::ADC, ArithmeticTarget8Bit::A};
        case 0x88: return Instruction{InstructionType::ADC, ArithmeticTarget8Bit::B};
        case 0x89: return Instruction{InstructionType::ADC, ArithmeticTarget8Bit::C};
        case 0x8A: return Instruction{InstructionType::ADC, ArithmeticTarget8Bit::D};
        case 0x8B: return Instruction{InstructionType::ADC, ArithmeticTarget8Bit::E};
        case 0x8C: return Instruction{InstructionType::ADC, ArithmeticTarget8Bit::H};
        case 0x8D: return Instruction{InstructionType::ADC, ArithmeticTarget8Bit::L};

        // --------------------
        // SUB r
        // --------------------
        case 0x97: return Instruction{InstructionType::SUB, ArithmeticTarget8Bit::A};
        case 0x90: return Instruction{InstructionType::SUB, ArithmeticTarget8Bit::B};
        case 0x91: return Instruction{InstructionType::SUB, ArithmeticTarget8Bit::C};
        case 0x92: return Instruction{InstructionType::SUB, ArithmeticTarget8Bit::D};
        case 0x93: return Instruction{InstructionType::SUB, ArithmeticTarget8Bit::E};
        case 0x94: return Instruction{InstructionType::SUB, ArithmeticTarget8Bit::H};
        case 0x95: return Instruction{InstructionType::SUB, ArithmeticTarget8Bit::L};

        // --------------------
        // SBC r
        // --------------------
        case 0x9F: return Instruction{InstructionType::SBC, ArithmeticTarget8Bit::A};
        case 0x98: return Instruction{InstructionType::SBC, ArithmeticTarget8Bit::B};
        case 0x99: return Instruction{InstructionType::SBC, ArithmeticTarget8Bit::C};
        case 0x9A: return Instruction{InstructionType::SBC, ArithmeticTarget8Bit::D};
        case 0x9B: return Instruction{InstructionType::SBC, ArithmeticTarget8Bit::E};
        case 0x9C: return Instruction{InstructionType::SBC, ArithmeticTarget8Bit::H};
        case 0x9D: return Instruction{InstructionType::SBC, ArithmeticTarget8Bit::L};

        // --------------------
        // AND r
        // --------------------
        case 0xA7: return Instruction{InstructionType::AND, ArithmeticTarget8Bit::A};
        case 0xA0: return Instruction{InstructionType::AND, ArithmeticTarget8Bit::B};
        case 0xA1: return Instruction{InstructionType::AND, ArithmeticTarget8Bit::C};
        case 0xA2: return Instruction{InstructionType::AND, ArithmeticTarget8Bit::D};
        case 0xA3: return Instruction{InstructionType::AND, ArithmeticTarget8Bit::E};
        case 0xA4: return Instruction{InstructionType::AND, ArithmeticTarget8Bit::H};
        case 0xA5: return Instruction{InstructionType::AND, ArithmeticTarget8Bit::L};

        // --------------------
        // OR r
        // --------------------
        case 0xB7: return Instruction{InstructionType::OR, ArithmeticTarget8Bit::A};
        case 0xB0: return Instruction{InstructionType::OR, ArithmeticTarget8Bit::B};
        case 0xB1: return Instruction{InstructionType::OR, ArithmeticTarget8Bit::C};
        case 0xB2: return Instruction{InstructionType::OR, ArithmeticTarget8Bit::D};
        case 0xB3: return Instruction{InstructionType::OR, ArithmeticTarget8Bit::E};
        case 0xB4: return Instruction{InstructionType::OR, ArithmeticTarget8Bit::H};
        case 0xB5: return Instruction{InstructionType::OR, ArithmeticTarget8Bit::L};

        // --------------------
        // XOR r
        // --------------------
        case 0xAF: return Instruction{InstructionType::XOR, ArithmeticTarget8Bit::A};
        case 0xA8: return Instruction{InstructionType::XOR, ArithmeticTarget8Bit::B};
        case 0xA9: return Instruction{InstructionType::XOR, ArithmeticTarget8Bit::C};
        case 0xAA: return Instruction{InstructionType::XOR, ArithmeticTarget8Bit::D};
        case 0xAB: return Instruction{InstructionType::XOR, ArithmeticTarget8Bit::E};
        case 0xAC: return Instruction{InstructionType::XOR, ArithmeticTarget8Bit::H};
        case 0xAD: return Instruction{InstructionType::XOR, ArithmeticTarget8Bit::L};

        // --------------------
        // CP r (compare)
        // --------------------
        case 0xBF: return Instruction{InstructionType::CP, ArithmeticTarget8Bit::A};
        case 0xB8: return Instruction{InstructionType::CP, ArithmeticTarget8Bit::B};
        case 0xB9: return Instruction{InstructionType::CP, ArithmeticTarget8Bit::C};
        case 0xBA: return Instruction{InstructionType::CP, ArithmeticTarget8Bit::D};
        case 0xBB: return Instruction{InstructionType::CP, ArithmeticTarget8Bit::E};
        case 0xBC: return Instruction{InstructionType::CP, ArithmeticTarget8Bit::H};
        case 0xBD: return Instruction{InstructionType::CP, ArithmeticTarget8Bit::L};


        // --------------------
        // INC r
        // --------------------
        case 0x3C: return Instruction{InstructionType::INC, ArithmeticTarget8Bit::A};
        case 0x04: return Instruction{InstructionType::INC, ArithmeticTarget8Bit::B};
        case 0x0C: return Instruction{InstructionType::INC, ArithmeticTarget8Bit::C};
        case 0x14: return Instruction{InstructionType::INC, ArithmeticTarget8Bit::D};
        case 0x1C: return Instruction{InstructionType::INC, ArithmeticTarget8Bit::E};
        case 0x24: return Instruction{InstructionType::INC, ArithmeticTarget8Bit::H};
        case 0x2C: return Instruction{InstructionType::INC, ArithmeticTarget8Bit::L};

        // --------------------
        // DEC r
        // --------------------
        case 0x3D: return Instruction{InstructionType::DEC, ArithmeticTarget8Bit::A};
        case 0x05: return Instruction{InstructionType::DEC, ArithmeticTarget8Bit::B};
        case 0x0D: return Instruction{InstructionType::DEC, ArithmeticTarget8Bit::C};
        case 0x15: return Instruction{InstructionType::DEC, ArithmeticTarget8Bit::D};
        case 0x1D: return Instruction{InstructionType::DEC, ArithmeticTarget8Bit::E};
        case 0x25: return Instruction{InstructionType::DEC, ArithmeticTarget8Bit::H};
        case 0x2D: return Instruction{InstructionType::DEC, ArithmeticTarget8Bit::L};

        // --------------------
        // CPL
        // --------------------
        case 0x2F: return Instruction{InstructionType::CPL};

        // --------------------
        // CCF
        // --------------------
        case 0x3F: return Instruction{InstructionType::CCF};

        // --------------------
        // SCF
        // --------------------
        case 0x37: return Instruction{InstructionType::SCF};

        // --------------------
        // ADDHL HL, r
        // --------------------
        case 0x09: return Instruction{InstructionType::ADDHL, ArithmeticTarget8Bit::A, ArithmeticTarget16Bit::BC};
        case 0x19: return Instruction{InstructionType::ADDHL, ArithmeticTarget8Bit::A, ArithmeticTarget16Bit::DE};
        case 0x29: return Instruction{InstructionType::ADDHL, ArithmeticTarget8Bit::A, ArithmeticTarget16Bit::HL};
        case 0x39: return Instruction{InstructionType::ADDHL, ArithmeticTarget8Bit::A, ArithmeticTarget16Bit::SP};

        // --------------------
        // RLCA
        // --------------------
        case 0x07: return Instruction{InstructionType::RLCA};

        // --------------------
        // RLA
        // --------------------
        case 0x17: return Instruction{InstructionType::RLA};

        // --------------------
        // RRCA
        // --------------------
        case 0x0F: return Instruction{InstructionType::RRCA};

        // --------------------
        // RRA
        // --------------------
        case 0x1F: return Instruction{InstructionType::RRA};

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

                    pc = pc + 1;

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

                    pc = pc + 1;

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

                    pc = pc + 1;

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

                    pc = pc + 1;

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

                    pc = pc + 1;

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

                    pc = pc + 1;

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

                    pc = pc + 1;

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

                    pc = pc + 1;

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

                    pc = pc + 1;

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

                    pc = pc + 1;

                    break;
                }

                /* Misc CPU Instructions */

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

                    pc = pc + 2;

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

                    pc = pc + 2;

                    break;
                }

                // RES Instruction sets the bit value for the given register at the given bit index to 0
                case InstructionType::RES: {
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

                    pc = pc + 2;

                    break;
                }

                /* 16-bit Arithmetic Operations */

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

                    pc = pc + 2;

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

                    pc = pc + 2;

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

                    pc = pc + 2;

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

                    pc = pc + 2;

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

                    pc = pc + 2;

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

                    pc = pc + 2;

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

                    pc = pc + 2;

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