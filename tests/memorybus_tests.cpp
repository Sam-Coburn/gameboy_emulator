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
