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

TEST_CASE("ADD Unit Tests", "[cpu][8-bit][add]") {
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

    SECTION ("ADD w/ Overflow") {
        CPU c2;

        c2.registers.a = 200;
        c2.registers.b = 100;

        c2.execute(instruct);

        REQUIRE(c2.registers.a == 44);
        REQUIRE(c2.registers.f.zero == false);
        REQUIRE(c2.registers.f.subtract == false);
        REQUIRE(c2.registers.f.carry == true);
        REQUIRE(c2.registers.f.half_carry == false);
    }

    SECTION ("ADD w/ Zeroes") {
        CPU c3;

        c3.registers.a = 0;
        c3.registers.b = 0;

        c3.execute(instruct);

        REQUIRE(c3.registers.a == 0);
        REQUIRE(c3.registers.f.zero == true);
        REQUIRE(c3.registers.f.subtract == false);
        REQUIRE(c3.registers.f.carry == false);
        REQUIRE(c3.registers.f.half_carry == false);
    }

    SECTION ("ADD w/ Half Carry") {
        CPU c4;

        c4.registers.a = 15;
        c4.registers.b = 1;

        c4.execute(instruct);

        REQUIRE(c4.registers.a == 16);
        REQUIRE(c4.registers.f.zero == false);
        REQUIRE(c4.registers.f.subtract == false);
        REQUIRE(c4.registers.f.carry == false);
        REQUIRE(c4.registers.f.half_carry == true);
    }
}

TEST_CASE("ADC Unit Tests", "[cpu][8-bit][adc]") {
    Instruction instruct;
    instruct.type = InstructionType::ADC;
    instruct.target = ArithmeticTarget::B;

    SECTION ("ADC w/o Overflow - No Carry") {
        CPU c1;

        c1.registers.a = 1;
        c1.registers.b = 1;
        c1.registers.f.carry = 0;

        c1.execute(instruct);

        REQUIRE(c1.registers.a == 2);
        REQUIRE(c1.registers.f.zero == false);
        REQUIRE(c1.registers.f.subtract == false);
        REQUIRE(c1.registers.f.carry == false);
        REQUIRE(c1.registers.f.half_carry == false);
    }

    SECTION ("ADC w/o Overflow - Carry") {
        CPU c2;

        c2.registers.a = 1;
        c2.registers.b = 1;
        c2.registers.f.carry = 1;

        c2.execute(instruct);

        REQUIRE(c2.registers.a == 3);
        REQUIRE(c2.registers.f.zero == false);
        REQUIRE(c2.registers.f.subtract == false);
        REQUIRE(c2.registers.f.carry == false);
        REQUIRE(c2.registers.f.half_carry == false);
    }

    SECTION ("ADC Near Overflow - No Carry") {
        CPU c3;

        c3.registers.a = 254;
        c3.registers.b = 1;
        c3.registers.f.carry = 0;

        c3.execute(instruct);

        REQUIRE(c3.registers.a == 255);
        REQUIRE(c3.registers.f.zero == false);
        REQUIRE(c3.registers.f.subtract == false);
        REQUIRE(c3.registers.f.carry == false);
        REQUIRE(c3.registers.f.half_carry == false);
    }

    SECTION ("ADC Near Overflow - Carry") {
        CPU c4;

        c4.registers.a = 254;
        c4.registers.b = 1;
        c4.registers.f.carry = 1;

        c4.execute(instruct);

        REQUIRE(c4.registers.a == 0);
        REQUIRE(c4.registers.f.zero == true);
        REQUIRE(c4.registers.f.subtract == false);
        REQUIRE(c4.registers.f.carry == true);
        REQUIRE(c4.registers.f.half_carry == false);
    }
}

TEST_CASE("SUB Unit Tests", "[cpu][8-bit][sub]") {
    Instruction instruct;
    instruct.type = InstructionType::SUB;
    instruct.target = ArithmeticTarget::B;

    SECTION ("SUB w/o Overflow") {
        CPU c1;

        c1.registers.a = 2;
        c1.registers.b = 1;

        c1.execute(instruct);

        REQUIRE(c1.registers.a == 1);
        REQUIRE(c1.registers.f.zero == false);
        REQUIRE(c1.registers.f.subtract == true);
        REQUIRE(c1.registers.f.carry == false);
        REQUIRE(c1.registers.f.half_carry == false);
    }

    SECTION ("SUB w Underflow") {
        CPU c2;

        c2.registers.a = 1;
        c2.registers.b = 2;

        c2.execute(instruct);

        REQUIRE(c2.registers.a == 255);
        REQUIRE(c2.registers.f.zero == false);
        REQUIRE(c2.registers.f.subtract == true);
        REQUIRE(c2.registers.f.carry == true);
        REQUIRE(c2.registers.f.half_carry == true);
    }

    SECTION ("SUB to 0") {
        CPU c3;
        c3.registers.a = 1;
        c3.registers.b = 1;

        c3.execute(instruct);

        REQUIRE(c3.registers.a == 0);
        REQUIRE(c3.registers.f.zero == true);
        REQUIRE(c3.registers.f.subtract == true);
        REQUIRE(c3.registers.f.carry == false);
        REQUIRE(c3.registers.f.half_carry == false);
    }
}

