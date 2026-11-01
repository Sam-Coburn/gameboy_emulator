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
    instruct.alu8.target = Operand8::B;

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
    instruct.alu8.target = Operand8::B;

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
    instruct.alu8.target = Operand8::B;

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
    instruct.alu8.target = Operand8::B;

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
    instruct.alu8.target = Operand8::B;

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
    instruct.alu8.target = Operand8::B;

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
    instruct.alu8.target = Operand8::B;

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
    instruct.alu8.target = Operand8::B;

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
        instruct.alu8.target = Operand8::A;

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
        instruct.alu8.target = Operand8::B;

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
        instruct.alu8.target = Operand8::C;

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
        instruct.alu8.target = Operand8::D;

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
        instruct.alu8.target = Operand8::E;

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
        instruct.alu8.target = Operand8::H;

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
        instruct.alu8.target = Operand8::L;

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
        instruct.alu8.target = Operand8::B;

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
        instruct.alu8.target = Operand8::B;

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
        instruct.alu8.target = Operand8::B;

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
        instruct.alu8.target = Operand8::A;

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
        instruct.alu8.target = Operand8::B;

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
        instruct.alu8.target = Operand8::C;

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
        instruct.alu8.target = Operand8::D;

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
        instruct.alu8.target = Operand8::E;

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
        instruct.alu8.target = Operand8::H;

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
        instruct.alu8.target = Operand8::L;

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
        instruct.alu8.target = Operand8::B;

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
        instruct.alu8.target = Operand8::B;

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

        instruct.alu8.target = Operand8::HL_PTR;

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
        instruct.alu8.target = Operand8::NUM;
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

        instruct.alu8.target = Operand8::HL_PTR;

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
        instruct.alu8.target = Operand8::NUM;
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

        instruct.alu8.target = Operand8::HL_PTR;

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
        instruct.alu8.target = Operand8::NUM;
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

        instruct.alu8.target = Operand8::HL_PTR;

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
        instruct.alu8.target = Operand8::NUM;
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

        instruct.alu8.target = Operand8::HL_PTR;

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
        instruct.alu8.target = Operand8::NUM;
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

        instruct.alu8.target = Operand8::HL_PTR;

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
        instruct.alu8.target = Operand8::NUM;
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

        instruct.alu8.target = Operand8::HL_PTR;

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
        instruct.alu8.target = Operand8::NUM;
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

        instruct.alu8.target = Operand8::HL_PTR;

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
        instruct.alu8.target = Operand8::NUM;
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
    instruct.alu8.target = Operand8::HL_PTR;

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
    instruct.alu8.target = Operand8::HL_PTR;
    instruct.bitop.bit_index = 3;

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
    instruct.bitop.bit_index = 3;

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
    instruct.bitop.bit_index = 3;

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
    instruct.alu8.target = Operand8::HL_PTR;
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
    instruct.alu8.target = Operand8::HL_PTR;
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
        instruct.alu8.target = Operand8::A;

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
        instruct.alu8.target = Operand8::B;

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
        instruct.alu8.target = Operand8::C;

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
        instruct.alu8.target = Operand8::D;

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
        instruct.alu8.target = Operand8::E;

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
        instruct.alu8.target = Operand8::H;

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
        instruct.alu8.target = Operand8::L;

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
        instruct.alu8.target = Operand8::A;
        instruct.bitop.bit_index = 3;

        c.registers.a = 0b00001000;

        c.execute(instruct);

        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == true);
    }

    SECTION ("BIT 2") {
        CPU c;
        instruct.alu8.target = Operand8::A;
        instruct.bitop.bit_index = 2;

        c.registers.a = 0b00001000;

        c.execute(instruct);

        REQUIRE(c.registers.f.zero == true);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == true);
    }

    SECTION ("BIT 3") {
        CPU c;
        instruct.alu8.target = Operand8::A;
        instruct.bitop.bit_index = 4;

        c.registers.a = 0b00001000;

        c.execute(instruct);

        REQUIRE(c.registers.f.zero == true);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == true);
    }

    SECTION ("BIT 4") {
        CPU c;
        instruct.alu8.target = Operand8::B;
        instruct.bitop.bit_index = 3;

        c.registers.b = 0b00001000;

        c.execute(instruct);

        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == true);
    }

    SECTION ("BIT 5") {
        CPU c;
        instruct.alu8.target = Operand8::C;
        instruct.bitop.bit_index = 3;

        c.registers.c = 0b00001000;

        c.execute(instruct);

        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == true);
    }

    SECTION ("BIT 6") {
        CPU c;
        instruct.alu8.target = Operand8::D;
        instruct.bitop.bit_index = 3;

        c.registers.d = 0b00001000;

        c.execute(instruct);

        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == true);
    }

    SECTION ("BIT 7") {
        CPU c;
        instruct.alu8.target = Operand8::E;
        instruct.bitop.bit_index = 3;

        c.registers.e = 0b00001000;

        c.execute(instruct);

        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == true);
    }

    SECTION ("BIT 8") {
        CPU c;
        instruct.alu8.target = Operand8::H;
        instruct.bitop.bit_index = 3;

        c.registers.h = 0b00001000;

        c.execute(instruct);

        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == true);
    }

    SECTION ("BIT 9") {
        CPU c;
        instruct.alu8.target = Operand8::L;
        instruct.bitop.bit_index = 3;

        c.registers.l = 0b00001000;

        c.execute(instruct);

        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == true);
    }

    SECTION ("BIT 10 - Carry set to True") {
        CPU c;
        instruct.alu8.target = Operand8::A;
        instruct.bitop.bit_index = 3;

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
        instruct.alu8.target = Operand8::A;
        instruct.bitop.bit_index = 3;

        c.registers.a = 0;

        c.execute(instruct);

        REQUIRE(c.registers.a == 0b00001000);
    }

    SECTION ("SET 2") {
        CPU c;
        instruct.alu8.target = Operand8::A;
        instruct.bitop.bit_index = 7;

        c.registers.a = 0;

        c.execute(instruct);

        REQUIRE(c.registers.a == 0b10000000);
    }

    SECTION ("SET 3") {
        CPU c;
        instruct.alu8.target = Operand8::A;
        instruct.bitop.bit_index = 0;

        c.registers.a = 0;

        c.execute(instruct);

        REQUIRE(c.registers.a == 0b00000001);
    }

    SECTION ("SET 4") {
        CPU c;
        instruct.alu8.target = Operand8::B;
        instruct.bitop.bit_index = 3;

        c.registers.b = 0;

        c.execute(instruct);

        REQUIRE(c.registers.b == 0b00001000);
    }

    SECTION ("SET 5") {
        CPU c;
        instruct.alu8.target = Operand8::C;
        instruct.bitop.bit_index = 3;

        c.registers.c = 0;

        c.execute(instruct);

        REQUIRE(c.registers.c == 0b00001000);
    }

    SECTION ("SET 6") {
        CPU c;
        instruct.alu8.target = Operand8::D;
        instruct.bitop.bit_index = 3;

        c.registers.d = 0;

        c.execute(instruct);

        REQUIRE(c.registers.d == 0b00001000);
    }

    SECTION ("SET 7") {
        CPU c;
        instruct.alu8.target = Operand8::E;
        instruct.bitop.bit_index = 3;

        c.registers.e = 0;

        c.execute(instruct);

        REQUIRE(c.registers.e == 0b00001000);
    }

    SECTION ("SET 8") {
        CPU c;
        instruct.alu8.target = Operand8::H;
        instruct.bitop.bit_index = 3;

        c.registers.h = 0;

        c.execute(instruct);

        REQUIRE(c.registers.h == 0b00001000);
    }

    SECTION ("SET 9") {
        CPU c;
        instruct.alu8.target = Operand8::L;
        instruct.bitop.bit_index = 3;

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
        instruct.alu8.target = Operand8::A;
        instruct.bitop.bit_index = 3;

        c.registers.a = 255;

        c.execute(instruct);

        REQUIRE(c.registers.a == 0b11110111);
    }

    SECTION ("RES 2") {
        CPU c;
        instruct.alu8.target = Operand8::A;
        instruct.bitop.bit_index = 7;

        c.registers.a = 255;

        c.execute(instruct);

        REQUIRE(c.registers.a == 0b01111111);
    }

    SECTION ("RES 3") {
        CPU c;
        instruct.alu8.target = Operand8::A;
        instruct.bitop.bit_index = 0;

        c.registers.a = 255;

        c.execute(instruct);

        REQUIRE(c.registers.a == 0b11111110);
    }

    SECTION ("RES 4") {
        CPU c;
        instruct.alu8.target = Operand8::B;
        instruct.bitop.bit_index = 0;

        c.registers.b = 255;

        c.execute(instruct);

        REQUIRE(c.registers.b == 0b11111110);
    }

    SECTION ("RES 5") {
        CPU c;
        instruct.alu8.target = Operand8::C;
        instruct.bitop.bit_index = 0;

        c.registers.c = 255;

        c.execute(instruct);

        REQUIRE(c.registers.c == 0b11111110);
    }

    SECTION ("RES 6") {
        CPU c;
        instruct.alu8.target = Operand8::D;
        instruct.bitop.bit_index = 0;

        c.registers.d = 255;

        c.execute(instruct);

        REQUIRE(c.registers.d == 0b11111110);
    }

    SECTION ("RES 7") {
        CPU c;
        instruct.alu8.target = Operand8::E;
        instruct.bitop.bit_index = 0;

        c.registers.e = 255;

        c.execute(instruct);

        REQUIRE(c.registers.e == 0b11111110);
    }

    SECTION ("RES 8") {
        CPU c;
        instruct.alu8.target = Operand8::H;
        instruct.bitop.bit_index = 0;

        c.registers.h = 255;

        c.execute(instruct);

        REQUIRE(c.registers.h == 0b11111110);
    }

    SECTION ("RES 9") {
        CPU c;
        instruct.alu8.target = Operand8::L;
        instruct.bitop.bit_index = 0;

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
        instruct.alu16.target = Operand16::BC;

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
        instruct.alu16.target = Operand16::BC;

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
        instruct.alu16.target = Operand16::BC;

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
        instruct.alu16.target = Operand16::DE;

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
        instruct.alu16.target = Operand16::HL;

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
        instruct.alu16.target = Operand16::SP;

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
        instruct.alu8.target = Operand8::A;
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
        instruct.alu8.target = Operand8::A;
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
        instruct.alu8.target = Operand8::B;
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
        instruct.alu8.target = Operand8::C;
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
        instruct.alu8.target = Operand8::D;
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
        instruct.alu8.target = Operand8::E;
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
        instruct.alu8.target = Operand8::H;
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
        instruct.alu8.target = Operand8::L;
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
        instruct.alu8.target = Operand8::A;
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
        instruct.alu8.target = Operand8::B;
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
        instruct.alu8.target = Operand8::C;
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
        instruct.alu8.target = Operand8::D;
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
        instruct.alu8.target = Operand8::E;
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
        instruct.alu8.target = Operand8::H;
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
        instruct.alu8.target = Operand8::L;
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
        instruct.alu8.target = Operand8::A;
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
        instruct.alu8.target = Operand8::B;
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
        instruct.alu8.target = Operand8::C;
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
        instruct.alu8.target = Operand8::D;
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
        instruct.alu8.target = Operand8::E;
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
        instruct.alu8.target = Operand8::H;
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
        instruct.alu8.target = Operand8::L;
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
        instruct.alu8.target = Operand8::A;
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
        instruct.alu8.target = Operand8::B;
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
        instruct.alu8.target = Operand8::C;
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
        instruct.alu8.target = Operand8::D;
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
        instruct.alu8.target = Operand8::E;
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
        instruct.alu8.target = Operand8::H;
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
        instruct.alu8.target = Operand8::L;
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
        instruct.alu8.target = Operand8::A;
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
        instruct.alu8.target = Operand8::A;
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
        instruct.alu8.target = Operand8::B;
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
        instruct.alu8.target = Operand8::C;
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
        instruct.alu8.target = Operand8::D;
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
        instruct.alu8.target = Operand8::E;
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
        instruct.alu8.target = Operand8::H;
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
        instruct.alu8.target = Operand8::L;
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
        instruct.alu8.target = Operand8::A;
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
        instruct.alu8.target = Operand8::A;
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
        instruct.alu8.target = Operand8::B;
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
        instruct.alu8.target = Operand8::C;
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
        instruct.alu8.target = Operand8::D;
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
        instruct.alu8.target = Operand8::E;
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
        instruct.alu8.target = Operand8::H;
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
        instruct.alu8.target = Operand8::L;
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
        instruct.alu8.target = Operand8::A;
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
        instruct.alu8.target = Operand8::A;
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
        instruct.alu8.target = Operand8::B;
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
        instruct.alu8.target = Operand8::C;
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
        instruct.alu8.target = Operand8::D;
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
        instruct.alu8.target = Operand8::E;
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
        instruct.alu8.target = Operand8::H;
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
        instruct.alu8.target = Operand8::L;
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

TEST_CASE("JP Unit Tests", "[cpu][jump][jp]") {
    Instruction instruct;
    instruct.type = InstructionType::JP;

    SECTION("JP Always jumps to absolute address") {
        CPU c;
        uint16_t target = 0xC000;
        c.bus.write_byte(c.pc + 1, (uint8_t)(target & 0xFF));
        c.bus.write_byte(c.pc + 2, (uint8_t)(target >> 8));

        instruct.jump.condition = JumpTest::Always;

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

            instruct.jump.condition = JumpTest::NotZero;

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

            instruct.jump.condition = JumpTest::NotZero;
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

        instruct.jump.condition = JumpTest::Always;
        uint16_t start_pc = c.pc;

        c.execute(instruct);

        REQUIRE(c.pc == (uint16_t)(start_pc + 2 + offset));
    }

    SECTION("JR Always backward offset") {
        CPU c;
        int8_t offset = -2;
        c.bus.write_byte(c.pc + 1, (uint8_t)offset);

        instruct.jump.condition = JumpTest::Always;
        uint16_t start_pc = c.pc;

        c.execute(instruct);

        REQUIRE(c.pc == (uint16_t)(start_pc + 2 + offset));
    }

    SECTION("JR NotZero not taken") {
        CPU c;
        int8_t offset = 3;
        c.bus.write_byte(c.pc + 1, (uint8_t)offset);
        c.registers.f.zero = true;

        instruct.jump.condition = JumpTest::NotZero;
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

TEST_CASE("LD_8 Unit Tests", "[cpu][load][ld_8]") {
    Instruction instruct;
    instruct.type = InstructionType::LD_8;

    SECTION("LD_8 B into A") {
        CPU c;
        c.registers.b = 0x42;
        instruct.load8.source = LoadSource::B;
        instruct.load8.target = LoadTarget::A;
        uint16_t start_pc = c.pc;

        c.execute(instruct);

        REQUIRE(c.registers.a == 0x42);
        REQUIRE(c.pc == start_pc + 1);
    }

    SECTION("LD_8 HL_PTR into C") {
        CPU c;
        c.registers.set_hl(0xC000);
        c.bus.write_byte(0xC000, 0x99);
        instruct.load8.source = LoadSource::HL_PTR;
        instruct.load8.target = LoadTarget::C;
        uint16_t start_pc = c.pc;

        c.execute(instruct);

        REQUIRE(c.registers.c == 0x99);
        REQUIRE(c.pc == start_pc + 1);
    }

    SECTION("LD_8 E into HL_PTR") {
        CPU c;
        c.registers.e = 0x7A;
        c.registers.set_hl(0xC010);
        instruct.load8.source = LoadSource::E;
        instruct.load8.target = LoadTarget::HL_PTR;
        uint16_t start_pc = c.pc;

        c.execute(instruct);

        REQUIRE(c.bus.read_byte(0xC010) == 0x7A);
        REQUIRE(c.pc == start_pc + 1);
    }
}

TEST_CASE("LD_N Unit Tests", "[cpu][load][ld_n]") {
    Instruction instruct;
    instruct.type = InstructionType::LD_N;
    instruct.load8.source = LoadSource::NUM;

    SECTION("LD_N into A") {
        CPU c;
        c.bus.write_byte(c.pc + 1, 0x5C);
        instruct.load8.target = LoadTarget::A;
        uint16_t start_pc = c.pc;

        c.execute(instruct);

        REQUIRE(c.registers.a == 0x5C);
        REQUIRE(c.pc == start_pc + 2);
    }

    SECTION("LD_N into L") {
        CPU c;
        c.bus.write_byte(c.pc + 1, 0x11);
        instruct.load8.target = LoadTarget::L;
        uint16_t start_pc = c.pc;

        c.execute(instruct);

        REQUIRE(c.registers.l == 0x11);
        REQUIRE(c.pc == start_pc + 2);
    }

    SECTION("LD_N into HL_PTR") {
        CPU c;
        c.registers.set_hl(0xC020);
        c.bus.write_byte(c.pc + 1, 0xAB);
        instruct.load8.target = LoadTarget::HL_PTR;
        uint16_t start_pc = c.pc;

        c.execute(instruct);

        REQUIRE(c.bus.read_byte(0xC020) == 0xAB);
        REQUIRE(c.pc == start_pc + 2);
    }
}

TEST_CASE("LD_16 Unit Tests", "[cpu][load][ld_16]") {
    Instruction instruct;
    instruct.type = InstructionType::LD_16;

    SECTION("LD_16 into BC is little-endian") {
        CPU c;
        c.bus.write_byte(c.pc + 1, 0x34);
        c.bus.write_byte(c.pc + 2, 0x12);
        instruct.load16.target = Operand16::BC;
        uint16_t start_pc = c.pc;

        c.execute(instruct);

        REQUIRE(c.registers.get_bc() == 0x1234);
        REQUIRE(c.registers.b == 0x12);
        REQUIRE(c.registers.c == 0x34);
        REQUIRE(c.pc == start_pc + 3);
    }

    SECTION("LD_16 into HL") {
        CPU c;
        c.bus.write_byte(c.pc + 1, 0xEF);
        c.bus.write_byte(c.pc + 2, 0xBE);
        instruct.load16.target = Operand16::HL;
        uint16_t start_pc = c.pc;

        c.execute(instruct);

        REQUIRE(c.registers.get_hl() == 0xBEEF);
        REQUIRE(c.pc == start_pc + 3);
    }

    SECTION("LD_16 into SP") {
        CPU c;
        c.bus.write_byte(c.pc + 1, 0xFE);
        c.bus.write_byte(c.pc + 2, 0xDF);
        instruct.load16.target = Operand16::SP;
        uint16_t start_pc = c.pc;

        c.execute(instruct);

        REQUIRE(c.sp == 0xDFFE);
        REQUIRE(c.pc == start_pc + 3);
    }
}

TEST_CASE("LD_8 r,A Unit Tests", "[cpu][load][ld_8]") {
    Instruction instruct;
    instruct.type = InstructionType::LD_8;
    instruct.load8.source = LoadSource::A;

    SECTION("LD_8 A into H") {
        CPU c;
        c.registers.a = 0x3C;
        instruct.load8.target = LoadTarget::H;
        uint16_t start_pc = c.pc;

        c.execute(instruct);

        REQUIRE(c.registers.h == 0x3C);
        REQUIRE(c.pc == start_pc + 1);
    }

    SECTION("LD_8 A into HL_PTR") {
        CPU c;
        c.registers.a = 0x5D;
        c.registers.set_hl(0xC030);
        instruct.load8.target = LoadTarget::HL_PTR;

        c.execute(instruct);

        REQUIRE(c.bus.read_byte(0xC030) == 0x5D);
    }
}

TEST_CASE("LD_A_IND Unit Tests", "[cpu][load][ld_a_ind]") {
    Instruction instruct;
    instruct.type = InstructionType::LD_A_IND;

    SECTION("LD A,(BC)") {
        CPU c;
        c.registers.set_bc(0xC100);
        c.bus.write_byte(0xC100, 0x21);
        instruct.load_ind.address = IndirectAddress::BC;
        uint16_t start_pc = c.pc;

        c.execute(instruct);

        REQUIRE(c.registers.a == 0x21);
        REQUIRE(c.pc == start_pc + 1);
    }

    SECTION("LD A,(DE)") {
        CPU c;
        c.registers.set_de(0xC200);
        c.bus.write_byte(0xC200, 0x43);
        instruct.load_ind.address = IndirectAddress::DE;
        uint16_t start_pc = c.pc;

        c.execute(instruct);

        REQUIRE(c.registers.a == 0x43);
        REQUIRE(c.pc == start_pc + 1);
    }

    SECTION("LD A,(nn)") {
        CPU c;
        c.bus.write_byte(c.pc + 1, 0x34);
        c.bus.write_byte(c.pc + 2, 0xC3);
        c.bus.write_byte(0xC334, 0x65);
        instruct.load_ind.address = IndirectAddress::NN;
        uint16_t start_pc = c.pc;

        c.execute(instruct);

        REQUIRE(c.registers.a == 0x65);
        REQUIRE(c.pc == start_pc + 3);
    }

    SECTION("LD A,(FF00+C)") {
        CPU c;
        c.registers.c = 0x80;
        c.bus.write_byte(0xFF80, 0x87);
        instruct.load_ind.address = IndirectAddress::FF00_C;
        uint16_t start_pc = c.pc;

        c.execute(instruct);

        REQUIRE(c.registers.a == 0x87);
        REQUIRE(c.pc == start_pc + 1);
    }

    SECTION("LDH A,(FF00+n)") {
        CPU c;
        c.bus.write_byte(c.pc + 1, 0x44);
        c.bus.write_byte(0xFF44, 0x90);
        instruct.load_ind.address = IndirectAddress::FF00_N;
        uint16_t start_pc = c.pc;

        c.execute(instruct);

        REQUIRE(c.registers.a == 0x90);
        REQUIRE(c.pc == start_pc + 2);
    }

    SECTION("LD A,(HL+)") {
        CPU c;
        c.registers.set_hl(0xC400);
        c.bus.write_byte(0xC400, 0xA1);
        instruct.load_ind.address = IndirectAddress::HL_INC;
        uint16_t start_pc = c.pc;

        c.execute(instruct);

        REQUIRE(c.registers.a == 0xA1);
        REQUIRE(c.registers.get_hl() == 0xC401);
        REQUIRE(c.pc == start_pc + 1);
    }

    SECTION("LD A,(HL-)") {
        CPU c;
        c.registers.set_hl(0xC500);
        c.bus.write_byte(0xC500, 0xB2);
        instruct.load_ind.address = IndirectAddress::HL_DEC;
        uint16_t start_pc = c.pc;

        c.execute(instruct);

        REQUIRE(c.registers.a == 0xB2);
        REQUIRE(c.registers.get_hl() == 0xC4FF);
        REQUIRE(c.pc == start_pc + 1);
    }
}

TEST_CASE("LD_IND_A Unit Tests", "[cpu][load][ld_ind_a]") {
    Instruction instruct;
    instruct.type = InstructionType::LD_IND_A;

    SECTION("LD (BC),A") {
        CPU c;
        c.registers.a = 0x11;
        c.registers.set_bc(0xC100);
        instruct.load_ind.address = IndirectAddress::BC;
        uint16_t start_pc = c.pc;

        c.execute(instruct);

        REQUIRE(c.bus.read_byte(0xC100) == 0x11);
        REQUIRE(c.pc == start_pc + 1);
    }

    SECTION("LD (DE),A") {
        CPU c;
        c.registers.a = 0x22;
        c.registers.set_de(0xC200);
        instruct.load_ind.address = IndirectAddress::DE;
        uint16_t start_pc = c.pc;

        c.execute(instruct);

        REQUIRE(c.bus.read_byte(0xC200) == 0x22);
        REQUIRE(c.pc == start_pc + 1);
    }

    SECTION("LD (nn),A") {
        CPU c;
        c.registers.a = 0x33;
        c.bus.write_byte(c.pc + 1, 0x00);
        c.bus.write_byte(c.pc + 2, 0xD0);
        instruct.load_ind.address = IndirectAddress::NN;
        uint16_t start_pc = c.pc;

        c.execute(instruct);

        REQUIRE(c.bus.read_byte(0xD000) == 0x33);
        REQUIRE(c.pc == start_pc + 3);
    }

    SECTION("LD (FF00+C),A") {
        CPU c;
        c.registers.a = 0x44;
        c.registers.c = 0x81;
        instruct.load_ind.address = IndirectAddress::FF00_C;
        uint16_t start_pc = c.pc;

        c.execute(instruct);

        REQUIRE(c.bus.read_byte(0xFF81) == 0x44);
        REQUIRE(c.pc == start_pc + 1);
    }

    SECTION("LDH (FF00+n),A") {
        CPU c;
        c.registers.a = 0x55;
        c.bus.write_byte(c.pc + 1, 0x82);
        instruct.load_ind.address = IndirectAddress::FF00_N;
        uint16_t start_pc = c.pc;

        c.execute(instruct);

        REQUIRE(c.bus.read_byte(0xFF82) == 0x55);
        REQUIRE(c.pc == start_pc + 2);
    }

    SECTION("LD (HL+),A") {
        CPU c;
        c.registers.a = 0x66;
        c.registers.set_hl(0xC0FF);
        instruct.load_ind.address = IndirectAddress::HL_INC;
        uint16_t start_pc = c.pc;

        c.execute(instruct);

        REQUIRE(c.bus.read_byte(0xC0FF) == 0x66);
        REQUIRE(c.registers.get_hl() == 0xC100);
        REQUIRE(c.pc == start_pc + 1);
    }

    SECTION("LD (HL-),A") {
        CPU c;
        c.registers.a = 0x77;
        c.registers.set_hl(0xC600);
        instruct.load_ind.address = IndirectAddress::HL_DEC;
        uint16_t start_pc = c.pc;

        c.execute(instruct);

        REQUIRE(c.bus.read_byte(0xC600) == 0x77);
        REQUIRE(c.registers.get_hl() == 0xC5FF);
        REQUIRE(c.pc == start_pc + 1);
    }
}

TEST_CASE("LD_SP_HL Unit Tests", "[cpu][load][ld_sp_hl]") {
    Instruction instruct;
    instruct.type = InstructionType::LD_SP_HL;

    SECTION("LD SP,HL") {
        CPU c;
        c.registers.set_hl(0xD123);
        uint16_t start_pc = c.pc;

        c.execute(instruct);

        REQUIRE(c.sp == 0xD123);
        REQUIRE(c.pc == start_pc + 1);
    }
}

TEST_CASE("LD_HL_SP Unit Tests", "[cpu][load][ld_hl_sp]") {
    Instruction instruct;
    instruct.type = InstructionType::LD_HL_SP;

    SECTION("LD HL,SP+e positive offset w/o carries") {
        CPU c;
        c.sp = 0xC000;
        c.bus.write_byte(c.pc + 1, 0x05);
        c.registers.f.zero = true;
        c.registers.f.subtract = true;
        uint16_t start_pc = c.pc;

        c.execute(instruct);

        REQUIRE(c.registers.get_hl() == 0xC005);
        REQUIRE(c.sp == 0xC000);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.half_carry == false);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.pc == start_pc + 2);
    }

    SECTION("LD HL,SP+e w/ half carry and carry") {
        CPU c;
        c.sp = 0xC0FF;
        c.bus.write_byte(c.pc + 1, 0x01);

        c.execute(instruct);

        REQUIRE(c.registers.get_hl() == 0xC100);
        REQUIRE(c.registers.f.half_carry == true);
        REQUIRE(c.registers.f.carry == true);
    }

    SECTION("LD HL,SP+e negative offset") {
        CPU c;
        c.sp = 0xC010;
        int8_t offset = -2;
        c.bus.write_byte(c.pc + 1, (uint8_t)offset);

        c.execute(instruct);

        // Flags come from unsigned 0x10 + 0xFE on the low byte
        REQUIRE(c.registers.get_hl() == 0xC00E);
        REQUIRE(c.registers.f.half_carry == false);
        REQUIRE(c.registers.f.carry == true);
    }
}

TEST_CASE("LD_NN_SP Unit Tests", "[cpu][load][ld_nn_sp]") {
    Instruction instruct;
    instruct.type = InstructionType::LD_NN_SP;

    SECTION("LD (nn),SP stores SP little-endian") {
        CPU c;
        c.sp = 0xBEEF;
        c.bus.write_byte(c.pc + 1, 0x00);
        c.bus.write_byte(c.pc + 2, 0xC8);
        uint16_t start_pc = c.pc;

        c.execute(instruct);

        REQUIRE(c.bus.read_byte(0xC800) == 0xEF);
        REQUIRE(c.bus.read_byte(0xC801) == 0xBE);
        REQUIRE(c.pc == start_pc + 3);
    }
}

TEST_CASE("PUSH Unit Tests", "[cpu][stack][push]") {
    Instruction instruct;
    instruct.type = InstructionType::PUSH;

    SECTION("PUSH BC") {
        CPU c;
        c.registers.set_bc(0x1234);
        uint16_t start_sp = c.sp;
        uint16_t start_pc = c.pc;
        instruct.stack.target = StackTarget::BC;

        c.execute(instruct);

        REQUIRE(c.sp == start_sp - 2);
        REQUIRE(c.bus.read_byte(start_sp - 1) == 0x12);
        REQUIRE(c.bus.read_byte(start_sp - 2) == 0x34);
        REQUIRE(c.pc == start_pc + 1);
    }

    SECTION("PUSH AF") {
        CPU c;
        c.registers.a = 0xAB;
        c.registers.f.zero = true;
        c.registers.f.carry = true;
        instruct.stack.target = StackTarget::AF;

        c.execute(instruct);

        REQUIRE(c.bus.read_byte(c.sp + 1) == 0xAB);
        REQUIRE(c.bus.read_byte(c.sp) == 0x90);
    }
}

TEST_CASE("POP Unit Tests", "[cpu][stack][pop]") {
    Instruction instruct;
    instruct.type = InstructionType::POP;

    SECTION("POP DE") {
        CPU c;
        c.sp = 0xDFFC;
        c.bus.write_byte(0xDFFC, 0x78);
        c.bus.write_byte(0xDFFD, 0x56);
        uint16_t start_pc = c.pc;
        instruct.stack.target = StackTarget::DE;

        c.execute(instruct);

        REQUIRE(c.registers.get_de() == 0x5678);
        REQUIRE(c.sp == 0xDFFE);
        REQUIRE(c.pc == start_pc + 1);
    }

    SECTION("POP AF masks lower nibble of F") {
        CPU c;
        c.sp = 0xDFFC;
        c.bus.write_byte(0xDFFC, 0xFF);
        c.bus.write_byte(0xDFFD, 0x12);
        instruct.stack.target = StackTarget::AF;

        c.execute(instruct);

        REQUIRE(c.registers.a == 0x12);
        REQUIRE(c.registers.get_af() == 0x12F0);
    }

    SECTION("PUSH then POP round trip") {
        CPU c;
        Instruction push;
        push.type = InstructionType::PUSH;
        push.stack.target = StackTarget::HL;
        instruct.stack.target = StackTarget::BC;
        c.registers.set_hl(0xCAFE);
        uint16_t start_sp = c.sp;

        c.execute(push);
        c.execute(instruct);

        REQUIRE(c.registers.get_bc() == 0xCAFE);
        REQUIRE(c.sp == start_sp);
    }
}

TEST_CASE("CALL Unit Tests", "[cpu][call][call]") {
    Instruction instruct;
    instruct.type = InstructionType::CALL;

    SECTION("CALL Always pushes return address and jumps") {
        CPU c;
        c.bus.write_byte(c.pc + 1, 0x34);
        c.bus.write_byte(c.pc + 2, 0x12);
        instruct.jump.condition = JumpTest::Always;
        uint16_t start_pc = c.pc;
        uint16_t start_sp = c.sp;

        c.execute(instruct);

        REQUIRE(c.pc == 0x1234);
        REQUIRE(c.sp == start_sp - 2);
        REQUIRE(c.pop() == start_pc + 3);
    }

    SECTION("CALL NotZero taken") {
        CPU c;
        c.bus.write_byte(c.pc + 1, 0x00);
        c.bus.write_byte(c.pc + 2, 0x40);
        c.registers.f.zero = false;
        instruct.jump.condition = JumpTest::NotZero;

        c.execute(instruct);

        REQUIRE(c.pc == 0x4000);
    }

    SECTION("CALL Carry not taken") {
        CPU c;
        c.registers.f.carry = false;
        instruct.jump.condition = JumpTest::Carry;
        uint16_t start_pc = c.pc;
        uint16_t start_sp = c.sp;

        c.execute(instruct);

        REQUIRE(c.pc == start_pc + 3);
        REQUIRE(c.sp == start_sp);
    }
}

TEST_CASE("RET Unit Tests", "[cpu][call][ret]") {
    Instruction instruct;
    instruct.type = InstructionType::RET;

    SECTION("RET Always pops return address") {
        CPU c;
        c.push(0x2345);
        uint16_t pushed_sp = c.sp;
        instruct.jump.condition = JumpTest::Always;

        c.execute(instruct);

        REQUIRE(c.pc == 0x2345);
        REQUIRE(c.sp == pushed_sp + 2);
    }

    SECTION("RET Zero not taken") {
        CPU c;
        c.push(0x2345);
        uint16_t pushed_sp = c.sp;
        c.registers.f.zero = false;
        instruct.jump.condition = JumpTest::Zero;
        uint16_t start_pc = c.pc;

        c.execute(instruct);

        REQUIRE(c.pc == start_pc + 1);
        REQUIRE(c.sp == pushed_sp);
    }

    SECTION("CALL then RET returns to instruction after CALL") {
        CPU c;
        Instruction call;
        call.type = InstructionType::CALL;
        call.jump.condition = JumpTest::Always;
        c.bus.write_byte(c.pc + 1, 0x00);
        c.bus.write_byte(c.pc + 2, 0x50);
        uint16_t start_pc = c.pc;
        uint16_t start_sp = c.sp;
        instruct.jump.condition = JumpTest::Always;

        c.execute(call);
        c.execute(instruct);

        REQUIRE(c.pc == start_pc + 3);
        REQUIRE(c.sp == start_sp);
    }
}

TEST_CASE("RETI Unit Tests", "[cpu][call][reti]") {
    Instruction instruct;
    instruct.type = InstructionType::RETI;

    SECTION("RETI pops return address") {
        CPU c;
        c.push(0x0150);
        uint16_t pushed_sp = c.sp;

        c.execute(instruct);

        REQUIRE(c.pc == 0x0150);
        REQUIRE(c.sp == pushed_sp + 2);
    }
}

TEST_CASE("RST Unit Tests", "[cpu][call][rst]") {
    Instruction instruct;
    instruct.type = InstructionType::RST;

    SECTION("RST 0x38 pushes return address and jumps") {
        CPU c;
        instruct.rst.address = 0x38;
        uint16_t start_pc = c.pc;
        uint16_t start_sp = c.sp;

        c.execute(instruct);

        REQUIRE(c.pc == 0x0038);
        REQUIRE(c.sp == start_sp - 2);
        REQUIRE(c.pop() == start_pc + 1);
    }
}

TEST_CASE("INC16 Unit Tests", "[cpu][16-bit][inc16]") {
    Instruction instruct;
    instruct.type = InstructionType::INC16;

    SECTION("INC BC carries into high byte and leaves flags alone") {
        CPU c;
        c.registers.set_bc(0x12FF);
        c.registers.f.zero = true;
        c.registers.f.carry = true;
        instruct.alu16.target = Operand16::BC;
        uint16_t start_pc = c.pc;

        c.execute(instruct);

        REQUIRE(c.registers.get_bc() == 0x1300);
        REQUIRE(c.registers.f.zero == true);
        REQUIRE(c.registers.f.carry == true);
        REQUIRE(c.pc == start_pc + 1);
    }

    SECTION("INC HL wraps around") {
        CPU c;
        c.registers.set_hl(0xFFFF);
        instruct.alu16.target = Operand16::HL;

        c.execute(instruct);

        REQUIRE(c.registers.get_hl() == 0x0000);
        REQUIRE(c.registers.f.zero == false);
    }

    SECTION("INC SP") {
        CPU c;
        c.sp = 0xC000;
        instruct.alu16.target = Operand16::SP;

        c.execute(instruct);

        REQUIRE(c.sp == 0xC001);
    }
}

TEST_CASE("DEC16 Unit Tests", "[cpu][16-bit][dec16]") {
    Instruction instruct;
    instruct.type = InstructionType::DEC16;

    SECTION("DEC DE borrows from high byte") {
        CPU c;
        c.registers.set_de(0x1200);
        instruct.alu16.target = Operand16::DE;
        uint16_t start_pc = c.pc;

        c.execute(instruct);

        REQUIRE(c.registers.get_de() == 0x11FF);
        REQUIRE(c.pc == start_pc + 1);
    }

    SECTION("DEC BC wraps around and leaves flags alone") {
        CPU c;
        c.registers.set_bc(0x0000);
        instruct.alu16.target = Operand16::BC;

        c.execute(instruct);

        REQUIRE(c.registers.get_bc() == 0xFFFF);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
    }

    SECTION("DEC SP") {
        CPU c;
        c.sp = 0xC000;
        instruct.alu16.target = Operand16::SP;

        c.execute(instruct);

        REQUIRE(c.sp == 0xBFFF);
    }
}

TEST_CASE("ADDSP Unit Tests", "[cpu][16-bit][addsp]") {
    Instruction instruct;
    instruct.type = InstructionType::ADDSP;

    SECTION("ADD SP,e positive offset w/ half carry and carry") {
        CPU c;
        c.sp = 0xC0FF;
        c.bus.write_byte(c.pc + 1, 0x01);
        c.registers.f.zero = true;
        c.registers.f.subtract = true;
        uint16_t start_pc = c.pc;

        c.execute(instruct);

        REQUIRE(c.sp == 0xC100);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.subtract == false);
        REQUIRE(c.registers.f.half_carry == true);
        REQUIRE(c.registers.f.carry == true);
        REQUIRE(c.pc == start_pc + 2);
    }

    SECTION("ADD SP,e negative offset") {
        CPU c;
        c.sp = 0xC000;
        int8_t offset = -1;
        c.bus.write_byte(c.pc + 1, (uint8_t)offset);

        c.execute(instruct);

        // Flags come from unsigned 0x00 + 0xFF on the low byte
        REQUIRE(c.sp == 0xBFFF);
        REQUIRE(c.registers.f.half_carry == false);
        REQUIRE(c.registers.f.carry == false);
    }
}

