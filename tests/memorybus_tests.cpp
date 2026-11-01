#include <catch2/catch_test_macros.hpp>
#include <memorybus.hpp>

TEST_CASE("MemoryBus Unit Tests", "[memorybus]") {
    SECTION ("Read and write lowest address") {
        MemoryBus bus;
        bus.write_byte(0x0000, 0x12);

        REQUIRE(bus.read_byte(0x0000) == 0x12);
    }

    SECTION ("Read and write highest address (IE register)") {
        MemoryBus bus;
        bus.write_byte(0xFFFF, 0x1F);

        REQUIRE(bus.read_byte(0xFFFF) == 0x1F);
    }
}

TEST_CASE("MemoryBus word Unit Tests", "[memorybus][word]") {
    SECTION ("Words are little endian") {
        MemoryBus bus;
        bus.write_word(0xC000, 0x1234);

        REQUIRE(bus.read_byte(0xC000) == 0x34);
        REQUIRE(bus.read_byte(0xC001) == 0x12);
        REQUIRE(bus.read_word(0xC000) == 0x1234);
    }

    SECTION ("High byte wraps from 0xFFFF to 0x0000") {
        MemoryBus bus;
        bus.write_word(0xFFFF, 0xABCD);

        REQUIRE(bus.read_byte(0xFFFF) == 0xCD);
        REQUIRE(bus.read_byte(0x0000) == 0xAB);
        REQUIRE(bus.read_word(0xFFFF) == 0xABCD);
    }
}
