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
    instruct.target_8bit = ArithmeticTarget8Bit::B;

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
    instruct.target_8bit = ArithmeticTarget8Bit::B;

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
    instruct.target_8bit = ArithmeticTarget8Bit::B;

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
    instruct.target_8bit = ArithmeticTarget8Bit::B;

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
    instruct.target_8bit = ArithmeticTarget8Bit::B;

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
    instruct.target_8bit = ArithmeticTarget8Bit::B;

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
    instruct.target_8bit = ArithmeticTarget8Bit::B;

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
    instruct.target_8bit = ArithmeticTarget8Bit::B;

    SECTION ("CP w/o Overflow") {
        CPU c1;

        c1.registers.a = 2;
        c1.registers.b = 1;

        c1.execute(instruct);

        REQUIRE(c1.registers.a == 2);
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

        REQUIRE(c2.registers.a == 1);
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

        REQUIRE(c3.registers.a == 1);
        REQUIRE(c3.registers.f.zero == true);
        REQUIRE(c3.registers.f.subtract == true);
        REQUIRE(c3.registers.f.carry == false);
        REQUIRE(c3.registers.f.half_carry == false);
    }
}

TEST_CASE("INC Unit Tests", "[cpu][8-bit][inc]") {
    Instruction instruct;
    instruct.type = InstructionType::INC;

    SECTION ("INC Register A") {
        instruct.target_8bit = ArithmeticTarget8Bit::A;

        CPU c;

        c.registers.a = 1;

        c.execute(instruct);

        REQUIRE(c.registers.a == 2);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == false);
    }

    SECTION ("INC Register B") {
        instruct.target_8bit = ArithmeticTarget8Bit::B;

        CPU c;

        c.registers.b = 1;

        c.execute(instruct);

        REQUIRE(c.registers.b == 2);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == false);
    }

    SECTION ("INC Register C") {
        instruct.target_8bit = ArithmeticTarget8Bit::C;

        CPU c;

        c.registers.c = 1;

        c.execute(instruct);

        REQUIRE(c.registers.c == 2);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == false);
    }

    SECTION ("INC Register D") {
        instruct.target_8bit = ArithmeticTarget8Bit::D;

        CPU c;

        c.registers.d = 1;

        c.execute(instruct);

        REQUIRE(c.registers.d == 2);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == false);
    }

    SECTION ("INC Register E") {
        instruct.target_8bit = ArithmeticTarget8Bit::E;

        CPU c;

        c.registers.e = 1;

        c.execute(instruct);

        REQUIRE(c.registers.e == 2);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == false);
    }

    SECTION ("INC Register H") {
        instruct.target_8bit = ArithmeticTarget8Bit::H;

        CPU c;

        c.registers.h = 1;

        c.execute(instruct);

        REQUIRE(c.registers.h == 2);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == false);
    }

    SECTION ("INC Register L") {
        instruct.target_8bit = ArithmeticTarget8Bit::L;

        CPU c;

        c.registers.l = 1;

        c.execute(instruct);

        REQUIRE(c.registers.l == 2);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == false);
    }

    SECTION ("INC - Overflow - Carry Set to False") {
        instruct.target_8bit = ArithmeticTarget8Bit::B;

        CPU c;

        c.registers.b = 255;
        c.registers.f.carry = false;

        c.execute(instruct);

        REQUIRE(c.registers.b == 0);
        REQUIRE(c.registers.f.zero == true);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == true);
    }

    SECTION ("INC - Overflow - Carry Set to True") {
        instruct.target_8bit = ArithmeticTarget8Bit::B;

        CPU c;

        c.registers.b = 255;
        c.registers.f.carry = true;

        c.execute(instruct);

        REQUIRE(c.registers.b == 0);
        REQUIRE(c.registers.f.zero == true);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == true);
        REQUIRE(c.registers.f.half_carry == true);
    }

    SECTION ("INC - Half Carry") {
        instruct.target_8bit = ArithmeticTarget8Bit::B;

        CPU c;

        c.registers.b = 15;

        c.execute(instruct);

        REQUIRE(c.registers.b == 16);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == true);
    }
}

TEST_CASE("DEC Unit Tests", "[cpu][8-bit][dec]") {
    Instruction instruct;
    instruct.type = InstructionType::DEC;

    SECTION ("DEC Register A") {
        instruct.target_8bit = ArithmeticTarget8Bit::A;

        CPU c;

        c.registers.a = 2;

        c.execute(instruct);

        REQUIRE(c.registers.a == 1);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == true);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == false);
    }

    SECTION ("DEC Register B") {
        instruct.target_8bit = ArithmeticTarget8Bit::B;

        CPU c;

        c.registers.b = 2;

        c.execute(instruct);

        REQUIRE(c.registers.b == 1);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == true);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == false);
    }

    SECTION ("DEC Register C") {
        instruct.target_8bit = ArithmeticTarget8Bit::C;

        CPU c;

        c.registers.c = 2;

        c.execute(instruct);

        REQUIRE(c.registers.c == 1);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == true);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == false);
    }

    SECTION ("DEC Register D") {
        instruct.target_8bit = ArithmeticTarget8Bit::D;

        CPU c;

        c.registers.d = 2;

        c.execute(instruct);

        REQUIRE(c.registers.d == 1);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == true);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == false);
    }

    SECTION ("DEC Register E") {
        instruct.target_8bit = ArithmeticTarget8Bit::E;

        CPU c;

        c.registers.e = 2;

        c.execute(instruct);

        REQUIRE(c.registers.e == 1);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == true);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == false);
    }

    SECTION ("DEC Register H") {
        instruct.target_8bit = ArithmeticTarget8Bit::H;

        CPU c;

        c.registers.h = 2;

        c.execute(instruct);

        REQUIRE(c.registers.h == 1);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == true);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == false);
    }

    SECTION ("DEC Register L") {
        instruct.target_8bit = ArithmeticTarget8Bit::L;

        CPU c;

        c.registers.l = 2;

        c.execute(instruct);

        REQUIRE(c.registers.l == 1);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == true);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == false);
    }

    SECTION ("DEC - Decrementing 0") {
        instruct.target_8bit = ArithmeticTarget8Bit::B;

        CPU c;

        c.registers.b = 0;

        c.execute(instruct);

        REQUIRE(c.registers.b == 255);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == true);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == true);
    }

    SECTION ("DEC - Decrementing 1") {
        instruct.target_8bit = ArithmeticTarget8Bit::B;

        CPU c;

        c.registers.b = 1;

        c.execute(instruct);

        REQUIRE(c.registers.b == 0);
        REQUIRE(c.registers.f.zero == true);
        REQUIRE(c.registers.f.subtract == true);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == false);
    }
}

TEST_CASE("ADD HL_PTR and NUM", "[cpu][8-bit][add][hl_ptr][num]") {
    Instruction instruct;
    instruct.type = InstructionType::ADD;

    SECTION("ADD HL_PTR w/o Overflow") {
        CPU c;

        c.registers.a = 1;
        c.registers.set_hl(0xC000);
        c.bus.write_byte(c.registers.get_hl(), 1);

        instruct.target_8bit = ArithmeticTarget8Bit::HL_PTR;

        c.execute(instruct);

        REQUIRE(c.registers.a == 2);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == false);
    }

    SECTION("ADD NUM w/o Overflow") {
        CPU c;

        c.registers.a = 1;
        instruct.target_8bit = ArithmeticTarget8Bit::NUM;
        c.bus.write_byte(c.pc + 1, 1);

        c.execute(instruct);

        REQUIRE(c.registers.a == 2);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == false);
    }
}

