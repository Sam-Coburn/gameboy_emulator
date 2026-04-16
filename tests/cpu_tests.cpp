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

TEST_CASE("RESET Unit Tests", "[cpu][bit_opcode][reset]") {
    Instruction instruct;
    instruct.type = InstructionType::RESET;

    SECTION ("RESET 1") {
        CPU c;
        instruct.target_8bit = ArithmeticTarget8Bit::A;
        instruct.bit_index = 3;

        c.registers.a = 255;

        c.execute(instruct);

        REQUIRE(c.registers.a == 0b11110111);
    }

    SECTION ("RESET 2") {
        CPU c;
        instruct.target_8bit = ArithmeticTarget8Bit::A;
        instruct.bit_index = 7;

        c.registers.a = 255;

        c.execute(instruct);

        REQUIRE(c.registers.a == 0b01111111);
    }

    SECTION ("RESET 3") {
        CPU c;
        instruct.target_8bit = ArithmeticTarget8Bit::A;
        instruct.bit_index = 0;

        c.registers.a = 255;

        c.execute(instruct);

        REQUIRE(c.registers.a == 0b11111110);
    }

    SECTION ("RESET 4") {
        CPU c;
        instruct.target_8bit = ArithmeticTarget8Bit::B;
        instruct.bit_index = 0;

        c.registers.b = 255;

        c.execute(instruct);

        REQUIRE(c.registers.b == 0b11111110);
    }

    SECTION ("RESET 5") {
        CPU c;
        instruct.target_8bit = ArithmeticTarget8Bit::C;
        instruct.bit_index = 0;

        c.registers.c = 255;

        c.execute(instruct);

        REQUIRE(c.registers.c == 0b11111110);
    }

    SECTION ("RESET 6") {
        CPU c;
        instruct.target_8bit = ArithmeticTarget8Bit::D;
        instruct.bit_index = 0;

        c.registers.d = 255;

        c.execute(instruct);

        REQUIRE(c.registers.d == 0b11111110);
    }

    SECTION ("RESET 7") {
        CPU c;
        instruct.target_8bit = ArithmeticTarget8Bit::E;
        instruct.bit_index = 0;

        c.registers.e = 255;

        c.execute(instruct);

        REQUIRE(c.registers.e == 0b11111110);
    }

    SECTION ("RESET 8") {
        CPU c;
        instruct.target_8bit = ArithmeticTarget8Bit::H;
        instruct.bit_index = 0;

        c.registers.h = 255;

        c.execute(instruct);

        REQUIRE(c.registers.h == 0b11111110);
    }

    SECTION ("RESET 9") {
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