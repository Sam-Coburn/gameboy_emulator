#include <catch2/catch_test_macros.hpp>
#include <registers.hpp>

TEST_CASE("Default constructor initializes all register contents to 0", "[registers]") {
    Registers r;

    REQUIRE(r.a == 0);
    REQUIRE(r.b == 0);
    REQUIRE(r.c == 0);
    REQUIRE(r.d == 0);
    REQUIRE(r.e == 0);
    REQUIRE(r.h == 0);
    REQUIRE(r.l == 0);

    REQUIRE(r.f.zero == false);
    REQUIRE(r.f.subtract == false);
    REQUIRE(r.f.half_carry == false);
    REQUIRE(r.f.carry == false);
}

TEST_CASE("Set and get functions properly set and retrieve register values with provided contents", "[registers][set/get]") {
    Registers r;

    SECTION ("set_af() and get_af() function calls") {
        r.set_af(0b1000000010000000);

        REQUIRE(r.a == 0b10000000);
        REQUIRE(r.f.zero == true);
        REQUIRE(r.f.subtract == false);
        REQUIRE(r.f.half_carry == false);
        REQUIRE(r.f.carry == false);

        uint16_t result = r.get_af();

        REQUIRE(result == 0b1000000010000000);
    }

    // Todo: Add unit tests for other register pairs
}