TEST_CASE("ADC HL_PTR and NUM", "[cpu][8-bit][adc][hl_ptr][num]") {
    Instruction instruct;
    instruct.type = InstructionType::ADC;

    SECTION("ADC HL_PTR - No Carry") {
        CPU c;

        c.registers.a = 1;
        c.registers.f.carry = false;
        c.registers.set_hl(0xC100);
        c.bus.write_byte(c.registers.get_hl(), 1);

        instruct.target_8bit = ArithmeticTarget8Bit::HL_PTR;

        c.execute(instruct);

        REQUIRE(c.registers.a == 2);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == false);
    }

    SECTION("ADC NUM - No Carry") {
        CPU c;

        c.registers.a = 1;
        c.registers.f.carry = false;
        instruct.target_8bit = ArithmeticTarget8Bit::NUM;
        c.bus.write_byte(c.pc + 1, 1);

        c.execute(instruct);

        REQUIRE(c.registers.a == 2);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == false);
    }
}

TEST_CASE("SUB HL_PTR and NUM", "[cpu][8-bit][sub][hl_ptr][num]") {
    Instruction instruct;
    instruct.type = InstructionType::SUB;

    SECTION("SUB HL_PTR w/o Underflow") {
        CPU c;

        c.registers.a = 2;
        c.registers.set_hl(0xC200);
        c.bus.write_byte(c.registers.get_hl(), 1);

        instruct.target_8bit = ArithmeticTarget8Bit::HL_PTR;

        c.execute(instruct);

        REQUIRE(c.registers.a == 1);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == true);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == false);
    }

    SECTION("SUB NUM w/o Underflow") {
        CPU c;

        c.registers.a = 2;
        instruct.target_8bit = ArithmeticTarget8Bit::NUM;
        c.bus.write_byte(c.pc + 1, 1);

        c.execute(instruct);

        REQUIRE(c.registers.a == 1);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == true);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == false);
    }
}

TEST_CASE("SBC HL_PTR and NUM", "[cpu][8-bit][sbc][hl_ptr][num]") {
    Instruction instruct;
    instruct.type = InstructionType::SBC;

    SECTION("SBC HL_PTR - No Carry") {
        CPU c;

        c.registers.a = 2;
        c.registers.f.carry = false;
        c.registers.set_hl(0xC300);
        c.bus.write_byte(c.registers.get_hl(), 1);

        instruct.target_8bit = ArithmeticTarget8Bit::HL_PTR;

        c.execute(instruct);

        REQUIRE(c.registers.a == 1);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == true);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == false);
    }

    SECTION("SBC NUM - No Carry") {
        CPU c;

        c.registers.a = 2;
        c.registers.f.carry = false;
        instruct.target_8bit = ArithmeticTarget8Bit::NUM;
        c.bus.write_byte(c.pc + 1, 1);

        c.execute(instruct);

        REQUIRE(c.registers.a == 1);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == true);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == false);
    }
}

TEST_CASE("AND HL_PTR and NUM", "[cpu][8-bit][and][hl_ptr][num]") {
    Instruction instruct;
    instruct.type = InstructionType::AND;

    SECTION("AND HL_PTR - No Zero") {
        CPU c;

        c.registers.a = 1;
        c.registers.set_hl(0xC400);
        c.bus.write_byte(c.registers.get_hl(), 1);

        instruct.target_8bit = ArithmeticTarget8Bit::HL_PTR;

        c.execute(instruct);

        REQUIRE(c.registers.a == 1);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == true);
    }

    SECTION("AND NUM - Zero") {
        CPU c;

        c.registers.a = 100;
        instruct.target_8bit = ArithmeticTarget8Bit::NUM;
        c.bus.write_byte(c.pc + 1, 0);

        c.execute(instruct);

        REQUIRE(c.registers.a == 0);
        REQUIRE(c.registers.f.zero == true);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == true);
    }
}

TEST_CASE("OR HL_PTR and NUM", "[cpu][8-bit][or][hl_ptr][num]") {
    Instruction instruct;
    instruct.type = InstructionType::OR;

    SECTION("OR HL_PTR - No Zero") {
        CPU c;

        c.registers.a = 1;
        c.registers.set_hl(0xC500);
        c.bus.write_byte(c.registers.get_hl(), 1);

        instruct.target_8bit = ArithmeticTarget8Bit::HL_PTR;

        c.execute(instruct);

        REQUIRE(c.registers.a == 1);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == false);
    }

    SECTION("OR NUM - Zero") {
        CPU c;

        c.registers.a = 0;
        instruct.target_8bit = ArithmeticTarget8Bit::NUM;
        c.bus.write_byte(c.pc + 1, 0);

        c.execute(instruct);

        REQUIRE(c.registers.a == 0);
        REQUIRE(c.registers.f.zero == true);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == false);
    }
}

TEST_CASE("XOR HL_PTR and NUM", "[cpu][8-bit][xor][hl_ptr][num]") {
    Instruction instruct;
    instruct.type = InstructionType::XOR;

    SECTION("XOR HL_PTR") {
        CPU c;

        c.registers.a = 1;
        c.registers.set_hl(0xC600);
        c.bus.write_byte(c.registers.get_hl(), 2);

        instruct.target_8bit = ArithmeticTarget8Bit::HL_PTR;

        c.execute(instruct);

        REQUIRE(c.registers.a == 3);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == false);
    }

    SECTION("XOR NUM") {
        CPU c;

        c.registers.a = 0;
        instruct.target_8bit = ArithmeticTarget8Bit::NUM;
        c.bus.write_byte(c.pc + 1, 0);

        c.execute(instruct);

        REQUIRE(c.registers.a == 0);
        REQUIRE(c.registers.f.zero == true);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == false);
    }
}

TEST_CASE("CP HL_PTR and NUM", "[cpu][8-bit][cp][hl_ptr][num]") {
    Instruction instruct;
    instruct.type = InstructionType::CP;

    SECTION("CP HL_PTR - w/o Overflow") {
        CPU c;

        c.registers.a = 2;
        c.registers.set_hl(0xC700);
        c.bus.write_byte(c.registers.get_hl(), 1);

        instruct.target_8bit = ArithmeticTarget8Bit::HL_PTR;

        c.execute(instruct);

        REQUIRE(c.registers.a == 2);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == true);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == false);
    }

    SECTION("CP NUM - w/o Overflow") {
        CPU c;

        c.registers.a = 2;
        instruct.target_8bit = ArithmeticTarget8Bit::NUM;
        c.bus.write_byte(c.pc + 1, 1);

        c.execute(instruct);

        REQUIRE(c.registers.a == 2);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == true);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == false);
    }
}

TEST_CASE("SWAP HL_PTR Unit Tests", "[cpu][misc][swap][hl_ptr]") {
    Instruction instruct;
    instruct.type = InstructionType::SWAP;
    instruct.target_8bit = ArithmeticTarget8Bit::HL_PTR;

    CPU c;
    uint16_t addr = 0xC900;
    c.registers.set_hl(addr);
    c.bus.write_byte(addr, 0b10100001);

    c.execute(instruct);

    REQUIRE(c.bus.read_byte(addr) == 0b00011010);
    REQUIRE(c.registers.f.zero == false);
    REQUIRE(c.registers.f.subtract == false);
    REQUIRE(c.registers.f.carry == false);
    REQUIRE(c.registers.f.half_carry == false);
}

TEST_CASE("BIT/SET/RES HL_PTR Unit Tests", "[cpu][bit_opcode][hl_ptr]") {
    Instruction instruct;

    // BIT HL_PTR
    instruct.type = InstructionType::BIT;
    instruct.target_8bit = ArithmeticTarget8Bit::HL_PTR;
    instruct.bit_index = 3;

    {
        CPU c;
        uint16_t addr = 0xCA00;
        c.registers.set_hl(addr);
        c.bus.write_byte(addr, 0b00001000);

        c.execute(instruct);

        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == true);
    }

    // SET HL_PTR
    instruct.type = InstructionType::SET;
    instruct.bit_index = 3;

    {
        CPU c;
        uint16_t addr = 0xCB00;
        c.registers.set_hl(addr);
        c.bus.write_byte(addr, 0x00);

        c.execute(instruct);

        REQUIRE(c.bus.read_byte(addr) == 0b00001000);
    }

    // RES HL_PTR
    instruct.type = InstructionType::RES;
    instruct.bit_index = 3;

    {
        CPU c;
        uint16_t addr = 0xCC00;
        c.registers.set_hl(addr);
        c.bus.write_byte(addr, 0xFF);

        c.execute(instruct);

        REQUIRE(c.bus.read_byte(addr) == (uint8_t)(0xFF & ~(1 << 3)));
    }
}

