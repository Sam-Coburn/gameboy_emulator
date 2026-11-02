# Roadmap: Running Pokémon Blue

**Goal:** the emulator is "finished" when Pokémon Blue runs fully: it boots, the overworld, menus and battles work, and an in-game save survives quitting and reloading the emulator.

**What Pokémon Blue needs:** it's an original Game Boy (DMG) game, a 1 MB ROM on an **MBC3** cartridge (type `0x13`: MBC3 + RAM + battery) with 32 KB of battery-backed save RAM. It needs graphics, input, interrupts and bank switching. Sound is needed for "fully run", but the game is playable without it.

## Big picture

The emulator is an interpreter that models the Game Boy's state, and the rules for changing it, faithfully enough that the game can't tell it isn't running on real hardware. The CPU runs the game's code. Every other component (graphics, timers, sound, cartridge, joypad) is controlled through **memory-mapped I/O**, meaning the CPU reads and writes special addresses. Real hardware runs all of these in parallel. The emulator runs one CPU instruction, then advances every other component by the same number of cycles.

```
            ┌──────────────── main loop (one frame = 70,224 cycles) ────────────────┐
            │                                                                       │
  CPU ──step()──► cycles ──► Timer.tick()  PPU.tick()  APU.tick()                   │
   │                                                                                │
   └──read/write──► MemoryBus ──► Cartridge (MBC3) │ VRAM/OAM (PPU) │ WRAM │ HRAM   │
                                  I/O registers: Timer, PPU, APU, Joypad, Serial,   │
                                  interrupt flags (IF/IE)                           │
            │                                                                       │
            └──► frame ready ──► Frontend (SDL): display, input, audio, pacing ─────┘
```

## Current state

