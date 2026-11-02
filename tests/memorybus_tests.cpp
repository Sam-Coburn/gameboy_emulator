#include <catch2/catch_test_macros.hpp>
#include <interrupts.hpp>
#include <memorybus.hpp>

#include <vector>

// A bus with a blank 32 KB ROM in the cartridge slot
static MemoryBus make_bus() {
    MemoryBus bus;
    bus.cartridge = Cartridge(std::vector<uint8_t>(0x8000, 0));
    return bus;
}

TEST_CASE("MemoryBus routes each range to its owner", "[memorybus][routing]") {
    MemoryBus bus = make_bus();

    SECTION("0000-7FFF reads cartridge ROM") {
        bus.cartridge.rom[0x0000] = 0x11;
        bus.cartridge.rom[0x7FFF] = 0x22;

        REQUIRE(bus.read_byte(0x0000) == 0x11);
        REQUIRE(bus.read_byte(0x7FFF) == 0x22);
    }

    SECTION("0000-7FFF writes go to the cartridge and don't change ROM") {
        bus.cartridge.rom[0x2000] = 0x11;
        bus.write_byte(0x2000, 0x99); // On an MBC cartridge this would select a ROM bank

        REQUIRE(bus.read_byte(0x2000) == 0x11);
    }

    SECTION("8000-9FFF is VRAM") {
        bus.write_byte(0x8000, 0x33);
        bus.write_byte(0x9FFF, 0x44);

        REQUIRE(bus.ppu.vram[0x0000] == 0x33);
        REQUIRE(bus.ppu.vram[0x1FFF] == 0x44);
        REQUIRE(bus.read_byte(0x8000) == 0x33);
        REQUIRE(bus.read_byte(0x9FFF) == 0x44);
    }

    SECTION("A000-BFFF is cartridge RAM") {
        bus.write_byte(0xA000, 0x55);
        bus.write_byte(0xBFFF, 0x66);

        REQUIRE(bus.cartridge.ram[0x0000] == 0x55);
        REQUIRE(bus.cartridge.ram[0x1FFF] == 0x66);
        REQUIRE(bus.read_byte(0xA000) == 0x55);
        REQUIRE(bus.read_byte(0xBFFF) == 0x66);
    }

    SECTION("C000-DFFF is work RAM") {
        bus.write_byte(0xC000, 0x77);
        bus.write_byte(0xDFFF, 0x88);

        REQUIRE(bus.wram[0x0000] == 0x77);
        REQUIRE(bus.wram[0x1FFF] == 0x88);
        REQUIRE(bus.read_byte(0xC000) == 0x77);
        REQUIRE(bus.read_byte(0xDFFF) == 0x88);
    }

    SECTION("FE00-FE9F is OAM") {
        bus.write_byte(0xFE00, 0x99);
        bus.write_byte(0xFE9F, 0xAA);

        REQUIRE(bus.ppu.oam[0x00] == 0x99);
        REQUIRE(bus.ppu.oam[0x9F] == 0xAA);
        REQUIRE(bus.read_byte(0xFE00) == 0x99);
        REQUIRE(bus.read_byte(0xFE9F) == 0xAA);
    }

    SECTION("FEA0-FEFF is unusable: reads 0xFF, writes ignored") {
        bus.write_byte(0xFEA0, 0x12);
        bus.write_byte(0xFEFF, 0x34);

        REQUIRE(bus.read_byte(0xFEA0) == 0xFF);
        REQUIRE(bus.read_byte(0xFEFF) == 0xFF);
        REQUIRE(bus.ppu.oam[0x9F] == 0x00); // Didn't spill into the end of OAM
    }

    SECTION("FF80-FFFE is high RAM") {
        bus.write_byte(0xFF80, 0xBB);
        bus.write_byte(0xFFFE, 0xCC);

        REQUIRE(bus.hram[0x00] == 0xBB);
        REQUIRE(bus.hram[0x7E] == 0xCC);
        REQUIRE(bus.read_byte(0xFF80) == 0xBB);
        REQUIRE(bus.read_byte(0xFFFE) == 0xCC);
    }

    SECTION("FFFF is IE") {
        bus.write_byte(0xFFFF, 0x1F);

        REQUIRE(bus.interrupt_enable == 0x1F);
        REQUIRE(bus.read_byte(0xFFFF) == 0x1F);
    }

    SECTION("FF0F is IF") {
        bus.write_byte(0xFF0F, 0x05);

        REQUIRE(bus.interrupt_flag == 0x05);
        REQUIRE(bus.read_byte(0xFF0F) == 0x05);
    }

    SECTION("FF01-FF02 go to the serial port") {
        bus.write_byte(Serial::SB_ADDRESS, 'A');

        REQUIRE(bus.serial.sb == 'A');
        REQUIRE(bus.read_byte(Serial::SB_ADDRESS) == 'A');
    }

    SECTION("Other I/O registers are stored until their component exists") {
        bus.write_byte(0xFF40, 0x91); // LCDC
        bus.write_byte(0xFF7F, 0x42);

        REQUIRE(bus.io[0x40] == 0x91);
        REQUIRE(bus.read_byte(0xFF40) == 0x91);
        REQUIRE(bus.read_byte(0xFF7F) == 0x42);
    }
}

