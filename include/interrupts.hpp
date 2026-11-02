#pragma once

#include <stdint.h>

// Bits of the IF (FF0F) and IE (FFFF) registers. Components return these from their write()
// and tick() methods to request an interrupt, and the bus ORs them into IF. Lower bits have
// higher priority
namespace Interrupt {
    constexpr uint8_t VBLANK = 0x01;
    constexpr uint8_t STAT = 0x02;
    constexpr uint8_t TIMER = 0x04;
    constexpr uint8_t SERIAL = 0x08;
    constexpr uint8_t JOYPAD = 0x10;
}
