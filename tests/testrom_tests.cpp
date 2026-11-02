#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <testromrunner.hpp>

#include <filesystem>
#include <string>

// Blargg's ROMs aren't committed. Download gb-test-roms into roms/ to run these
// (see roms/README.md); tests are skipped if the ROMs are missing
static const std::string CPU_INSTRS_DIR = std::string(ROMS_DIR) + "/gb-test-roms/cpu_instrs/individual/";

static void require_rom_passes(const std::string& rom) {
    std::string path = CPU_INSTRS_DIR + rom;

    if (!std::filesystem::exists(path)) {
        SKIP("ROM not found: " + path);
    }

    std::string output = TestRomRunner::run(path);

    INFO("ROM: " << rom);
    INFO("Serial output:\n" << output);
    REQUIRE(output.find("Passed") != std::string::npos);
}

TEST_CASE("Blargg cpu_instrs test ROMs", "[testrom][blargg]") {
    std::string rom = GENERATE(
        "01-special.gb",
        "03-op sp,hl.gb",
        "04-op r,imm.gb",
        "05-op rp.gb",
        "06-ld r,r.gb",
        "07-jr,jp,call,ret,rst.gb",
        "08-misc instrs.gb",
        "09-op r,r.gb",
        "10-bit ops.gb",
        "11-op a,(hl).gb"
    );

    require_rom_passes(rom);
}

// Tests the timer interrupt, so it fails until the timer is implemented (#19).
// [!mayfail] still runs it and reports the result without failing the suite
TEST_CASE("Blargg cpu_instrs 02-interrupts test ROM", "[testrom][blargg][!mayfail]") {
    require_rom_passes("02-interrupts.gb");
}
