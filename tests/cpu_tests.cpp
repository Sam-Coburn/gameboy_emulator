#include <catch2/catch_test_macros.hpp>
#include <cpu.hpp>

TEST_CASE("Default constructor initializes CPU successfully", "[cpu]") {
    CPU c;

    REQUIRE(c.registers.a == 0);
    REQUIRE(c.registers.b == 0);
    REQUIRE(c.registers.c == 0);
    REQUIRE(c.registers.d == 0);
    REQUIRE(c.registers.e == 0);
    REQUIRE(c.registers.h == 0);
    REQUIRE(c.registers.l == 0);

    REQUIRE(c.registers.f.zero == false);
    REQUIRE(c.registers.f.subtract == false);
    REQUIRE(c.registers.f.half_carry == false);
    REQUIRE(c.registers.f.carry == false);
}

TEST_CASE("CPU 8-bit Arithmetic Unit Tests - ADD", "[cpu][8-bit][add]") {
    Instruction instruct;
    instruct.type = InstructionType::ADD;
    instruct.target = ArithmeticTarget::B;

    SECTION ("ADD w/o Overflow") {
        CPU c1;

        c1.registers.a = 1;
        c1.registers.b = 1;

        c1.execute(instruct);

        REQUIRE(c1.registers.a == 2);
        REQUIRE(c1.registers.f.zero == false);
        REQUIRE(c1.registers.f.subtract == false);
        REQUIRE(c1.registers.f.carry == false);
        REQUIRE(c1.registers.f.half_carry == false);
    }

    // SECTION ("ADD w/ Overflow") {
    //     CPU c2;


    // }

    // SECTION ("ADD w/ Zeroes") {
    //     CPU c3;


    // }

    // SECTION ("ADD w/ Half Carry") {
    //     CPU c4;
    // }
}