TEST_CASE("Rotate/Shift HL_PTR Unit Tests", "[cpu][rotate][hl_ptr]") {
    Instruction instruct;

    // RLC HL_PTR
    instruct.type = InstructionType::RLC;
    instruct.target_8bit = ArithmeticTarget8Bit::HL_PTR;
    {
        CPU c;
        uint16_t addr = 0xCD00;
        c.registers.set_hl(addr);
        c.bus.write_byte(addr, 0b00001001);
        c.registers.f.carry = 1;

        c.execute(instruct);

        REQUIRE(c.bus.read_byte(addr) == 0b00010010);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == false);
    }

    // RL HL_PTR
    instruct.type = InstructionType::RL;
    {
        CPU c;
        uint16_t addr = 0xCE00;
        c.registers.set_hl(addr);
        c.bus.write_byte(addr, 0b00001001);
        c.registers.f.carry = 1;

        c.execute(instruct);

        REQUIRE(c.bus.read_byte(addr) == 0b00010011);
        REQUIRE(c.registers.f.carry == false);
    }

    // RRC HL_PTR
    instruct.type = InstructionType::RRC;
    {
        CPU c;
        uint16_t addr = 0xCF00;
        c.registers.set_hl(addr);
        c.bus.write_byte(addr, 0b00001001);
        c.registers.f.carry = 1;

        c.execute(instruct);

        REQUIRE(c.bus.read_byte(addr) == 0b10000100);
        REQUIRE(c.registers.f.carry == true);
    }

    // RR HL_PTR
    instruct.type = InstructionType::RR;
    {
        CPU c;
        uint16_t addr = 0xD000;
        c.registers.set_hl(addr);
        c.bus.write_byte(addr, 0b00001001);
        c.registers.f.carry = 1;

        c.execute(instruct);

        REQUIRE(c.bus.read_byte(addr) == 0b10000100);
        REQUIRE(c.registers.f.carry == true);
    }

    // SLA HL_PTR
    instruct.type = InstructionType::SLA;
    {
        CPU c;
        uint16_t addr = 0xD100;
        c.registers.set_hl(addr);
        c.bus.write_byte(addr, 0b10101010);
        c.registers.f.carry = 0;

        c.execute(instruct);

        REQUIRE(c.bus.read_byte(addr) == 0b01010100);
        REQUIRE(c.registers.f.carry == true);
    }

    // SRA HL_PTR
    instruct.type = InstructionType::SRA;
    {
        CPU c;
        uint16_t addr = 0xD200;
        c.registers.set_hl(addr);
        c.bus.write_byte(addr, 0b00001001);
        c.registers.f.carry = 0;

        c.execute(instruct);

        REQUIRE(c.bus.read_byte(addr) == 0b00000100);
        REQUIRE(c.registers.f.carry == 1);
    }

    // SRL HL_PTR
    instruct.type = InstructionType::SRL;
    {
        CPU c;
        uint16_t addr = 0xD300;
        c.registers.set_hl(addr);
        c.bus.write_byte(addr, 0b10000000);
        c.registers.f.carry = 1;

        c.execute(instruct);

        REQUIRE(c.bus.read_byte(addr) == 0b01000000);
        REQUIRE(c.registers.f.carry == 0);
    }
}

TEST_CASE("INC/DEC HL_PTR Unit Tests", "[cpu][8-bit][inc][dec][hl_ptr]") {
    Instruction instruct;

    // INC HL_PTR normal
    instruct.type = InstructionType::INC;
    instruct.target_8bit = ArithmeticTarget8Bit::HL_PTR;
    {
        CPU c;
        uint16_t addr = 0xD400;
        c.registers.set_hl(addr);
        c.bus.write_byte(addr, 1);

        c.execute(instruct);

        REQUIRE(c.bus.read_byte(addr) == 2);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
    }

    // INC HL_PTR - Overflow preserves carry
    {
        CPU c;
        uint16_t addr = 0xD500;
        c.registers.set_hl(addr);
        c.bus.write_byte(addr, 255);
        c.registers.f.carry = true;

        c.execute(instruct);

        REQUIRE(c.bus.read_byte(addr) == 0);
        REQUIRE(c.registers.f.zero == true);
        REQUIRE(c.registers.f.carry == true);
        REQUIRE(c.registers.f.half_carry == true);
    }

    // DEC HL_PTR - underflow
    instruct.type = InstructionType::DEC;
    {
        CPU c;
        uint16_t addr = 0xD600;
        c.registers.set_hl(addr);
        c.bus.write_byte(addr, 0);

        c.execute(instruct);

        REQUIRE(c.bus.read_byte(addr) == 255);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == true);
        REQUIRE(c.registers.f.half_carry == true);
    }

    // DEC HL_PTR - to zero
    {
        CPU c;
        uint16_t addr = 0xD700;
        c.registers.set_hl(addr);
        c.bus.write_byte(addr, 1);

        c.execute(instruct);

        REQUIRE(c.bus.read_byte(addr) == 0);
        REQUIRE(c.registers.f.zero == true);
        REQUIRE(c.registers.f.subtract == true);
        REQUIRE(c.registers.f.half_carry == false);
    }
}


TEST_CASE("SWAP Unit Tests", "[cpu][misc][swap]") {
    Instruction instruct;
    instruct.type = InstructionType::SWAP;

    SECTION ("SWAP Register A") {
        instruct.target_8bit = ArithmeticTarget8Bit::A;

        CPU c;

        c.registers.a = 0b10100001;

        c.execute(instruct);

        REQUIRE(c.registers.a == 0b00011010);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == false);
    }

    SECTION ("SWAP Register B") {
        instruct.target_8bit = ArithmeticTarget8Bit::B;

        CPU c;

        c.registers.b = 0b10100001;

        c.execute(instruct);

        REQUIRE(c.registers.b == 0b00011010);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == false);
    }

    SECTION ("SWAP Register C") {
        instruct.target_8bit = ArithmeticTarget8Bit::C;

        CPU c;

        c.registers.c = 0b10100001;

        c.execute(instruct);

        REQUIRE(c.registers.c == 0b00011010);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == false);
    }

    SECTION ("SWAP Register D") {
        instruct.target_8bit = ArithmeticTarget8Bit::D;

        CPU c;

        c.registers.d = 0b10100001;

        c.execute(instruct);

        REQUIRE(c.registers.d == 0b00011010);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == false);
    }

    SECTION ("SWAP Register E") {
        instruct.target_8bit = ArithmeticTarget8Bit::E;

        CPU c;

        c.registers.e = 0b10100001;

        c.execute(instruct);

        REQUIRE(c.registers.e == 0b00011010);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == false);
    }

    SECTION ("SWAP Register H") {
        instruct.target_8bit = ArithmeticTarget8Bit::H;

        CPU c;

        c.registers.h = 0b10100001;

        c.execute(instruct);

        REQUIRE(c.registers.h == 0b00011010);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == false);
    }

    SECTION ("SWAP Register L") {
        instruct.target_8bit = ArithmeticTarget8Bit::L;

        CPU c;

        c.registers.l = 0b10100001;

        c.execute(instruct);

        REQUIRE(c.registers.l == 0b00011010);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == false);
    }
}

TEST_CASE("SCF Unit Tests", "[cpu][misc][scf]") {
    Instruction instruct;
    instruct.type = InstructionType::SCF;

    SECTION ("Carry Flag is False") {
        CPU c;

        c.registers.f.carry = false;

        c.execute(instruct);

        REQUIRE(c.registers.f.carry == true);

        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.half_carry == false);
    }

    SECTION ("Carry Flag is True") {
        CPU c;

        c.registers.f.carry = true;

        c.execute(instruct);

        REQUIRE(c.registers.f.carry == true);

        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.half_carry == false);
    }

    SECTION ("Zero Flag is True") {
        CPU c;

        c.registers.f.carry = false;
        c.registers.f.zero = true;

        c.execute(instruct);

        REQUIRE(c.registers.f.carry == true);

        REQUIRE(c.registers.f.zero == true);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.half_carry == false);
    }
}

