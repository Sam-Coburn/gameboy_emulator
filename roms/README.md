# Test ROMs

ROM files are gitignored. To run the test-ROM tests, clone Blargg's test ROMs here:

```sh
git clone https://github.com/retrio/gb-test-roms roms/gb-test-roms
```

Run one ROM headless and print its serial output:

```sh
./build/gameboy "roms/gb-test-roms/cpu_instrs/individual/01-special.gb"
```

Only the individual `cpu_instrs` ROMs (32 KB each) load for now. The combined `cpu_instrs.gb` is 64 KB and needs MBC1 bank switching (#18).
