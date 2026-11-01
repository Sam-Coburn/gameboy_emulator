#include <catch2/catch_test_macros.hpp>
#include <instruction.hpp>

TEST_CASE("Instruction from_byte() Unit Tests", "[instruction][from_byte]") {
    SECTION ("ADD A") {
        std::optional<Instruction> opt_instruct = from_byte(0x87, false);
        Instruction instruct = opt_instruct.value(); 

        REQUIRE(instruct.type == InstructionType::ADD);
        REQUIRE(instruct.alu8.target == Operand8::A);
    }

    SECTION ("SWAP A") {
        std::optional<Instruction> opt_instruct = from_byte(0x37, true);
        Instruction instruct = opt_instruct.value(); 

        REQUIRE(instruct.type == InstructionType::SWAP);
        REQUIRE(instruct.alu8.target == Operand8::A);
    }

    SECTION ("from_byte_prefixed SWAP HL_PTR") {
        std::optional<Instruction> opt_instruct = from_byte_prefixed(0x36);
        Instruction instruct = opt_instruct.value();

        REQUIRE(instruct.type == InstructionType::SWAP);
        REQUIRE(instruct.alu8.target == Operand8::HL_PTR);
    }

    SECTION ("from_byte_not_prefixed JP Always") {
        std::optional<Instruction> opt_instruct = from_byte_not_prefixed(0xC3);
        Instruction instruct = opt_instruct.value();

        REQUIRE(instruct.type == InstructionType::JP);
        REQUIRE(instruct.jump.condition == JumpTest::Always);
    }

    SECTION ("from_byte_not_prefixed LD_N A,n") {
        Instruction instruct = from_byte_not_prefixed(0x3E).value();

        REQUIRE(instruct.type == InstructionType::LD_N);
        REQUIRE(instruct.load8.source == LoadSource::NUM);
        REQUIRE(instruct.load8.target == LoadTarget::A);
    }

    SECTION ("from_byte_not_prefixed LD_N (HL),n") {
        Instruction instruct = from_byte_not_prefixed(0x36).value();

        REQUIRE(instruct.type == InstructionType::LD_N);
        REQUIRE(instruct.load8.source == LoadSource::NUM);
        REQUIRE(instruct.load8.target == LoadTarget::HL_PTR);
    }

    SECTION ("from_byte_not_prefixed LD_16 SP,nn") {
        Instruction instruct = from_byte_not_prefixed(0x31).value();

        REQUIRE(instruct.type == InstructionType::LD_16);
        REQUIRE(instruct.load16.target == Operand16::SP);
    }

    SECTION ("from_byte_not_prefixed LD_8 (HL),A") {
        Instruction instruct = from_byte_not_prefixed(0x77).value();

        REQUIRE(instruct.type == InstructionType::LD_8);
        REQUIRE(instruct.load8.source == LoadSource::A);
        REQUIRE(instruct.load8.target == LoadTarget::HL_PTR);
    }

    SECTION ("from_byte_not_prefixed LD_A_IND A,(HL+)") {
        Instruction instruct = from_byte_not_prefixed(0x2A).value();

        REQUIRE(instruct.type == InstructionType::LD_A_IND);
        REQUIRE(instruct.load_ind.address == IndirectAddress::HL_INC);
    }

    SECTION ("from_byte_not_prefixed LD_IND_A (FF00+n),A") {
        Instruction instruct = from_byte_not_prefixed(0xE0).value();

        REQUIRE(instruct.type == InstructionType::LD_IND_A);
        REQUIRE(instruct.load_ind.address == IndirectAddress::FF00_N);
    }

    SECTION ("from_byte_not_prefixed LD_HL_SP") {
        Instruction instruct = from_byte_not_prefixed(0xF8).value();

        REQUIRE(instruct.type == InstructionType::LD_HL_SP);
    }

    SECTION ("from_byte_not_prefixed PUSH AF") {
        Instruction instruct = from_byte_not_prefixed(0xF5).value();

        REQUIRE(instruct.type == InstructionType::PUSH);
        REQUIRE(instruct.stack.target == StackTarget::AF);
    }

    SECTION ("from_byte_not_prefixed POP HL") {
        Instruction instruct = from_byte_not_prefixed(0xE1).value();

        REQUIRE(instruct.type == InstructionType::POP);
        REQUIRE(instruct.stack.target == StackTarget::HL);
    }

    SECTION ("from_byte_not_prefixed CALL NotCarry") {
        Instruction instruct = from_byte_not_prefixed(0xD4).value();

        REQUIRE(instruct.type == InstructionType::CALL);
        REQUIRE(instruct.jump.condition == JumpTest::NotCarry);
    }

    SECTION ("from_byte_not_prefixed RET Always") {
        Instruction instruct = from_byte_not_prefixed(0xC9).value();

        REQUIRE(instruct.type == InstructionType::RET);
        REQUIRE(instruct.jump.condition == JumpTest::Always);
    }

    SECTION ("from_byte_not_prefixed RST 0x28") {
        Instruction instruct = from_byte_not_prefixed(0xEF).value();

        REQUIRE(instruct.type == InstructionType::RST);
        REQUIRE(instruct.rst.address == 0x28);
    }

    SECTION ("from_byte_not_prefixed DEC16 SP") {
        Instruction instruct = from_byte_not_prefixed(0x3B).value();

        REQUIRE(instruct.type == InstructionType::DEC16);
        REQUIRE(instruct.alu16.target == Operand16::SP);
    }

    SECTION ("from_byte_not_prefixed HALT") {
        Instruction instruct = from_byte_not_prefixed(0x76).value();

        REQUIRE(instruct.type == InstructionType::HALT);
    }
}