TEST_CASE("Echo RAM mirrors work RAM", "[memorybus][echo]") {
    MemoryBus bus = make_bus();

    SECTION("Writes to work RAM appear in echo RAM") {
        bus.write_byte(0xC000, 0x12);
        bus.write_byte(0xDDFF, 0x34);

        REQUIRE(bus.read_byte(0xE000) == 0x12);
        REQUIRE(bus.read_byte(0xFDFF) == 0x34);
    }

    SECTION("Writes to echo RAM appear in work RAM") {
        bus.write_byte(0xE123, 0x56);

        REQUIRE(bus.wram[0x0123] == 0x56);
        REQUIRE(bus.read_byte(0xC123) == 0x56);
    }

    SECTION("Echo RAM stops before OAM, so DE00-DFFF has no mirror") {
        bus.write_byte(0xDE00, 0x78);

        REQUIRE(bus.read_byte(0xFE00) == 0x00); // FE00 is OAM, not a mirror of DE00
    }
}

TEST_CASE("MemoryBus starts zeroed", "[memorybus]") {
    MemoryBus bus = make_bus();

    REQUIRE(bus.read_byte(0xC000) == 0x00);
    REQUIRE(bus.read_byte(0xDFFF) == 0x00);
    REQUIRE(bus.read_byte(0xFF80) == 0x00);
    REQUIRE(bus.read_byte(0x8000) == 0x00);
    REQUIRE(bus.read_byte(MemoryBus::IF_ADDRESS) == 0x00);
    REQUIRE(bus.read_byte(MemoryBus::IE_ADDRESS) == 0x00);
}

TEST_CASE("MemoryBus word Unit Tests", "[memorybus][word]") {
    MemoryBus bus = make_bus();

    SECTION ("Words are little endian") {
        bus.write_word(0xC000, 0x1234);

        REQUIRE(bus.read_byte(0xC000) == 0x34);
        REQUIRE(bus.read_byte(0xC001) == 0x12);
        REQUIRE(bus.read_word(0xC000) == 0x1234);
    }

    SECTION ("High byte wraps from 0xFFFF to 0x0000") {
        bus.cartridge.rom[0x0000] = 0xAB;
        bus.write_byte(0xFFFF, 0xCD);

        REQUIRE(bus.read_word(0xFFFF) == 0xABCD);
    }
}

TEST_CASE("MemoryBus serial port Unit Tests", "[memorybus][serial]") {
    MemoryBus bus = make_bus();

    SECTION ("Internal clock transfer sends SB and completes") {
        bus.write_byte(Serial::SB_ADDRESS, 'P');
        bus.write_byte(Serial::SC_ADDRESS, 0x81);

        REQUIRE(bus.serial.output == "P");
        REQUIRE(bus.read_byte(Serial::SB_ADDRESS) == 0xFF); // Disconnected cable shifts in all 1s
        REQUIRE(bus.read_byte(Serial::SC_ADDRESS) == 0x01); // Bit 7 cleared, clock select kept
        REQUIRE(bus.read_byte(MemoryBus::IF_ADDRESS) == Interrupt::SERIAL);
    }

    SECTION ("Consecutive transfers append to serial output") {
        bus.write_byte(Serial::SB_ADDRESS, 'O');
        bus.write_byte(Serial::SC_ADDRESS, 0x81);
        bus.write_byte(Serial::SB_ADDRESS, 'K');
        bus.write_byte(Serial::SC_ADDRESS, 0x81);

        REQUIRE(bus.serial.output == "OK");
    }

    SECTION ("Serial interrupt leaves other IF bits alone") {
        bus.write_byte(MemoryBus::IF_ADDRESS, Interrupt::VBLANK); // V-Blank already requested
        bus.write_byte(Serial::SB_ADDRESS, 'P');
        bus.write_byte(Serial::SC_ADDRESS, 0x81);

        REQUIRE(bus.read_byte(MemoryBus::IF_ADDRESS) == (Interrupt::VBLANK | Interrupt::SERIAL));
    }

    SECTION ("External clock transfer never completes with no cable") {
        bus.write_byte(Serial::SB_ADDRESS, 'P');
        bus.write_byte(Serial::SC_ADDRESS, 0x80);

        REQUIRE(bus.serial.output.empty());
        REQUIRE(bus.read_byte(MemoryBus::IF_ADDRESS) == 0x00);
    }
}