- **Done:**
  - **CPU (#16):** all 256 unprefixed and 256 CB-prefixed opcodes, T-cycle timing (including taken branches), interrupts (`IME`, `IE`/`IF`, priority, vectors, delayed `EI`) and `HALT`.
  - **Serial (#22):** `SB`/`SC` with a "no cable connected" stub. Internal-clock transfers capture the byte, set `SB` to `0xFF` and request the serial interrupt.
  - **Headless test-ROM runner:** `build/gameboy <rom>` runs a ROM of 32 KB or less and prints its serial output. Blargg `cpu_instrs` tests also run as Catch2 tests tagged `[testrom]`.
- **Blargg `cpu_instrs`:** 10 of the 11 individual ROMs pass. `02-interrupts` fails until the timer (#19) exists, because it tests the timer interrupt. The combined `cpu_instrs.gb` is 64 KB and can't load until MBC1 banking (#18).
- **Memory bus (#17):** `MemoryBus` routes every address to its owner, with echo RAM mirroring WRAM. `GameBoy` owns the `MemoryBus` and the `CPU`, and the CPU holds a reference to the bus. The bus owns the components (cartridge, PPU memory, serial) plus WRAM, HRAM, `IF` and `IE`. Components never reference the bus: they return the interrupt bits they request, and the bus ORs them into `IF`. The machine starts in the post-boot state.
- **Not started:** cartridge banking, timers, PPU rendering, frontend, audio.

## Phases

| # | Phase | Issue | Depends on | Status |
|---|---|---|---|---|
| 1 | Finish the CPU: remaining opcodes, cycle counts, interrupts | [#16](https://github.com/Sam-Coburn/gameboy_emulator/issues/16) | (#22 for test ROMs) | ✅ Done |
| 2 | Memory bus address decoding and post-boot state | [#17](https://github.com/Sam-Coburn/gameboy_emulator/issues/17) | — | ✅ Done |
| 3 | Cartridge loading and MBC3 bank switching, battery saves | [#18](https://github.com/Sam-Coburn/gameboy_emulator/issues/18) | #17 | Not started |
| 4 | Timer and divider registers | [#19](https://github.com/Sam-Coburn/gameboy_emulator/issues/19) | #16, #17 | Not started |
| 5 | PPU (graphics) | [#20](https://github.com/Sam-Coburn/gameboy_emulator/issues/20) | #16, #17 | Not started |
| 6 | SDL frontend, main loop, joypad | [#21](https://github.com/Sam-Coburn/gameboy_emulator/issues/21) | #17, #19, #20 | Not started |
| 7 | Serial port output and link cable stub | [#22](https://github.com/Sam-Coburn/gameboy_emulator/issues/22) | — | ✅ Done |
| 8 | APU (audio) | [#23](https://github.com/Sam-Coburn/gameboy_emulator/issues/23) | #16, #21 | Not started |

Each issue explains what the component does conceptually, the technical work, and a "done when" check.

### 1. Finish the CPU ([#16](https://github.com/Sam-Coburn/gameboy_emulator/issues/16))
- **Opcodes:** `CALL`/`RET`/`RETI`/`RST`, `INC`/`DEC rr`, `ADD SP,e`, `DAA`, `NOP`, `HALT`, `STOP`, `DI`/`EI`.
- **Cycle counts:** `step()` returns the T-cycles each instruction took, including the extra time when a conditional branch is taken. This is the clock that keeps everything else in sync.
- **Interrupts:** the `IME`, `IE` (`FFFF`) and `IF` (`FF0F`) registers, priority order, the vectors `0x40`–`0x60`, the delayed effect of `EI`, and `HALT` waking up.

### 2. Memory bus ([#17](https://github.com/Sam-Coburn/gameboy_emulator/issues/17))
- **Address routing:** turn the flat 64 KB array into an address decoder that sends each range to its owner: cartridge, VRAM, WRAM, echo RAM, OAM, I/O, HRAM and `IE`.
- **Post-boot state:** start in the state the boot ROM leaves behind (`AF=01B0`, `BC=0013`, `DE=00D8`, `HL=014D`, `SP=FFFE`, `PC=0100`, plus the I/O registers).

### 3. Cartridge and MBC3 ([#18](https://github.com/Sam-Coburn/gameboy_emulator/issues/18))
- **Loading:** load the ROM and parse the header.
- **MBC3:** writes to ROM addresses are bank-switching commands (RAM enable, ROM bank, RAM bank, RTC latch).
- **Saves:** battery-backed RAM is saved to and loaded from a `.sav` file.

### 4. Timers ([#19](https://github.com/Sam-Coburn/gameboy_emulator/issues/19))
- **Registers:** `DIV`, `TIMA`, `TMA` and `TAC`, plus the timer interrupt.
- **Sync pattern:** the first component to follow the "tick by N cycles" pattern.

### 5. PPU ([#20](https://github.com/Sam-Coburn/gameboy_emulator/issues/20))
- **Line timing:** 456 dots per scanline across 154 lines, with modes 2, 3, 0 and 1.
- **Registers:** `LCDC`, `STAT`, scroll, `LY`/`LYC`, palettes and window position.
- **Scanline rendering:** background, window, then sprites.
- **Also:** the V-Blank and STAT interrupts, and OAM DMA at `FF46`.

### 6. Frontend ([#21](https://github.com/Sam-Coburn/gameboy_emulator/issues/21))
- **SDL2 window:** shows the framebuffer, scaled up.
- **Joypad:** keyboard input feeds the joypad register at `FF00`.
- **Main loop:** run 70,224 cycles per frame and pace to about 59.7 fps.

### 7. Serial ([#22](https://github.com/Sam-Coburn/gameboy_emulator/issues/22))
- **Test output:** capture `SB`/`SC` output so Blargg's test ROMs print results to the terminal. Do this early, since it allows CPU testing before graphics exist.
- **Pokémon:** a "no cable connected" stub is all it needs.

### 8. APU ([#23](https://github.com/Sam-Coburn/gameboy_emulator/issues/23))
- **Channels:** two square waves (one with sweep), a wave channel and a noise channel.
- **Timing:** a frame sequencer drives length, sweep and envelope.
- **Output:** mixing, resampling to 44.1 or 48 kHz, and SDL audio. Done last.

## Recommended order

Each step makes the next one testable, so you're never writing a lot of code you can't check:

1. ✅ CPU opcodes (#16)
2. ✅ Serial capture (#22)
3. ✅ **Blargg `cpu_instrs` passes** (all individual ROMs except `02-interrupts`, which needs the timer)
4. ✅ Cycle counts and interrupts (#16)
5. ✅ Bus refactor (#17)
6. **Next:** cartridge loading, no-MBC (#18)
7. Timers (#19), then **Blargg `instr_timing` passes**
8. PPU (#20)
9. Frontend and joypad (#21), then **`dmg-acid2` renders and Tetris is playable**
10. MBC3 and saves (#18)
11. **Pokémon Blue**
12. APU (#23)

## Milestones

| Milestone | What it proves | Status |
|---|---|---|
| Blargg `cpu_instrs` (output through serial) | Every CPU instruction is correct | 10/11 individual ROMs pass; `02-interrupts` waits on #19 |
| Blargg `instr_timing` | Cycle counts and timers are correct | Not yet |
| [`dmg-acid2`](https://github.com/mattcurrie/dmg-acid2) | The PPU renders background, window and sprites correctly | Not yet |
| Tetris (32 KB, no MBC) | The whole system works together on a simple game | Not yet |
| Pokémon Blue boots to the title screen | MBC3 banking, interrupts and PPU basics work | Not yet |
| Pokémon Blue: overworld, menus, a battle, then save, quit and load | Everything Pokémon needs is in place | Not yet |
| Pokémon Blue with music and cries | The APU works, and the emulator is complete | Not yet |

## Code changes to plan for
- ✅ **Cycle counts:** `step()` returns the T-cycles each instruction took (done in #16).
- **Header-only code:** the new components and the bus are split into `.cpp` files in a `gameboy_core` library. `cpu.hpp` is still header-only and can move to `cpu.cpp` later.
- ✅ **Memory initialisation:** every RAM array zero-initialises itself (#17).
- ✅ **Serial special case:** moved into its own `Serial` component (#17).

## Resources
- [Pan Docs](https://gbdev.io/pandocs/): the main hardware reference
- [gbdev opcode table](https://gbdev.io/gb-opcodes/optables/): opcodes, cycle counts, flags
- [Blargg test ROMs](https://github.com/retrio/gb-test-roms)
- [dmg-acid2](https://github.com/mattcurrie/dmg-acid2)
- See also [`docs/resources.txt`](resources.txt)
