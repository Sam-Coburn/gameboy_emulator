#include <cartridge.hpp>

#include <fstream>
#include <iterator>
#include <stdexcept>
#include <utility>

Cartridge::Cartridge(std::vector<uint8_t> rom) : rom(std::move(rom)) {
    if (this->rom.size() > 0x8000) {
        throw std::runtime_error("ROM is bigger than 32 KB and needs bank switching (#18)");
    }
}

Cartridge Cartridge::from_file(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Could not open ROM: " + path);
    }

    std::vector<uint8_t> data((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return Cartridge(std::move(data));
}

uint8_t Cartridge::read(uint16_t addr) const {
    if (addr < 0x8000) {
        return addr < rom.size() ? rom[addr] : 0xFF;
    }

    if (addr >= 0xA000 && addr <= 0xBFFF) {
        return ram[addr - 0xA000];
    }

    return 0xFF;
}

void Cartridge::write(uint16_t addr, uint8_t byte) {
    if (addr >= 0xA000 && addr <= 0xBFFF) {
        ram[addr - 0xA000] = byte;
    }
}
