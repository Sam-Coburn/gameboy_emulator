#include <gameboy.hpp>

#include <utility>

GameBoy::GameBoy(Cartridge cartridge) : cpu(bus) {
    bus.cartridge = std::move(cartridge);
    bus.apply_post_boot_state();
    cpu.apply_post_boot_state();
}

uint8_t GameBoy::step() {
    uint8_t cycles = cpu.step();
    bus.tick(cycles);
    return cycles;
}
