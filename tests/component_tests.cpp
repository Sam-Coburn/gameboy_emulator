#include <catch2/catch_test_macros.hpp>
#include <cartridge.hpp>
#include <interrupts.hpp>
#include <ppu.hpp>
#include <serial.hpp>

#include <stdexcept>
#include <vector>

// These test each component on its own, with no bus. That's possible because components don't
// hold a reference to the bus: they only take addresses in and return values

TEST_CASE("Serial Unit Tests", "[serial]") {
    Serial serial;

    SECTION("Internal clock transfer returns the serial interrupt bit") {
        serial.write(Serial::SB_ADDRESS, 'P');

        REQUIRE(serial.write(Serial::SC_ADDRESS, 0x81) == Interrupt::SERIAL);
        REQUIRE(serial.output == "P");
        REQUIRE(serial.read(Serial::SB_ADDRESS) == 0xFF); // Disconnected cable shifts in all 1s
        REQUIRE(serial.read(Serial::SC_ADDRESS) == 0x01); // Bit 7 cleared, clock select kept
    }

    SECTION("External clock transfer never completes with no cable") {
        serial.write(Serial::SB_ADDRESS, 'P');

        REQUIRE(serial.write(Serial::SC_ADDRESS, 0x80) == 0);
        REQUIRE(serial.output.empty());
        REQUIRE(serial.read(Serial::SB_ADDRESS) == 'P');
        REQUIRE(serial.read(Serial::SC_ADDRESS) == 0x80); // Still waiting for a partner
    }

    SECTION("Selecting internal clock without requesting a transfer does nothing") {
        serial.write(Serial::SB_ADDRESS, 'P');

        REQUIRE(serial.write(Serial::SC_ADDRESS, 0x01) == 0);
        REQUIRE(serial.output.empty());
        REQUIRE(serial.read(Serial::SB_ADDRESS) == 'P');
    }

    SECTION("Writing SB alone does not start a transfer") {
        REQUIRE(serial.write(Serial::SB_ADDRESS, 'P') == 0);
        REQUIRE(serial.output.empty());
    }
}

TEST_CASE("Cartridge Unit Tests", "[cartridge]") {
    SECTION("Reads ROM and ignores writes to it") {
        Cartridge cart(std::vector<uint8_t>{0x11, 0x22, 0x33});
        cart.write(0x0001, 0x99);

        REQUIRE(cart.read(0x0000) == 0x11);
        REQUIRE(cart.read(0x0001) == 0x22);
    }

    SECTION("Reads past the end of a small ROM return 0xFF") {
        Cartridge cart(std::vector<uint8_t>{0x11});

        REQUIRE(cart.read(0x0001) == 0xFF);
        REQUIRE(cart.read(0x7FFF) == 0xFF);
    }

    SECTION("External RAM is readable and writable") {
        Cartridge cart;
        cart.write(0xA000, 0x55);
        cart.write(0xBFFF, 0x66);

        REQUIRE(cart.read(0xA000) == 0x55);
        REQUIRE(cart.read(0xBFFF) == 0x66);
    }

    SECTION("ROMs bigger than 32 KB need bank switching") {
        REQUIRE_THROWS_AS(Cartridge(std::vector<uint8_t>(0x8001, 0)), std::runtime_error);
    }
}

TEST_CASE("Ppu memory Unit Tests", "[ppu]") {
    Ppu ppu;

    SECTION("VRAM covers 8000-9FFF") {
        ppu.write(0x8000, 0x12);
        ppu.write(0x9FFF, 0x34);

        REQUIRE(ppu.read(0x8000) == 0x12);
        REQUIRE(ppu.read(0x9FFF) == 0x34);
        REQUIRE(ppu.vram[0x1FFF] == 0x34);
    }

    SECTION("OAM covers FE00-FE9F") {
        ppu.write(0xFE00, 0x56);
        ppu.write(0xFE9F, 0x78);

        REQUIRE(ppu.read(0xFE00) == 0x56);
        REQUIRE(ppu.read(0xFE9F) == 0x78);
        REQUIRE(ppu.oam[0x9F] == 0x78);
    }
}