TEST_CASE("CCF Unit Tests", "[cpu][misc][ccf]") {
    Instruction instruct;
    instruct.type = InstructionType::CCF;

    SECTION ("Carry Flag is False") {
        CPU c;

        c.registers.f.carry = false;

        c.execute(instruct);

        REQUIRE(c.registers.f.carry == true);

        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.half_carry == false);
    }

    SECTION ("Carry Flag is True") {
        CPU c;

        c.registers.f.carry = true;

        c.execute(instruct);

        REQUIRE(c.registers.f.carry == false);

        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.half_carry == false);
    }

    SECTION ("Zero Flag is True") {
        CPU c;

        c.registers.f.carry = true;
        c.registers.f.zero = true;

        c.execute(instruct);

        REQUIRE(c.registers.f.carry == false);

        REQUIRE(c.registers.f.zero == true);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.half_carry == false);
    }
}

TEST_CASE("CPL Unit Tests", "[cpu][misc][cpl]") {
    Instruction instruct;
    instruct.type = InstructionType::CPL;

    SECTION ("CPL 1") {
        CPU c;

        c.registers.a = 0b11111111;

        c.execute(instruct);

        REQUIRE(c.registers.a == 0b00000000);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == true);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == true);
    }

    SECTION ("CPL 2") {
        CPU c;

        c.registers.a = 0b01010101;

        c.execute(instruct);

        REQUIRE(c.registers.a == 0b10101010);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == true);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == true);
    }

    SECTION ("CPL 3 - Zero Flag Set") {
        CPU c;

        c.registers.a = 0;
        c.registers.f.zero = true;

        c.execute(instruct);

        REQUIRE(c.registers.a == 255);
        REQUIRE(c.registers.f.zero == true);
        REQUIRE(c.registers.f.subtract == true);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == true);
    }

    SECTION ("CPL 4 - Carry Flag Set") {
        CPU c;

        c.registers.a = 0;
        c.registers.f.carry = true;

        c.execute(instruct);

        REQUIRE(c.registers.a == 255);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == true);
        REQUIRE(c.registers.f.carry == true);
        REQUIRE(c.registers.f.half_carry == true);
    }

    SECTION ("CPL 5 - Zero and Carry Flag Set") {
        CPU c;

        c.registers.a = 0;
        c.registers.f.zero = true;
        c.registers.f.carry = true;

        c.execute(instruct);

        REQUIRE(c.registers.a == 255);
        REQUIRE(c.registers.f.zero == true);
        REQUIRE(c.registers.f.subtract == true);
        REQUIRE(c.registers.f.carry == true);
        REQUIRE(c.registers.f.half_carry == true);
    }
}

TEST_CASE("BIT Unit Tests", "[cpu][bit_opcode][bit]") {
    Instruction instruct;
    instruct.type = InstructionType::BIT;

    SECTION ("BIT 1") {
        CPU c;
        instruct.target_8bit = ArithmeticTarget8Bit::A;
        instruct.bit_index = 3;

        c.registers.a = 0b00001000;

        c.execute(instruct);

        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == true);
    }

    SECTION ("BIT 2") {
        CPU c;
        instruct.target_8bit = ArithmeticTarget8Bit::A;
        instruct.bit_index = 2;

        c.registers.a = 0b00001000;

        c.execute(instruct);

        REQUIRE(c.registers.f.zero == true);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == true);
    }

    SECTION ("BIT 3") {
        CPU c;
        instruct.target_8bit = ArithmeticTarget8Bit::A;
        instruct.bit_index = 4;

        c.registers.a = 0b00001000;

        c.execute(instruct);

        REQUIRE(c.registers.f.zero == true);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == true);
    }

    SECTION ("BIT 4") {
        CPU c;
        instruct.target_8bit = ArithmeticTarget8Bit::B;
        instruct.bit_index = 3;

        c.registers.b = 0b00001000;

        c.execute(instruct);

        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == true);
    }

    SECTION ("BIT 5") {
        CPU c;
        instruct.target_8bit = ArithmeticTarget8Bit::C;
        instruct.bit_index = 3;

        c.registers.c = 0b00001000;

        c.execute(instruct);

        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == true);
    }

    SECTION ("BIT 6") {
        CPU c;
        instruct.target_8bit = ArithmeticTarget8Bit::D;
        instruct.bit_index = 3;

        c.registers.d = 0b00001000;

        c.execute(instruct);

        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == true);
    }

    SECTION ("BIT 7") {
        CPU c;
        instruct.target_8bit = ArithmeticTarget8Bit::E;
        instruct.bit_index = 3;

        c.registers.e = 0b00001000;

        c.execute(instruct);

        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == true);
    }

    SECTION ("BIT 8") {
        CPU c;
        instruct.target_8bit = ArithmeticTarget8Bit::H;
        instruct.bit_index = 3;

        c.registers.h = 0b00001000;

        c.execute(instruct);

        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == true);
    }

    SECTION ("BIT 9") {
        CPU c;
        instruct.target_8bit = ArithmeticTarget8Bit::L;
        instruct.bit_index = 3;

        c.registers.l = 0b00001000;

        c.execute(instruct);

        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == true);
    }

    SECTION ("BIT 10 - Carry set to True") {
        CPU c;
        instruct.target_8bit = ArithmeticTarget8Bit::A;
        instruct.bit_index = 3;

        c.registers.a = 0b00001000;
        c.registers.f.carry = true;

        c.execute(instruct);

        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == true);
        REQUIRE(c.registers.f.half_carry == true);
    }
}

TEST_CASE("SET Unit Tests", "[cpu][bit_opcode][set]") {
    Instruction instruct;
    instruct.type = InstructionType::SET;

    SECTION ("SET 1") {
        CPU c;
        instruct.target_8bit = ArithmeticTarget8Bit::A;
        instruct.bit_index = 3;

        c.registers.a = 0;

        c.execute(instruct);

        REQUIRE(c.registers.a == 0b00001000);
    }

    SECTION ("SET 2") {
        CPU c;
        instruct.target_8bit = ArithmeticTarget8Bit::A;
        instruct.bit_index = 7;

        c.registers.a = 0;

        c.execute(instruct);

        REQUIRE(c.registers.a == 0b10000000);
    }

    SECTION ("SET 3") {
        CPU c;
        instruct.target_8bit = ArithmeticTarget8Bit::A;
        instruct.bit_index = 0;

        c.registers.a = 0;

        c.execute(instruct);

        REQUIRE(c.registers.a == 0b00000001);
    }

    SECTION ("SET 4") {
        CPU c;
        instruct.target_8bit = ArithmeticTarget8Bit::B;
        instruct.bit_index = 3;

        c.registers.b = 0;

        c.execute(instruct);

        REQUIRE(c.registers.b == 0b00001000);
    }

    SECTION ("SET 5") {
        CPU c;
        instruct.target_8bit = ArithmeticTarget8Bit::C;
        instruct.bit_index = 3;

        c.registers.c = 0;

        c.execute(instruct);

        REQUIRE(c.registers.c == 0b00001000);
    }

    SECTION ("SET 6") {
        CPU c;
        instruct.target_8bit = ArithmeticTarget8Bit::D;
        instruct.bit_index = 3;

        c.registers.d = 0;

        c.execute(instruct);

        REQUIRE(c.registers.d == 0b00001000);
    }

    SECTION ("SET 7") {
        CPU c;
        instruct.target_8bit = ArithmeticTarget8Bit::E;
        instruct.bit_index = 3;

        c.registers.e = 0;

        c.execute(instruct);

        REQUIRE(c.registers.e == 0b00001000);
    }

    SECTION ("SET 8") {
        CPU c;
        instruct.target_8bit = ArithmeticTarget8Bit::H;
        instruct.bit_index = 3;

        c.registers.h = 0;

        c.execute(instruct);

        REQUIRE(c.registers.h == 0b00001000);
    }

    SECTION ("SET 9") {
        CPU c;
        instruct.target_8bit = ArithmeticTarget8Bit::L;
        instruct.bit_index = 3;

        c.registers.l = 0;

        c.execute(instruct);

        REQUIRE(c.registers.l == 0b00001000);
    }
}

