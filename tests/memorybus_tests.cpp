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

TEST_CASE("MemoryBus serial port Unit Tests", "[memorybus][serial]") {
    // {} value-initializes the bus so memory (and IF) start at zero instead of garbage
    SECTION ("Internal clock transfer sends SB and completes") {
        MemoryBus bus{};
        bus.write_byte(MemoryBus::SB_ADDRESS, 'P');
        bus.write_byte(MemoryBus::SC_ADDRESS, 0x81);

        REQUIRE(bus.serial_output == "P");
        REQUIRE(bus.read_byte(MemoryBus::SB_ADDRESS) == 0xFF); // Disconnected cable shifts in all 1s
        REQUIRE(bus.read_byte(MemoryBus::SC_ADDRESS) == 0x01); // Bit 7 cleared, clock select kept
        REQUIRE(bus.read_byte(MemoryBus::IF_ADDRESS) == 0x08); // Serial interrupt requested
    }

    SECTION ("Consecutive transfers append to serial output") {
        MemoryBus bus{};
        bus.write_byte(MemoryBus::SB_ADDRESS, 'O');
        bus.write_byte(MemoryBus::SC_ADDRESS, 0x81);
        bus.write_byte(MemoryBus::SB_ADDRESS, 'K');
        bus.write_byte(MemoryBus::SC_ADDRESS, 0x81);

        REQUIRE(bus.serial_output == "OK");
    }

    SECTION ("Serial interrupt leaves other IF bits alone") {
        MemoryBus bus{};
        bus.write_byte(MemoryBus::IF_ADDRESS, 0x01); // V-Blank already requested
        bus.write_byte(MemoryBus::SB_ADDRESS, 'P');
        bus.write_byte(MemoryBus::SC_ADDRESS, 0x81);

        REQUIRE(bus.read_byte(MemoryBus::IF_ADDRESS) == 0x09);
    }

    SECTION ("External clock transfer never completes with no cable") {
        MemoryBus bus{};
        bus.write_byte(MemoryBus::SB_ADDRESS, 'P');
        bus.write_byte(MemoryBus::SC_ADDRESS, 0x80);

        REQUIRE(bus.serial_output.empty());
        REQUIRE(bus.read_byte(MemoryBus::SB_ADDRESS) == 'P');
        REQUIRE(bus.read_byte(MemoryBus::SC_ADDRESS) == 0x80); // Still waiting for a partner
        REQUIRE(bus.read_byte(MemoryBus::IF_ADDRESS) == 0x00);
    }

    SECTION ("Selecting internal clock without requesting a transfer does nothing") {
        MemoryBus bus{};
        bus.write_byte(MemoryBus::SB_ADDRESS, 'P');
        bus.write_byte(MemoryBus::SC_ADDRESS, 0x01);

        REQUIRE(bus.serial_output.empty());
        REQUIRE(bus.read_byte(MemoryBus::SB_ADDRESS) == 'P');
        REQUIRE(bus.read_byte(MemoryBus::SC_ADDRESS) == 0x01);
        REQUIRE(bus.read_byte(MemoryBus::IF_ADDRESS) == 0x00);
    }

    SECTION ("Writing SB alone does not start a transfer") {
        MemoryBus bus{};
        bus.write_byte(MemoryBus::SB_ADDRESS, 'P');

        REQUIRE(bus.serial_output.empty());
        REQUIRE(bus.read_byte(MemoryBus::SB_ADDRESS) == 'P');
        REQUIRE(bus.read_byte(MemoryBus::IF_ADDRESS) == 0x00);
    }
}
