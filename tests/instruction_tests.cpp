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
}