TEST_CASE("RES Unit Tests", "[cpu][bit_opcode][reset]") {
    Instruction instruct;
    instruct.type = InstructionType::RES;

    SECTION ("RES 1") {
        CPU c;
        instruct.target_8bit = ArithmeticTarget8Bit::A;
        instruct.bit_index = 3;

        c.registers.a = 255;

        c.execute(instruct);

        REQUIRE(c.registers.a == 0b11110111);
    }

    SECTION ("RES 2") {
        CPU c;
        instruct.target_8bit = ArithmeticTarget8Bit::A;
        instruct.bit_index = 7;

        c.registers.a = 255;

        c.execute(instruct);

        REQUIRE(c.registers.a == 0b01111111);
    }

    SECTION ("RES 3") {
        CPU c;
        instruct.target_8bit = ArithmeticTarget8Bit::A;
        instruct.bit_index = 0;

        c.registers.a = 255;

        c.execute(instruct);

        REQUIRE(c.registers.a == 0b11111110);
    }

    SECTION ("RES 4") {
        CPU c;
        instruct.target_8bit = ArithmeticTarget8Bit::B;
        instruct.bit_index = 0;

        c.registers.b = 255;

        c.execute(instruct);

        REQUIRE(c.registers.b == 0b11111110);
    }

    SECTION ("RES 5") {
        CPU c;
        instruct.target_8bit = ArithmeticTarget8Bit::C;
        instruct.bit_index = 0;

        c.registers.c = 255;

        c.execute(instruct);

        REQUIRE(c.registers.c == 0b11111110);
    }

    SECTION ("RES 6") {
        CPU c;
        instruct.target_8bit = ArithmeticTarget8Bit::D;
        instruct.bit_index = 0;

        c.registers.d = 255;

        c.execute(instruct);

        REQUIRE(c.registers.d == 0b11111110);
    }

    SECTION ("RES 7") {
        CPU c;
        instruct.target_8bit = ArithmeticTarget8Bit::E;
        instruct.bit_index = 0;

        c.registers.e = 255;

        c.execute(instruct);

        REQUIRE(c.registers.e == 0b11111110);
    }

    SECTION ("RES 8") {
        CPU c;
        instruct.target_8bit = ArithmeticTarget8Bit::H;
        instruct.bit_index = 0;

        c.registers.h = 255;

        c.execute(instruct);

        REQUIRE(c.registers.h == 0b11111110);
    }

    SECTION ("RES 9") {
        CPU c;
        instruct.target_8bit = ArithmeticTarget8Bit::L;
        instruct.bit_index = 0;

        c.registers.l = 255;

        c.execute(instruct);

        REQUIRE(c.registers.l == 0b11111110);
    }
}

TEST_CASE("ADDHL Unit Tests", "[cpu][16-bit][addhl]") {
    Instruction instruct;
    instruct.type = InstructionType::ADDHL;


    SECTION ("ADDHL w/o Overflow") {
        CPU c;
        instruct.target_16bit = ArithmeticTarget16Bit::BC;

        c.registers.set_hl(1);
        c.registers.set_bc(1);

        c.execute(instruct);

        REQUIRE(c.registers.get_hl() == 2);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == false);
    }

    SECTION ("ADDHL w Overflow") {
        CPU c;
        instruct.target_16bit = ArithmeticTarget16Bit::BC;

        c.registers.set_hl(65535);
        c.registers.set_bc(1);

        c.execute(instruct);

        REQUIRE(c.registers.get_hl() == 0);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == true);
        REQUIRE(c.registers.f.half_carry == true);
    }

    SECTION ("ADDHL w Half Carry") {
        CPU c;
        instruct.target_16bit = ArithmeticTarget16Bit::BC;

        c.registers.set_hl(4095);
        c.registers.set_bc(1);

        c.execute(instruct);

        REQUIRE(c.registers.get_hl() == 4096);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == true);
    }

    SECTION ("ADDHL - DE") {
        CPU c;
        instruct.target_16bit = ArithmeticTarget16Bit::DE;

        c.registers.set_hl(1);
        c.registers.set_de(1);

        c.execute(instruct);

        REQUIRE(c.registers.get_hl() == 2);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == false);
    }

    SECTION ("ADDHL - HL") {
        CPU c;
        instruct.target_16bit = ArithmeticTarget16Bit::HL;

        c.registers.set_hl(1);

        c.execute(instruct);

        REQUIRE(c.registers.get_hl() == 2);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == false);
    }

    SECTION ("ADDHL - SP") {
        CPU c;
        instruct.target_16bit = ArithmeticTarget16Bit::SP;

        c.registers.set_hl(1);

        c.execute(instruct);

        REQUIRE(c.registers.get_hl() == 65535);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == false);
    }
}

TEST_CASE("RLCA Unit Tests", "[cpu][rotate][rlca]") {
    Instruction instruct;
    instruct.type = InstructionType::RLCA;


    SECTION ("RLCA 1") {
        CPU c;

        c.registers.a = 0b00001001;
        c.registers.f.carry = 1;

        c.execute(instruct);

        REQUIRE(c.registers.a == 0b00010010);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == false);
    }

    SECTION ("RLCA 2") {
        CPU c;

        c.registers.a = 0b10001001;
        c.registers.f.carry = 1;

        c.execute(instruct);

        REQUIRE(c.registers.a == 0b00010011);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == true);
        REQUIRE(c.registers.f.half_carry == false);
    }
}

TEST_CASE("RLA Unit Tests", "[cpu][rotate][rla]") {
    Instruction instruct;
    instruct.type = InstructionType::RLA;

    SECTION ("RLA 1") {
        CPU c;

        c.registers.a = 0b00001001;
        c.registers.f.carry = 1;

        c.execute(instruct);

        REQUIRE(c.registers.a == 0b00010011);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == false);
    }
}

TEST_CASE("RRCA Unit Tests", "[cpu][rotate][rrca]") {
    Instruction instruct;
    instruct.type = InstructionType::RRCA;


    SECTION ("RRCA 1") {
        CPU c;

        c.registers.a = 0b00001001;
        c.registers.f.carry = 1;

        c.execute(instruct);

        REQUIRE(c.registers.a == 0b10000100);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == true);
        REQUIRE(c.registers.f.half_carry == false);
    }

    SECTION ("RRCA 2") {
        CPU c;

        c.registers.a = 0b00001001;
        c.registers.f.carry = 0;

        c.execute(instruct);

        REQUIRE(c.registers.a == 0b10000100);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == true);
        REQUIRE(c.registers.f.half_carry == false);
    }

    SECTION ("RRCA 3") {
        CPU c;

        c.registers.a = 0b00001000;
        c.registers.f.carry = 0;

        c.execute(instruct);

        REQUIRE(c.registers.a == 0b00000100);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == false);
    }
}

TEST_CASE("RRA Unit Tests", "[cpu][rotate][rra]") {
    Instruction instruct;
    instruct.type = InstructionType::RRA;


    SECTION ("RRA") {
        CPU c;

        c.registers.a = 0b00001001;
        c.registers.f.carry = 1;

        c.execute(instruct);

        REQUIRE(c.registers.a == 0b10000100);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == true);
        REQUIRE(c.registers.f.half_carry == false);
    }
}

