#include <optional>
#include <stdint.h>

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

// enum class LoadSource {
//     A, B, C, D, E, H, L, HL_PTR
// };

enum class LoadTarget {
    A, B, C, D, E, H, L, BC, DE, HL, SP
};

struct Load8Data {
    // LoadSource source;
    LoadTarget target;
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
    JP, JR, JP_HL,

    // Load instructions
    LD_N
};

struct Instruction {
    InstructionType type;

    union {
        ALU8Data alu8;
        ALU16Data alu16;
        BitOpData bitop;
        JumpData jump;
        Load8Data load8;
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

    auto make_load8 = [](InstructionType t, LoadTarget target) -> std::optional<Instruction> {
        Instruction inst{};
        inst.type = t;
        inst.load8.target = target;
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

        // --------------------
        // LD_N nn,n
        // --------------------
        case 0x06: return make_load8(InstructionType::LD_N, LoadTarget::B);
        case 0x0E: return make_load8(InstructionType::LD_N, LoadTarget::C);
        case 0x16: return make_load8(InstructionType::LD_N, LoadTarget::D);
        case 0x1E: return make_load8(InstructionType::LD_N, LoadTarget::E);
        case 0x26: return make_load8(InstructionType::LD_N, LoadTarget::H);
        case 0x2E: return make_load8(InstructionType::LD_N, LoadTarget::L);
        case 0x01: return make_load8(InstructionType::LD_N, LoadTarget::BC);
        case 0x11: return make_load8(InstructionType::LD_N, LoadTarget::DE);
        case 0x21: return make_load8(InstructionType::LD_N, LoadTarget::HL);
        case 0x31: return make_load8(InstructionType::LD_N, LoadTarget::SP);

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