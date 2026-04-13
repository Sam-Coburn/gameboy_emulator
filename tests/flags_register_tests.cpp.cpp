#include <catch2/catch_test_macros.hpp>
#include <flagsregister.hpp>

// Test for Unit Test Suite Implementation
TEST_CASE("basic math works") {
    REQUIRE(1 + 1 == 2);
}

// FlagsRegister Tests

TEST_CASE("Default constructor initializes all flags to false", "[flags]") {
    FlagsRegister f;

    REQUIRE(f.zero == false);
    REQUIRE(f.subtract == false);
    REQUIRE(f.half_carry == false);
    REQUIRE(f.carry == false);
}

TEST_CASE("FlagsRegister converts correctly to uint8_t", "[flags][encode]") {
    FlagsRegister f;

    SECTION("All flags false") {
        uint8_t result = static_cast<uint8_t>(f);
        REQUIRE(result == 0x00);
    }

    SECTION("Only zero flag") {
        f.zero = true;
        REQUIRE(static_cast<uint8_t>(f) == 0b10000000);
    }

    SECTION("Only subtract flag") {
        f.subtract = true;
        REQUIRE(static_cast<uint8_t>(f) == 0b01000000);
    }

    SECTION("Only half carry flag") {
        f.half_carry = true;
        REQUIRE(static_cast<uint8_t>(f) == 0b00100000);
    }

    SECTION("Only carry flag") {
        f.carry = true;
        REQUIRE(static_cast<uint8_t>(f) == 0b00010000);
    }

    SECTION("All flags true") {
        f.zero = true;
        f.subtract = true;
        f.half_carry = true;
        f.carry = true;

        REQUIRE(static_cast<uint8_t>(f) == 0b11110000);
    }
}

TEST_CASE("uint8_t converts correctly to FlagsRegister", "[flags][decode]") {

    SECTION("All flags false") {
        FlagsRegister f(0x00);

        REQUIRE_FALSE(f.zero);
        REQUIRE_FALSE(f.subtract);
        REQUIRE_FALSE(f.half_carry);
        REQUIRE_FALSE(f.carry);
    }

    SECTION("Individual flags") {
        FlagsRegister z(0b10000000);
        REQUIRE(z.zero);
        REQUIRE_FALSE(z.subtract);
        REQUIRE_FALSE(z.half_carry);
        REQUIRE_FALSE(z.carry);

        FlagsRegister n(0b01000000);
        REQUIRE_FALSE(n.zero);
        REQUIRE(n.subtract);
        REQUIRE_FALSE(n.half_carry);
        REQUIRE_FALSE(n.carry);

        FlagsRegister h(0b00100000);
        REQUIRE_FALSE(h.zero);
        REQUIRE_FALSE(h.subtract);
        REQUIRE(h.half_carry);
        REQUIRE_FALSE(h.carry);

        FlagsRegister c(0b00010000);
        REQUIRE_FALSE(c.zero);
        REQUIRE_FALSE(c.subtract);
        REQUIRE_FALSE(c.half_carry);
        REQUIRE(c.carry);
    }

    SECTION("All flags true") {
        FlagsRegister f(0b11110000);

        REQUIRE(f.zero);
        REQUIRE(f.subtract);
        REQUIRE(f.half_carry);
        REQUIRE(f.carry);
    }
}

TEST_CASE("Round-trip conversion preserves values", "[flags][roundtrip]") {
    std::vector<uint8_t> test_values = {
        0x00, 0x10, 0x20, 0x40, 0x80, 0xF0
    };

    for (uint8_t value : test_values) {
        FlagsRegister f(value);
        uint8_t result = static_cast<uint8_t>(f);

        REQUIRE(result == (value & 0xF0)); // lower 4 bits should be ignored
    }
}

TEST_CASE("Lower 4 bits are ignored during decoding", "[flags][edge]") {
    FlagsRegister f(0xFF); // 11111111

    REQUIRE(f.zero);
    REQUIRE(f.subtract);
    REQUIRE(f.half_carry);
    REQUIRE(f.carry);

    REQUIRE(static_cast<uint8_t>(f) == 0xF0);
}