TEST_CASE("DAA Unit Tests", "[cpu][misc][daa]") {
    Instruction instruct;
    instruct.type = InstructionType::DAA;

    SECTION("DAA after addition w/o carry (15 + 27 = 42)") {
        CPU c;
        c.registers.a = 0x3C; // 0x15 + 0x27 in binary
        uint16_t start_pc = c.pc;

        c.execute(instruct);

        REQUIRE(c.registers.a == 0x42);
        REQUIRE(c.registers.f.zero == false);
        REQUIRE(c.registers.f.carry == false);
        REQUIRE(c.registers.f.half_carry == false);
        REQUIRE(c.pc == start_pc + 1);
    }

    SECTION("DAA after addition w/ decimal carry (99 + 1 = 100)") {
        CPU c;
        c.registers.a = 0x9A; // 0x99 + 0x01 in binary

        c.execute(instruct);

        REQUIRE(c.registers.a == 0x00);
        REQUIRE(c.registers.f.zero == true);
        REQUIRE(c.registers.f.carry == true);
    }

    SECTION("DAA after addition w/ half carry (09 + 08 = 17)") {
        CPU c;
        c.registers.a = 0x11; // 0x09 + 0x08 in binary, carried out of the low nibble
        c.registers.f.half_carry = true;

        c.execute(instruct);

        REQUIRE(c.registers.a == 0x17);
        REQUIRE(c.registers.f.half_carry == false);
    }

    SECTION("DAA after subtraction w/ half borrow (42 - 15 = 27)") {
        CPU c;
        c.registers.a = 0x2D; // 0x42 - 0x15 in binary
        c.registers.f.subtract = true;
        c.registers.f.half_carry = true;

        c.execute(instruct);

        REQUIRE(c.registers.a == 0x27);
        REQUIRE(c.registers.f.subtract == true);
        REQUIRE(c.registers.f.carry == false);
    }

    SECTION("DAA after subtraction w/ borrow (10 - 20 = 90, borrow)") {
        CPU c;
        c.registers.a = 0xF0; // 0x10 - 0x20 in binary
        c.registers.f.subtract = true;
        c.registers.f.carry = true;

        c.execute(instruct);

        REQUIRE(c.registers.a == 0x90);
        REQUIRE(c.registers.f.carry == true);
    }
}