TEST_CASE("SBC Unit Tests", "[cpu][8-bit][sbc]") {
    Instruction instruct;
    instruct.type = InstructionType::SBC;
    instruct.target = ArithmeticTarget::B;

    SECTION ("SBC w/o Overflow - No Carry") {
        CPU c1;

        c1.registers.a = 2;
        c1.registers.b = 1;
        c1.registers.f.carry = 0;

        c1.execute(instruct);

        REQUIRE(c1.registers.a == 1);
        REQUIRE(c1.registers.f.zero == false);
        REQUIRE(c1.registers.f.subtract == true);
        REQUIRE(c1.registers.f.carry == false);
        REQUIRE(c1.registers.f.half_carry == false);
    }

    SECTION ("SBC w/o Overflow - w/ Carry") {
        CPU c2;

        c2.registers.a = 2;
        c2.registers.b = 1;
        c2.registers.f.carry = 1;

        c2.execute(instruct);

        REQUIRE(c2.registers.a == 0);
        REQUIRE(c2.registers.f.zero == true);
        REQUIRE(c2.registers.f.subtract == true);
        REQUIRE(c2.registers.f.carry == false);
        REQUIRE(c2.registers.f.half_carry == false);
    }
}

TEST_CASE("AND Unit Tests", "[cpu][8-bit][and]") {
    Instruction instruct;
    instruct.type = InstructionType::AND;
    instruct.target = ArithmeticTarget::B;

    SECTION ("AND - No Zero") {
        CPU c1;

        c1.registers.a = 1;
        c1.registers.b = 1;

        c1.execute(instruct);

        REQUIRE(c1.registers.a == 1);
        REQUIRE(c1.registers.f.zero == false);
        REQUIRE(c1.registers.f.subtract == false);
        REQUIRE(c1.registers.f.carry == false);
        REQUIRE(c1.registers.f.half_carry == true);
    }

    SECTION ("AND - w/ Zero") {
        CPU c2;

        c2.registers.a = 100;
        c2.registers.b = 0;

        c2.execute(instruct);

        REQUIRE(c2.registers.a == 0);
        REQUIRE(c2.registers.f.zero == true);
        REQUIRE(c2.registers.f.subtract == false);
        REQUIRE(c2.registers.f.carry == false);
        REQUIRE(c2.registers.f.half_carry == true);
    }
}

TEST_CASE("OR Unit Tests", "[cpu][8-bit][or]") {
    Instruction instruct;
    instruct.type = InstructionType::OR;
    instruct.target = ArithmeticTarget::B;

    SECTION ("OR - No Zero") {
        CPU c1;

        c1.registers.a = 1;
        c1.registers.b = 1;

        c1.execute(instruct);

        REQUIRE(c1.registers.a == 1);
        REQUIRE(c1.registers.f.zero == false);
        REQUIRE(c1.registers.f.subtract == false);
        REQUIRE(c1.registers.f.carry == false);
        REQUIRE(c1.registers.f.half_carry == false);
    }

    SECTION ("OR - Zero") {
        CPU c2;

        c2.registers.a = 0;
        c2.registers.b = 0;

        c2.execute(instruct);

        REQUIRE(c2.registers.a == 0);
        REQUIRE(c2.registers.f.zero == true);
        REQUIRE(c2.registers.f.subtract == false);
        REQUIRE(c2.registers.f.carry == false);
        REQUIRE(c2.registers.f.half_carry == false);
    }
}

TEST_CASE("XOR Unit Tests", "[cpu][8-bit][xor]") {
    Instruction instruct;
    instruct.type = InstructionType::XOR;
    instruct.target = ArithmeticTarget::B;

    SECTION ("XOR 1") {
        CPU c1;

        c1.registers.a = 1;
        c1.registers.b = 2;

        c1.execute(instruct);

        REQUIRE(c1.registers.a == 3);
        REQUIRE(c1.registers.f.zero == false);
        REQUIRE(c1.registers.f.subtract == false);
        REQUIRE(c1.registers.f.carry == false);
        REQUIRE(c1.registers.f.half_carry == false);
    }

    SECTION ("XOR 2") {
        CPU c2;

        c2.registers.a = 0;
        c2.registers.b = 0;

        c2.execute(instruct);

        REQUIRE(c2.registers.a == 0);
        REQUIRE(c2.registers.f.zero == true);
        REQUIRE(c2.registers.f.subtract == false);
        REQUIRE(c2.registers.f.carry == false);
        REQUIRE(c2.registers.f.half_carry == false);
    }
}

TEST_CASE("CP Unit Tests", "[cpu][8-bit][cp]") {
    Instruction instruct;
    instruct.type = InstructionType::CP;
    instruct.target = ArithmeticTarget::B;

    SECTION ("CP w/o Overflow") {
        CPU c1;

        c1.registers.a = 2;
        c1.registers.b = 1;

        c1.execute(instruct);

        REQUIRE(c1.registers.f.zero == false);
        REQUIRE(c1.registers.f.subtract == true);
        REQUIRE(c1.registers.f.carry == false);
        REQUIRE(c1.registers.f.half_carry == false);
    }

    SECTION ("CP w Underflow") {
        CPU c2;

        c2.registers.a = 1;
        c2.registers.b = 2;

        c2.execute(instruct);

        REQUIRE(c2.registers.f.zero == false);
        REQUIRE(c2.registers.f.subtract == true);
        REQUIRE(c2.registers.f.carry == true);
        REQUIRE(c2.registers.f.half_carry == true);
    }

    SECTION ("CP to 0") {
        CPU c3;
        c3.registers.a = 1;
        c3.registers.b = 1;

        c3.execute(instruct);

        REQUIRE(c3.registers.f.zero == true);
        REQUIRE(c3.registers.f.subtract == true);
        REQUIRE(c3.registers.f.carry == false);
        REQUIRE(c3.registers.f.half_carry == false);
    }
}