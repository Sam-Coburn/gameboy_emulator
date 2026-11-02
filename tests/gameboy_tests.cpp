#include <catch2/catch_test_macros.hpp>
#include <gameboy.hpp>

#include <utility>
#include <vector>

TEST_CASE("GameBoy starts in the post-boot state", "[gameboy][postboot]") {
    GameBoy gb(Cartridge(std::vector<uint8_t>(0x8000, 0)));

    SECTION("CPU registers") {
        REQUIRE(gb.cpu.registers.get_af() == 0x01B0);
        REQUIRE(gb.cpu.registers.get_bc() == 0x0013);
        REQUIRE(gb.cpu.registers.get_de() == 0x00D8);
        REQUIRE(gb.cpu.registers.get_hl() == 0x014D);
        REQUIRE(gb.cpu.sp == 0xFFFE);
        REQUIRE(gb.cpu.pc == 0x0100);
    }

    SECTION("CPU uses the GameBoy's bus") {
        REQUIRE(&gb.cpu.bus == &gb.bus);
    }

    SECTION("I/O registers read back through the bus") {
        REQUIRE(gb.bus.read_byte(0xFF00) == 0xCF); // P1
        REQUIRE(gb.bus.read_byte(0xFF02) == 0x7E); // SC
        REQUIRE(gb.bus.read_byte(0xFF07) == 0xF8); // TAC
        REQUIRE(gb.bus.read_byte(0xFF0F) == 0xE1); // IF
        REQUIRE(gb.bus.read_byte(0xFF26) == 0xF1); // NR52
        REQUIRE(gb.bus.read_byte(0xFF40) == 0x91); // LCDC
        REQUIRE(gb.bus.read_byte(0xFF41) == 0x85); // STAT
        REQUIRE(gb.bus.read_byte(0xFF47) == 0xFC); // BGP
        REQUIRE(gb.bus.read_byte(0xFFFF) == 0x00); // IE
    }

    SECTION("RAM is zeroed") {
        REQUIRE(gb.bus.read_byte(0xC000) == 0x00);
        REQUIRE(gb.bus.read_byte(0xFF80) == 0x00);
        REQUIRE(gb.bus.read_byte(0x8000) == 0x00);
    }
}

TEST_CASE("GameBoy step runs the program from the cartridge", "[gameboy]") {
    std::vector<uint8_t> rom(0x8000, 0);
    rom[0x0100] = 0x3E; // LD A,0x42
    rom[0x0101] = 0x42;
    GameBoy gb(Cartridge(std::move(rom)));

    REQUIRE(gb.step() == 8);
    REQUIRE(gb.cpu.registers.a == 0x42);
    REQUIRE(gb.cpu.pc == 0x0102);
}