TEST_CASE("NOP and STOP Unit Tests", "[cpu][misc][nop]") {
    SECTION("NOP only advances pc") {
        CPU c;
        Instruction instruct;
        instruct.type = InstructionType::NOP;
        c.registers.a = 0x12;
        uint16_t start_pc = c.pc;
        uint16_t start_sp = c.sp;

        c.execute(instruct);

        REQUIRE(c.pc == start_pc + 1);
        REQUIRE(c.sp == start_sp);
        REQUIRE(c.registers.a == 0x12);
    }

    SECTION("STOP is 2 bytes wide") {
        CPU c;
        Instruction instruct;
        instruct.type = InstructionType::STOP;
        uint16_t start_pc = c.pc;

        c.execute(instruct);

        REQUIRE(c.pc == start_pc + 2);
    }
}

TEST_CASE("step() T-cycle Unit Tests", "[cpu][cycles]") {
    // Writes the instruction bytes at pc, runs one step and returns the T-cycles it took
    auto run = [](CPU& c, std::initializer_list<uint8_t> bytes) -> uint8_t {
        uint16_t addr = c.pc;
        for (uint8_t byte : bytes) {
            c.bus.write_byte(addr, byte);
            addr = addr + 1;
        }
        return c.step();
    };

    SECTION("Register-only instructions take 4 (opcode fetch only)") {
        CPU c;
        REQUIRE(run(c, {0x00}) == 4);       // NOP
        REQUIRE(run(c, {0x41}) == 4);       // LD B,C
        REQUIRE(run(c, {0x80}) == 4);       // ADD A,B
    }

    SECTION("Memory accesses add 4 each") {
        CPU c;
        c.registers.set_hl(0xC000);
        REQUIRE(run(c, {0x86}) == 8);             // ADD A,(HL): fetch + read
        REQUIRE(run(c, {0xC6, 0x01}) == 8);       // ADD A,n: fetch + read n
        REQUIRE(run(c, {0x34}) == 12);            // INC (HL): fetch + read + write
        REQUIRE(run(c, {0x36, 0x01}) == 12);      // LD (HL),n: fetch + read n + write
        REQUIRE(run(c, {0x01, 0x34, 0x12}) == 12); // LD BC,nn: fetch + 2 reads
    }

    SECTION("Internal steps add cycles without memory accesses") {
        CPU c;
        REQUIRE(run(c, {0x09}) == 8);  // ADD HL,BC
        REQUIRE(run(c, {0x03}) == 8);  // INC BC
    }

    SECTION("Stack and 16-bit memory instructions") {
        CPU c;
        c.sp = 0xDFF0;
        REQUIRE(run(c, {0xC5}) == 16);              // PUSH BC
        REQUIRE(run(c, {0xC1}) == 12);              // POP BC
        REQUIRE(run(c, {0x08, 0x00, 0xC0}) == 20);  // LD (nn),SP
    }

    SECTION("JR cc taken vs not taken") {
        CPU c;
        c.registers.f.zero = false;
        REQUIRE(run(c, {0x20, 0x00}) == 12);  // JR NZ taken
        c.registers.f.zero = true;
        REQUIRE(run(c, {0x20, 0x00}) == 8);   // JR NZ not taken
    }

    SECTION("JP cc taken vs not taken") {
        CPU c;
        c.registers.f.carry = true;
        REQUIRE(run(c, {0xDA, 0x00, 0xC0}) == 16);  // JP C taken
        c.pc = 0x0100;
        c.registers.f.carry = false;
        REQUIRE(run(c, {0xDA, 0x00, 0xC0}) == 12);  // JP C not taken
    }

    SECTION("CALL cc taken vs not taken") {
        CPU c;
        c.registers.f.zero = true;
        REQUIRE(run(c, {0xCC, 0x00, 0x02}) == 24);  // CALL Z taken
        c.pc = 0x0100;
        c.registers.f.zero = false;
        REQUIRE(run(c, {0xCC, 0x00, 0x02}) == 12);  // CALL Z not taken
    }

    SECTION("RET cc taken vs not taken") {
        CPU c;
        c.push(0x0100);
        c.registers.f.carry = false;
        REQUIRE(run(c, {0xD0}) == 20);  // RET NC taken
        c.registers.f.carry = true;
        REQUIRE(run(c, {0xD0}) == 8);   // RET NC not taken
    }

    SECTION("Unconditional jumps, calls and returns") {
        CPU c;
        REQUIRE(run(c, {0x18, 0x00}) == 12);        // JR e
        REQUIRE(run(c, {0xC3, 0x00, 0x02}) == 16);  // JP nn
        REQUIRE(run(c, {0xCD, 0x00, 0x03}) == 24);  // CALL nn
        REQUIRE(run(c, {0xC9}) == 16);              // RET
        REQUIRE(run(c, {0xFF}) == 16);              // RST 38
        c.registers.set_hl(0x0400);
        REQUIRE(run(c, {0xE9}) == 4);               // JP HL
    }

    SECTION("CB-prefixed instructions include the prefix fetch") {
        CPU c;
        c.registers.set_hl(0xC000);
        REQUIRE(run(c, {0xCB, 0x30}) == 8);   // SWAP B
        REQUIRE(run(c, {0xCB, 0x46}) == 12);  // BIT 0,(HL): read only
        REQUIRE(run(c, {0xCB, 0xC6}) == 16);  // SET 0,(HL): read + write
        REQUIRE(run(c, {0xCB, 0x06}) == 16);  // RLC (HL): read + write
    }

    SECTION("Every decodable opcode has a cycle count, illegal opcodes don't") {
        for (int op = 0; op < 256; op++) {
            if (op == 0xCB) {
                continue;
            }

            bool decodable = from_byte_not_prefixed(op).has_value();
            INFO("unprefixed opcode " << op);
            REQUIRE((UNPREFIXED_CYCLES[op] != 0) == decodable);

            INFO("prefixed opcode " << op);
            REQUIRE(PREFIXED_CYCLES[op] >= 8);
        }
    }
}