TEST_CASE("RLC Unit Tests", "[cpu][rotate][rlc]") {
    Instruction instruct;
    instruct.type = InstructionType::RLC;

    SECTION ("RLC 1") {
        instruct.target_8bit = ArithmeticTarget8Bit::A;
        CPU c;

        c.registers.a = 0b00001001;
        c.registers.f.carry = 1;

        c.execute(instruct);

        REQUIRE(c.registers.a == 0b00010010);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == false);
    }

    SECTION ("RLC 2") {
        instruct.target_8bit = ArithmeticTarget8Bit::A;
        CPU c;

        c.registers.a = 0b10001001;
        c.registers.f.carry = 1;

        c.execute(instruct);

        REQUIRE(c.registers.a == 0b00010011);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == true);
        REQUIRE(c.registers.f.half_carry == false);
    }

    SECTION ("RLC 3") {
        instruct.target_8bit = ArithmeticTarget8Bit::B;
        CPU c;

        c.registers.b = 0b00001001;
        c.registers.f.carry = 1;

        c.execute(instruct);

        REQUIRE(c.registers.b == 0b00010010);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == false);
    }

    SECTION ("RLC 4") {
        instruct.target_8bit = ArithmeticTarget8Bit::C;
        CPU c;

        c.registers.c = 0b00001001;
        c.registers.f.carry = 1;

        c.execute(instruct);

        REQUIRE(c.registers.c == 0b00010010);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == false);
    }

    SECTION ("RLC 5") {
        instruct.target_8bit = ArithmeticTarget8Bit::D;
        CPU c;

        c.registers.d = 0b00001001;
        c.registers.f.carry = 1;

        c.execute(instruct);

        REQUIRE(c.registers.d == 0b00010010);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == false);
    }

    SECTION ("RLC 6") {
        instruct.target_8bit = ArithmeticTarget8Bit::E;
        CPU c;

        c.registers.e = 0b00001001;
        c.registers.f.carry = 1;

        c.execute(instruct);

        REQUIRE(c.registers.e == 0b00010010);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == false);
    }

    SECTION ("RLC 7") {
        instruct.target_8bit = ArithmeticTarget8Bit::H;
        CPU c;

        c.registers.h = 0b00001001;
        c.registers.f.carry = 1;

        c.execute(instruct);

        REQUIRE(c.registers.h == 0b00010010);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == false);
    }

    SECTION ("RLC 8") {
        instruct.target_8bit = ArithmeticTarget8Bit::L;
        CPU c;

        c.registers.l = 0b00001001;
        c.registers.f.carry = 1;

        c.execute(instruct);

        REQUIRE(c.registers.l == 0b00010010);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == false);
    }
}

TEST_CASE("RL Unit Tests", "[cpu][rotate][rl]") {
    Instruction instruct;
    instruct.type = InstructionType::RL;

    SECTION ("RL 1") {
        instruct.target_8bit = ArithmeticTarget8Bit::A;
        CPU c;

        c.registers.a = 0b00001001;
        c.registers.f.carry = 1;

        c.execute(instruct);

        REQUIRE(c.registers.a == 0b00010011);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == false);
    }

    SECTION ("RL 2") {
        instruct.target_8bit = ArithmeticTarget8Bit::B;
        CPU c;

        c.registers.b = 0b00001001;
        c.registers.f.carry = 1;

        c.execute(instruct);

        REQUIRE(c.registers.b == 0b00010011);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == false);
    }

    SECTION ("RL 3") {
        instruct.target_8bit = ArithmeticTarget8Bit::C;
        CPU c;

        c.registers.c = 0b00001001;
        c.registers.f.carry = 1;

        c.execute(instruct);

        REQUIRE(c.registers.c == 0b00010011);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == false);
    }

    SECTION ("RL 4") {
        instruct.target_8bit = ArithmeticTarget8Bit::D;
        CPU c;

        c.registers.d = 0b00001001;
        c.registers.f.carry = 1;

        c.execute(instruct);

        REQUIRE(c.registers.d == 0b00010011);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == false);
    }

    SECTION ("RL 5") {
        instruct.target_8bit = ArithmeticTarget8Bit::E;
        CPU c;

        c.registers.e = 0b00001001;
        c.registers.f.carry = 1;

        c.execute(instruct);

        REQUIRE(c.registers.e == 0b00010011);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == false);
    }

    SECTION ("RL 6") {
        instruct.target_8bit = ArithmeticTarget8Bit::H;
        CPU c;

        c.registers.h = 0b00001001;
        c.registers.f.carry = 1;

        c.execute(instruct);

        REQUIRE(c.registers.h == 0b00010011);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == false);
    }

    SECTION ("RL 7") {
        instruct.target_8bit = ArithmeticTarget8Bit::L;
        CPU c;

        c.registers.l = 0b00001001;
        c.registers.f.carry = 1;

        c.execute(instruct);

        REQUIRE(c.registers.l == 0b00010011);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == false);
    }
}

TEST_CASE("RRC Unit Tests", "[cpu][rotate][rrc]") {
    Instruction instruct;
    instruct.type = InstructionType::RRC;


    SECTION ("RRC 1") {
        instruct.target_8bit = ArithmeticTarget8Bit::A;
        CPU c;

        c.registers.a = 0b00001001;
        c.registers.f.carry = 1;

        c.execute(instruct);

        REQUIRE(c.registers.a == 0b10000100);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == true);
        REQUIRE(c.registers.f.half_carry == false);
    }

    SECTION ("RRC 2") {
        instruct.target_8bit = ArithmeticTarget8Bit::B;
        CPU c;

        c.registers.b = 0b00001001;
        c.registers.f.carry = 1;

        c.execute(instruct);

        REQUIRE(c.registers.b == 0b10000100);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == true);
        REQUIRE(c.registers.f.half_carry == false);
    }

    SECTION ("RRC 3") {
        instruct.target_8bit = ArithmeticTarget8Bit::C;
        CPU c;

        c.registers.c = 0b00001001;
        c.registers.f.carry = 1;

        c.execute(instruct);

        REQUIRE(c.registers.c == 0b10000100);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == true);
        REQUIRE(c.registers.f.half_carry == false);
    }

    SECTION ("RRC 4") {
        instruct.target_8bit = ArithmeticTarget8Bit::D;
        CPU c;

        c.registers.d = 0b00001001;
        c.registers.f.carry = 1;

        c.execute(instruct);

        REQUIRE(c.registers.d == 0b10000100);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == true);
        REQUIRE(c.registers.f.half_carry == false);
    }

    SECTION ("RRC 5") {
        instruct.target_8bit = ArithmeticTarget8Bit::E;
        CPU c;

        c.registers.e = 0b00001001;
        c.registers.f.carry = 1;

        c.execute(instruct);

        REQUIRE(c.registers.e == 0b10000100);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == true);
        REQUIRE(c.registers.f.half_carry == false);
    }

    SECTION ("RRC 6") {
        instruct.target_8bit = ArithmeticTarget8Bit::H;
        CPU c;

        c.registers.h = 0b00001001;
        c.registers.f.carry = 1;

        c.execute(instruct);

        REQUIRE(c.registers.h == 0b10000100);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == true);
        REQUIRE(c.registers.f.half_carry == false);
    }

    SECTION ("RRC 7") {
        instruct.target_8bit = ArithmeticTarget8Bit::L;
        CPU c;

        c.registers.l = 0b00001001;
        c.registers.f.carry = 1;

        c.execute(instruct);

        REQUIRE(c.registers.l == 0b10000100);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == true);
        REQUIRE(c.registers.f.half_carry == false);
    }
}

TEST_CASE("RR Unit Tests", "[cpu][rotate][rr]") {
    Instruction instruct;
    instruct.type = InstructionType::RR;


    SECTION ("RR 1") {
        instruct.target_8bit = ArithmeticTarget8Bit::A;
        CPU c;

        c.registers.a = 0b00001001;
        c.registers.f.carry = 1;

        c.execute(instruct);

        REQUIRE(c.registers.a == 0b10000100);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == true);
        REQUIRE(c.registers.f.half_carry == false);
    }

    SECTION ("RR 2") {
        instruct.target_8bit = ArithmeticTarget8Bit::B;
        CPU c;

        c.registers.b = 0b00001001;
        c.registers.f.carry = 1;

        c.execute(instruct);

        REQUIRE(c.registers.b == 0b10000100);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == true);
        REQUIRE(c.registers.f.half_carry == false);
    }

    SECTION ("RR 3") {
        instruct.target_8bit = ArithmeticTarget8Bit::C;
        CPU c;

        c.registers.c = 0b00001001;
        c.registers.f.carry = 1;

        c.execute(instruct);

        REQUIRE(c.registers.c == 0b10000100);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == true);
        REQUIRE(c.registers.f.half_carry == false);
    }

    SECTION ("RR 4") {
        instruct.target_8bit = ArithmeticTarget8Bit::D;
        CPU c;

        c.registers.d = 0b00001001;
        c.registers.f.carry = 1;

        c.execute(instruct);

        REQUIRE(c.registers.d == 0b10000100);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == true);
        REQUIRE(c.registers.f.half_carry == false);
    }

    SECTION ("RR 5") {
        instruct.target_8bit = ArithmeticTarget8Bit::E;
        CPU c;

        c.registers.e = 0b00001001;
        c.registers.f.carry = 1;

        c.execute(instruct);

        REQUIRE(c.registers.e == 0b10000100);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == true);
        REQUIRE(c.registers.f.half_carry == false);
    }

    SECTION ("RR 6") {
        instruct.target_8bit = ArithmeticTarget8Bit::H;
        CPU c;

        c.registers.h = 0b00001001;
        c.registers.f.carry = 1;

        c.execute(instruct);

        REQUIRE(c.registers.h == 0b10000100);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == true);
        REQUIRE(c.registers.f.half_carry == false);
    }

    SECTION ("RR 7") {
        instruct.target_8bit = ArithmeticTarget8Bit::L;
        CPU c;

        c.registers.l = 0b00001001;
        c.registers.f.carry = 1;

        c.execute(instruct);

        REQUIRE(c.registers.l == 0b10000100);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == true);
        REQUIRE(c.registers.f.half_carry == false);
    }
}

TEST_CASE("SLA Unit Tests", "[cpu][rotate][sla]") {
    Instruction instruct;
    instruct.type = InstructionType::SLA;


    SECTION ("SLA 1") {
        instruct.target_8bit = ArithmeticTarget8Bit::A;
        CPU c;

        c.registers.a = 0b00001001;
        c.registers.f.carry = 1;

        c.execute(instruct);

        REQUIRE(c.registers.a == 0b00010010);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == false);
    }

    SECTION ("SLA 2") {
        instruct.target_8bit = ArithmeticTarget8Bit::A;
        CPU c;

        c.registers.a = 0b10101010;
        c.registers.f.carry = 0;

        c.execute(instruct);

        REQUIRE(c.registers.a == 0b01010100);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == true);
        REQUIRE(c.registers.f.half_carry == false);
    }

    SECTION ("SLA 3") {
        instruct.target_8bit = ArithmeticTarget8Bit::B;
        CPU c;

        c.registers.b = 0b10101010;
        c.registers.f.carry = 0;

        c.execute(instruct);

        REQUIRE(c.registers.b == 0b01010100);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == true);
        REQUIRE(c.registers.f.half_carry == false);
    }

    SECTION ("SLA 4") {
        instruct.target_8bit = ArithmeticTarget8Bit::C;
        CPU c;

        c.registers.c = 0b10101010;
        c.registers.f.carry = 0;

        c.execute(instruct);

        REQUIRE(c.registers.c == 0b01010100);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == true);
        REQUIRE(c.registers.f.half_carry == false);
    }

    SECTION ("SLA 5") {
        instruct.target_8bit = ArithmeticTarget8Bit::D;
        CPU c;

        c.registers.d = 0b10101010;
        c.registers.f.carry = 0;

        c.execute(instruct);

        REQUIRE(c.registers.d == 0b01010100);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == true);
        REQUIRE(c.registers.f.half_carry == false);
    }

    SECTION ("SLA 6") {
        instruct.target_8bit = ArithmeticTarget8Bit::E;
        CPU c;

        c.registers.e = 0b10101010;
        c.registers.f.carry = 0;

        c.execute(instruct);

        REQUIRE(c.registers.e == 0b01010100);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == true);
        REQUIRE(c.registers.f.half_carry == false);
    }

    SECTION ("SLA 7") {
        instruct.target_8bit = ArithmeticTarget8Bit::H;
        CPU c;

        c.registers.h = 0b10101010;
        c.registers.f.carry = 0;

        c.execute(instruct);

        REQUIRE(c.registers.h == 0b01010100);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == true);
        REQUIRE(c.registers.f.half_carry == false);
    }

    SECTION ("SLA 8") {
        instruct.target_8bit = ArithmeticTarget8Bit::L;
        CPU c;

        c.registers.l = 0b10101010;
        c.registers.f.carry = 0;

        c.execute(instruct);

        REQUIRE(c.registers.l == 0b01010100);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == true);
        REQUIRE(c.registers.f.half_carry == false);
    }
}

TEST_CASE("SRA Unit Tests", "[cpu][rotate][sra]") {
    Instruction instruct;
    instruct.type = InstructionType::SRA;


    SECTION ("SRA 1") {
        instruct.target_8bit = ArithmeticTarget8Bit::A;
        CPU c;

        c.registers.a = 0b00001001;
        c.registers.f.carry = 0;

        c.execute(instruct);

        REQUIRE(c.registers.a == 0b00000100);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == 1);
        REQUIRE(c.registers.f.half_carry == false);
    }

    SECTION ("SRA 2") {
        instruct.target_8bit = ArithmeticTarget8Bit::A;
        CPU c;

        c.registers.a = 0b10000000;
        c.registers.f.carry = 1;

        c.execute(instruct);

        REQUIRE(c.registers.a == 0b11000000);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == 0);
        REQUIRE(c.registers.f.half_carry == false);
    }

    SECTION ("SRA 3") {
        instruct.target_8bit = ArithmeticTarget8Bit::B;
        CPU c;

        c.registers.b = 0b00001001;
        c.registers.f.carry = 0;

        c.execute(instruct);

        REQUIRE(c.registers.b == 0b00000100);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == 1);
        REQUIRE(c.registers.f.half_carry == false);
    }

    SECTION ("SRA 4") {
        instruct.target_8bit = ArithmeticTarget8Bit::C;
        CPU c;

        c.registers.c = 0b00001001;
        c.registers.f.carry = 0;

        c.execute(instruct);

        REQUIRE(c.registers.c == 0b00000100);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == 1);
        REQUIRE(c.registers.f.half_carry == false);
    }

    SECTION ("SRA 5") {
        instruct.target_8bit = ArithmeticTarget8Bit::D;
        CPU c;

        c.registers.d = 0b00001001;
        c.registers.f.carry = 0;

        c.execute(instruct);

        REQUIRE(c.registers.d == 0b00000100);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == 1);
        REQUIRE(c.registers.f.half_carry == false);
    }

    SECTION ("SRA 6") {
        instruct.target_8bit = ArithmeticTarget8Bit::E;
        CPU c;

        c.registers.e = 0b00001001;
        c.registers.f.carry = 0;

        c.execute(instruct);

        REQUIRE(c.registers.e == 0b00000100);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == 1);
        REQUIRE(c.registers.f.half_carry == false);
    }

    SECTION ("SRA 7") {
        instruct.target_8bit = ArithmeticTarget8Bit::H;
        CPU c;

        c.registers.h = 0b00001001;
        c.registers.f.carry = 0;

        c.execute(instruct);

        REQUIRE(c.registers.h == 0b00000100);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == 1);
        REQUIRE(c.registers.f.half_carry == false);
    }

    SECTION ("SRA 8") {
        instruct.target_8bit = ArithmeticTarget8Bit::L;
        CPU c;

        c.registers.l = 0b00001001;
        c.registers.f.carry = 0;

        c.execute(instruct);

        REQUIRE(c.registers.l == 0b00000100);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == 1);
        REQUIRE(c.registers.f.half_carry == false);
    }
}

TEST_CASE("SRL Unit Tests", "[cpu][rotate][srl]") {
    Instruction instruct;
    instruct.type = InstructionType::SRL;


    SECTION ("SRL 1") {
        instruct.target_8bit = ArithmeticTarget8Bit::A;
        CPU c;

        c.registers.a = 0b00001001;
        c.registers.f.carry = 0;

        c.execute(instruct);

        REQUIRE(c.registers.a == 0b00000100);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == 1);
        REQUIRE(c.registers.f.half_carry == false);
    }

    SECTION ("SRL 2") {
        instruct.target_8bit = ArithmeticTarget8Bit::A;
        CPU c;

        c.registers.a = 0b10000000;
        c.registers.f.carry = 1;

        c.execute(instruct);

        REQUIRE(c.registers.a == 0b01000000);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == 0);
        REQUIRE(c.registers.f.half_carry == false);
    }

    SECTION ("SRL 3") {
        instruct.target_8bit = ArithmeticTarget8Bit::B;
        CPU c;

        c.registers.b = 0b10000000;
        c.registers.f.carry = 1;

        c.execute(instruct);

        REQUIRE(c.registers.b == 0b01000000);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == 0);
        REQUIRE(c.registers.f.half_carry == false);
    }

    SECTION ("SRL 4") {
        instruct.target_8bit = ArithmeticTarget8Bit::C;
        CPU c;

        c.registers.c = 0b10000000;
        c.registers.f.carry = 1;

        c.execute(instruct);

        REQUIRE(c.registers.c == 0b01000000);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == 0);
        REQUIRE(c.registers.f.half_carry == false);
    }

    SECTION ("SRL 5") {
        instruct.target_8bit = ArithmeticTarget8Bit::D;
        CPU c;

        c.registers.d = 0b10000000;
        c.registers.f.carry = 1;

        c.execute(instruct);

        REQUIRE(c.registers.d == 0b01000000);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == 0);
        REQUIRE
        
        (c.registers.f.half_carry == false);
    }

    SECTION ("SRL 6") {
        instruct.target_8bit = ArithmeticTarget8Bit::E;
        CPU c;

        c.registers.e = 0b10000000;
        c.registers.f.carry = 1;

        c.execute(instruct);

        REQUIRE(c.registers.e == 0b01000000);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == 0);
        REQUIRE(c.registers.f.half_carry == false);
    }

    SECTION ("SRL 7") {
        instruct.target_8bit = ArithmeticTarget8Bit::H;
        CPU c;

        c.registers.h = 0b10000000;
        c.registers.f.carry = 1;

        c.execute(instruct);

        REQUIRE(c.registers.h == 0b01000000);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == 0);
        REQUIRE(c.registers.f.half_carry == false);
    }

    SECTION ("SRL 8") {
        instruct.target_8bit = ArithmeticTarget8Bit::L;
        CPU c;

        c.registers.l = 0b10000000;
        c.registers.f.carry = 1;

        c.execute(instruct);

        REQUIRE(c.registers.l == 0b01000000);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == 0);
        REQUIRE(c.registers.f.half_carry == false);
    }
}

TEST_CASE("Instruction from_byte() Unit Tests", "[instruction][from_byte]") {
    SECTION ("ADD A") {
        std::optional<Instruction> opt_instruct = from_byte(0x87, false);
        Instruction instruct = opt_instruct.value(); 

        REQUIRE(instruct.type == InstructionType::ADD);
        REQUIRE(instruct.target_8bit == ArithmeticTarget8Bit::A);
    }

    SECTION ("SWAP A") {
        std::optional<Instruction> opt_instruct = from_byte(0x37, true);
        Instruction instruct = opt_instruct.value(); 

        REQUIRE(instruct.type == InstructionType::SWAP);
        REQUIRE(instruct.target_8bit == ArithmeticTarget8Bit::A);
    }

    SECTION ("from_byte_prefixed SWAP HL_PTR") {
        std::optional<Instruction> opt_instruct = from_byte_prefixed(0x36);
        Instruction instruct = opt_instruct.value();

        REQUIRE(instruct.type == InstructionType::SWAP);
        REQUIRE(instruct.target_8bit == ArithmeticTarget8Bit::HL_PTR);
    }

    SECTION ("from_byte_not_prefixed JP Always") {
        std::optional<Instruction> opt_instruct = from_byte_not_prefixed(0xC3);
        Instruction instruct = opt_instruct.value();

        REQUIRE(instruct.type == InstructionType::JP);
        REQUIRE(instruct.jump_test == JumpTest::Always);
    }
}

TEST_CASE("JP Unit Tests", "[cpu][jump][jp]") {
    Instruction instruct;
    instruct.type = InstructionType::JP;

    SECTION("JP Always jumps to absolute address") {
        CPU c;
        uint16_t target = 0xC000;
        c.bus.write_byte(c.pc + 1, (uint8_t)(target & 0xFF));
        c.bus.write_byte(c.pc + 2, (uint8_t)(target >> 8));

        instruct.jump_test = JumpTest::Always;

        c.execute(instruct);

        REQUIRE(c.pc == target);
    }

    SECTION("JP NotZero taken / not taken") {
        // Taken when Z flag is false
        {
            CPU c;
            uint16_t target = 0xC100;
            c.bus.write_byte(c.pc + 1, (uint8_t)(target & 0xFF));
            c.bus.write_byte(c.pc + 2, (uint8_t)(target >> 8));
            c.registers.f.zero = false;

            instruct.jump_test = JumpTest::NotZero;

            c.execute(instruct);

            REQUIRE(c.pc == target);
        }

        // Not taken when Z flag is true
        {
            CPU c;
            uint16_t target = 0xC200;
            c.bus.write_byte(c.pc + 1, (uint8_t)(target & 0xFF));
            c.bus.write_byte(c.pc + 2, (uint8_t)(target >> 8));
            c.registers.f.zero = true;

            instruct.jump_test = JumpTest::NotZero;
            uint16_t start_pc = c.pc;

            c.execute(instruct);

            REQUIRE(c.pc == (uint16_t)(start_pc + 3));
        }
    }
}

TEST_CASE("JR Unit Tests", "[cpu][jump][jr]") {
    Instruction instruct;
    instruct.type = InstructionType::JR;

    SECTION("JR Always forward offset") {
        CPU c;
        int8_t offset = 5;
        c.bus.write_byte(c.pc + 1, (uint8_t)offset);

        instruct.jump_test = JumpTest::Always;
        uint16_t start_pc = c.pc;

        c.execute(instruct);

        REQUIRE(c.pc == (uint16_t)(start_pc + 2 + offset));
    }

    SECTION("JR Always backward offset") {
        CPU c;
        int8_t offset = -2;
        c.bus.write_byte(c.pc + 1, (uint8_t)offset);

        instruct.jump_test = JumpTest::Always;
        uint16_t start_pc = c.pc;

        c.execute(instruct);

        REQUIRE(c.pc == (uint16_t)(start_pc + 2 + offset));
    }

    SECTION("JR NotZero not taken") {
        CPU c;
        int8_t offset = 3;
        c.bus.write_byte(c.pc + 1, (uint8_t)offset);
        c.registers.f.zero = true;

        instruct.jump_test = JumpTest::NotZero;
        uint16_t start_pc = c.pc;

        c.execute(instruct);

        REQUIRE(c.pc == (uint16_t)(start_pc + 2));
    }
}

TEST_CASE("JP_HL Unit Tests", "[cpu][jump][jp_hl]") {
    Instruction instruct;
    instruct.type = InstructionType::JP_HL;

    SECTION("JP_HL sets pc to HL") {
        CPU c;
        uint16_t target = 0xC123;
        c.registers.set_hl(target);

        c.execute(instruct);

        REQUIRE(c.pc == target);
    }
}