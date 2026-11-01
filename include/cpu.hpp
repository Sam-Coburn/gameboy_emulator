#include <cycles.hpp>
#include <instruction.hpp>
#include <memorybus.hpp>
#include <registers.hpp>

#include <bit>
#include <bitset>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <stdint.h>
#include <string>

class CPU {
    public:
        Registers registers;
        MemoryBus bus;

        uint16_t pc = 0x100;
        uint16_t sp = 0xFFFE;

        // Set by execute() when a conditional JP/JR/CALL/RET takes its branch, so step()
        // can add the extra cycles a taken branch costs
        bool branch_taken = false;

        bool ime = false; // Interrupt Master Enable
        bool ei_delayed = false; // Set by EI: turn IME on after the next instruction
        bool halted = false; // Set by HALT: CPU is asleep until an interrupt is pending

        static constexpr uint16_t IE_ADDRESS = 0xFFFF; // Interrupt Enable register
        static constexpr uint16_t IF_ADDRESS = 0xFF0F; // Interrupt Flag register

        // Executes the instruction at pc and returns the number of T-cycles it took
        uint8_t step() {
            uint8_t pending = pending_interrupts();

            // HALT sleeps until an interrupt is pending, even when IME is off. If IME is off
            // the CPU wakes up and continues with the next instruction without servicing it
            if (halted) {
                if (!pending) {
                    return 4;
                }
                halted = false;
            }

            // Interrupts are serviced between instructions instead of fetching the next one
            if (ime && pending) {
                service_interrupt(pending);
                return 20;
            }

            // Use program counter to read instruction byte from memory
            uint8_t instruction_byte = bus.read_byte(pc);
            bool prefixed = instruction_byte == 0xCB;

            if (prefixed) {
                instruction_byte = bus.read_byte(pc + 1);
            }

            // Decode instruction byte into an instruction
            std::optional<Instruction> opt_instruct = from_byte(instruction_byte, prefixed);
            if (opt_instruct.has_value()) {
                Instruction instruct = opt_instruct.value();

                branch_taken = false;

                // EI turns IME on only after the instruction following it, so only apply a
                // delayed enable that was already pending before this instruction ran
                bool ei_pending = ei_delayed;

                execute(instruct);

                // DI clears ei_delayed, so an EI directly followed by DI leaves IME off
                if (ei_pending && ei_delayed) {
                    ime = true;
                    ei_delayed = false;
                }

                if (prefixed) {
                    return PREFIXED_CYCLES[instruction_byte];
                }

                uint8_t cycles = UNPREFIXED_CYCLES[instruction_byte];
                if (branch_taken) {
                    cycles = cycles + branch_taken_extra_cycles(instruction_byte);
                }

                return cycles;
            }
            else {
                std::string description = "";

                if (prefixed) {
                    description = "Unkown instruction found for: 0xCB" + std::to_string(instruction_byte);
                }
                else {
                    description = "Unkown instruction found for: 0x" + std::to_string(instruction_byte);
                }

                throw std::runtime_error("Unkown instruction found for: 0x" + std::to_string(instruction_byte));
            }
        }

        void execute(const Instruction& instruction) {
            switch (instruction.type) {

                /* 8-bit Arithmetic Operations */

                // ADD Instruction adds specified register contents to register A
                case InstructionType::ADD: {
                    uint8_t value = 0;
                    bool inc_pc_by_two = false;

                    switch (instruction.alu8.target) {
                        case Operand8::A:
                            value = registers.a;
                            break;
                        case Operand8::B:
                            value = registers.b;
                            break;
                        case Operand8::C:
                            value = registers.c;
                            break;
                        case Operand8::D:
                            value = registers.d;
                            break;
                        case Operand8::E:
                            value = registers.e;
                            break;
                        case Operand8::H:
                            value = registers.h;
                            break;
                        case Operand8::L:
                            value = registers.l;
                            break;
                        case Operand8::HL_PTR: {
                            uint16_t addr = registers.get_hl();
                            value = bus.read_byte(addr);
                            break;
                        }
                        case Operand8::NUM:
                            value = bus.read_byte(pc + 1);
                            inc_pc_by_two = true;
                            break;
                        default:
                            break;
                    }

                    uint8_t new_value = add(value);
                    registers.a = new_value;

                    if (inc_pc_by_two) {
                        pc = pc + 2;
                    }
                    else {
                        pc = pc + 1;
                    }

                    break;
                }

                // ADC Instruction adds specified register contents + current carry flag value to register A
                case InstructionType::ADC: {
                    uint8_t value = 0;
                    bool inc_pc_by_two = false;

                    switch (instruction.alu8.target) {
                        case Operand8::A:
                            value = registers.a;
                            break;
                        case Operand8::B:
                            value = registers.b;
                            break;
                        case Operand8::C:
                            value = registers.c;
                            break;
                        case Operand8::D:
                            value = registers.d;
                            break;
                        case Operand8::E:
                            value = registers.e;
                            break;
                        case Operand8::H:
                            value = registers.h;
                            break;
                        case Operand8::L:
                            value = registers.l;
                            break;
                        case Operand8::HL_PTR: {
                            uint16_t addr = registers.get_hl();
                            value = bus.read_byte(addr);
                            break;
                        }
                        case Operand8::NUM:
                            value = bus.read_byte(pc + 1);
                            inc_pc_by_two = true;
                            break;
                        default:
                            break;
                    }

                    uint8_t new_value = adc(value);
                    registers.a = new_value;

                    if (inc_pc_by_two) {
                        pc = pc + 2;
                    }
                    else {
                        pc = pc + 1;
                    }

                    break;
                }

                // SUB Instruction subtracts specfified register contents from register A
                case InstructionType::SUB: {
                    uint8_t value = 0;
                    bool inc_pc_by_two = false;

                    switch (instruction.alu8.target) {
                        case Operand8::A:
                            value = registers.a;
                            break;
                        case Operand8::B:
                            value = registers.b;
                            break;
                        case Operand8::C:
                            value = registers.c;
                            break;
                        case Operand8::D:
                            value = registers.d;
                            break;
                        case Operand8::E:
                            value = registers.e;
                            break;
                        case Operand8::H:
                            value = registers.h;
                            break;
                        case Operand8::L:
                            value = registers.l;
                            break;
                        case Operand8::HL_PTR: {
                            uint16_t addr = registers.get_hl();
                            value = bus.read_byte(addr);
                            break;
                        }
                        case Operand8::NUM:
                            value = bus.read_byte(pc + 1);
                            inc_pc_by_two = true;
                            break;
                        default:
                            break;
                    }

                    uint8_t new_value = sub(value);
                    registers.a = new_value;

                    if (inc_pc_by_two) {
                        pc = pc + 2;
                    }
                    else {
                        pc = pc + 1;
                    }

                    break;
                }

                // SBC Instruction substracts specified register contents + carry flag from register A
                case InstructionType::SBC: {
                    uint8_t value = 0;
                    bool inc_pc_by_two = false;

                    switch (instruction.alu8.target) {
                        case Operand8::A:
                            value = registers.a;
                            break;
                        case Operand8::B:
                            value = registers.b;
                            break;
                        case Operand8::C:
                            value = registers.c;
                            break;
                        case Operand8::D:
                            value = registers.d;
                            break;
                        case Operand8::E:
                            value = registers.e;
                            break;
                        case Operand8::H:
                            value = registers.h;
                            break;
                        case Operand8::L:
                            value = registers.l;
                            break;
                        case Operand8::HL_PTR: {
                            uint16_t addr = registers.get_hl();
                            value = bus.read_byte(addr);
                            break;
                        }
                        case Operand8::NUM:
                            value = bus.read_byte(pc + 1);
                            inc_pc_by_two = true;
                            break;
                        default:
                            break;
                    }

                    uint8_t new_value = sbc(value);
                    registers.a = new_value;

                    if (inc_pc_by_two) {
                        pc = pc + 2;
                    }
                    else {
                        pc = pc + 1;
                    }

                    break;
                }

                // AND Instruction performs bitwise AND operation using specified register contents with register A
                case InstructionType::AND: {
                    uint8_t value = 0;
                    bool inc_pc_by_two = false;

                    switch (instruction.alu8.target) {
                        case Operand8::A:
                            value = registers.a;
                            break;
                        case Operand8::B:
                            value = registers.b;
                            break;
                        case Operand8::C:
                            value = registers.c;
                            break;
                        case Operand8::D:
                            value = registers.d;
                            break;
                        case Operand8::E:
                            value = registers.e;
                            break;
                        case Operand8::H:
                            value = registers.h;
                            break;
                        case Operand8::L:
                            value = registers.l;
                            break;
                        case Operand8::HL_PTR: {
                            uint16_t addr = registers.get_hl();
                            value = bus.read_byte(addr);
                            break;
                        }
                        case Operand8::NUM:
                            value = bus.read_byte(pc + 1);
                            inc_pc_by_two = true;
                            break;
                        default:
                            break;
                    }

                    uint8_t new_value = bitwise_and(value);
                    registers.a = new_value;

                    if (inc_pc_by_two) {
                        pc = pc + 2;
                    }
                    else {
                        pc = pc + 1;
                    }

                    break;
                }

                // OR Instruction performs bitwise OR operation using specified register contents with register A
                case InstructionType::OR: {
                    uint8_t value = 0;
                    bool inc_pc_by_two = false;

                    switch (instruction.alu8.target) {
                        case Operand8::A:
                            value = registers.a;
                            break;
                        case Operand8::B:
                            value = registers.b;
                            break;
                        case Operand8::C:
                            value = registers.c;
                            break;
                        case Operand8::D:
                            value = registers.d;
                            break;
                        case Operand8::E:
                            value = registers.e;
                            break;
                        case Operand8::H:
                            value = registers.h;
                            break;
                        case Operand8::L:
                            value = registers.l;
                            break;
                        case Operand8::HL_PTR: {
                            uint16_t addr = registers.get_hl();
                            value = bus.read_byte(addr);
                            break;
                        }
                        case Operand8::NUM:
                            value = bus.read_byte(pc + 1);
                            inc_pc_by_two = true;
                            break;
                        default:
                            break;
                    }

                    uint8_t new_value = bitwise_or(value);
                    registers.a = new_value;

                    if (inc_pc_by_two) {
                        pc = pc + 2;
                    }
                    else {
                        pc = pc + 1;
                    }

                    break;
                }

                // XOR Instruction performs bitwise XOR operation using specified register contents with register A
                case InstructionType::XOR: {
                    uint8_t value = 0;
                    bool inc_pc_by_two = false;

                    switch (instruction.alu8.target) {
                        case Operand8::A:
                            value = registers.a;
                            break;
                        case Operand8::B:
                            value = registers.b;
                            break;
                        case Operand8::C:
                            value = registers.c;
                            break;
                        case Operand8::D:
                            value = registers.d;
                            break;
                        case Operand8::E:
                            value = registers.e;
                            break;
                        case Operand8::H:
                            value = registers.h;
                            break;
                        case Operand8::L:
                            value = registers.l;
                            break;
                        case Operand8::HL_PTR: {
                            uint16_t addr = registers.get_hl();
                            value = bus.read_byte(addr);
                            break;
                        }
                        case Operand8::NUM:
                            value = bus.read_byte(pc + 1);
                            inc_pc_by_two = true;
                            break;
                        default:
                            break;
                    }

                    uint8_t new_value = bitwise_xor(value);
                    registers.a = new_value;

                    if (inc_pc_by_two) {
                        pc = pc + 2;
                    }
                    else {
                        pc = pc + 1;
                    }

                    break;
                }

                // CP Instruction subtracts specfified register contents from register A but doesnt store the result
                case InstructionType::CP: {
                    uint8_t value = 0;
                    bool inc_pc_by_two = false;

                    switch (instruction.alu8.target) {
                        case Operand8::A:
                            value = registers.a;
                            break;
                        case Operand8::B:
                            value = registers.b;
                            break;
                        case Operand8::C:
                            value = registers.c;
                            break;
                        case Operand8::D:
                            value = registers.d;
                            break;
                        case Operand8::E:
                            value = registers.e;
                            break;
                        case Operand8::H:
                            value = registers.h;
                            break;
                        case Operand8::L:
                            value = registers.l;
                            break;
                        case Operand8::HL_PTR: {
                            uint16_t addr = registers.get_hl();
                            value = bus.read_byte(addr);
                            break;
                        }
                        case Operand8::NUM:
                            value = bus.read_byte(pc + 1);
                            inc_pc_by_two = true;
                            break;
                        default:
                            break;
                    }

                    uint8_t new_value = sub(value);

                    if (inc_pc_by_two) {
                        pc = pc + 2;
                    }
                    else {
                        pc = pc + 1;
                    }

                    break;
                }

                // INC Instruction increments value of target register
                case InstructionType::INC: {
                    switch (instruction.alu8.target) {
                        case Operand8::A:
                            registers.a = inc(registers.a);
                            break;
                        case Operand8::B:
                            registers.b = inc(registers.b);
                            break;
                        case Operand8::C:
                            registers.c = inc(registers.c);
                            break;
                        case Operand8::D:
                            registers.d = inc(registers.d);
                            break;
                        case Operand8::E:
                            registers.e = inc(registers.e);
                            break;
                        case Operand8::H:
                            registers.h = inc(registers.h);
                            break;
                        case Operand8::L:
                            registers.l = inc(registers.l);
                            break;
                        case Operand8::HL_PTR: {
                            uint16_t addr = registers.get_hl();
                            uint8_t value = bus.read_byte(addr);
                            uint8_t result = inc(value);

                            bus.write_byte(addr, result);

                            break;
                        }
                        case Operand8::NUM:
                            break;
                        default:
                            break;
                    }

                    pc = pc + 1;

                    break;
                }

                // DEC Instruction increments value of target register
                case InstructionType::DEC: {
                    switch (instruction.alu8.target) {
                        case Operand8::A:
                            registers.a = dec(registers.a);
                            break;
                        case Operand8::B:
                            registers.b = dec(registers.b);
                            break;
                        case Operand8::C:
                            registers.c = dec(registers.c);
                            break;
                        case Operand8::D:
                            registers.d = dec(registers.d);
                            break;
                        case Operand8::E:
                            registers.e = dec(registers.e);
                            break;
                        case Operand8::H:
                            registers.h = dec(registers.h);
                            break;
                        case Operand8::L:
                            registers.l = dec(registers.l);
                            break;
                        case Operand8::HL_PTR: {
                            uint16_t addr = registers.get_hl();
                            uint8_t value = bus.read_byte(addr);
                            uint8_t result = dec(value);

                            bus.write_byte(addr, result);

                            break;
                        }
                        case Operand8::NUM:
                            break;
                        default:
                            break;
                    }

                    pc = pc + 1;

                    break;
                }

                /* Misc CPU Instructions */

                // SWAP Instruction swaps the values of the upper and lower nibbles of the target register
                case InstructionType::SWAP: {
                    switch (instruction.alu8.target) {
                        case Operand8::A:
                            registers.a = swap(registers.a);
                            break;
                        case Operand8::B:
                            registers.b = swap(registers.b);
                            break;
                        case Operand8::C:
                            registers.c = swap(registers.c);
                            break;
                        case Operand8::D:
                            registers.d = swap(registers.d);
                            break;
                        case Operand8::E:
                            registers.e = swap(registers.e);
                            break;
                        case Operand8::H:
                            registers.h = swap(registers.h);
                            break;
                        case Operand8::L:
                            registers.l = swap(registers.l);
                            break;
                        case Operand8::HL_PTR: {
                            uint16_t addr = registers.get_hl();
                            uint8_t value = bus.read_byte(addr);
                            uint8_t result = swap(value);

                            bus.write_byte(addr, result);

                            break;
                        }
                        case Operand8::NUM:
                            break;
                        default:
                            break;
                    }

                    pc = pc + 2;

                    break;
                }

                // SCF Instruction sets the carry flag, i.e. changes its value to True
                case InstructionType::SCF: {
                    registers.f.carry = true;

                    registers.f.zero = registers.f.zero; // SCF instruction leaves zero flag unaffected
                    registers.f.subtract = false;
                    registers.f.half_carry = false;

                    pc = pc + 1;

                    break;
                }

                // CCF Instruction toggles the carry flag, e.g. C = True --> False, C = False --> True
                case InstructionType::CCF: {
                    if (registers.f.carry) {
                        registers.f.carry = false;
                    }
                    else {
                        registers.f.carry = true;
                    }

                    registers.f.zero = registers.f.zero; // CCF instruction leaves zero flag unaffected
                    registers.f.subtract = false;
                    registers.f.half_carry = false;

                    pc = pc + 1;

                    break;
                }

                // CPL Instruction toggles every bit of register A
                case InstructionType::CPL: {
                    registers.a = ~registers.a;

                    registers.f.zero = registers.f.zero; // CPL instruction leaves zero flag unaffected
                    registers.f.subtract = true;
                    registers.f.carry = registers.f.carry; // CPL instruction leaves carry flag unaffected
                    registers.f.half_carry = true;

                    pc = pc + 1;

                    break;
                }

                // DAA Instruction adjusts register A into a valid BCD (binary-coded decimal) number
                // after an addition or subtraction, using the subtract, half carry and carry flags
                case InstructionType::DAA: {
                    uint8_t value = registers.a;

                    if (!registers.f.subtract) {
                        // After an addition, correct each nibble that went above 9 (or carried out)
                        if (registers.f.carry || value > 0x99) {
                            value = value + 0x60;
                            registers.f.carry = true;
                        }
                        if (registers.f.half_carry || (value & 0xF) > 0x9) {
                            value = value + 0x06;
                        }
                    }
                    else {
                        // After a subtraction, only undo the borrows that actually happened
                        if (registers.f.carry) {
                            value = value - 0x60;
                        }
                        if (registers.f.half_carry) {
                            value = value - 0x06;
                        }
                    }

                    registers.a = value;

                    registers.f.zero = value == 0;
                    registers.f.subtract = registers.f.subtract; // DAA leaves subtract flag unaffected
                    registers.f.half_carry = false;

                    pc = pc + 1;

                    break;
                }

                // NOP Instruction does nothing
                case InstructionType::NOP: {
                    pc = pc + 1;

                    break;
                }

                // HALT Instruction suspends the CPU until an interrupt is pending
                case InstructionType::HALT: {
                    halted = true;

                    pc = pc + 1;

                    break;
                }

                // STOP Instruction enters very low power mode until a button is pressed. It is
                // 2 bytes wide (0x10 0x00) and isn't needed by most games, so it's treated as a NOP
                case InstructionType::STOP: {
                    pc = pc + 2;

                    break;
                }

                // DI Instruction disables interrupts
                case InstructionType::DI: {
                    ime = false;
                    ei_delayed = false;

                    pc = pc + 1;

                    break;
                }

                // EI Instruction enables interrupts after the following instruction
                case InstructionType::EI: {
                    ei_delayed = true;

                    pc = pc + 1;

                    break;
                }

                /* Bitwise CPU Instructions */

                // BIT Instruction tests the bit value for the given register at the given bit index
                case InstructionType::BIT: {
                    uint8_t bit_index = instruction.bitop.bit_index;
                    uint8_t value = 0;

                    switch (instruction.bitop.target) {
                        case Operand8::A:
                            value = registers.a;
                            break;
                        case Operand8::B:
                            value = registers.b;
                            break;
                        case Operand8::C:
                            value = registers.c;
                            break;
                        case Operand8::D:
                            value = registers.d;
                            break;
                        case Operand8::E:
                            value = registers.e;
                            break;
                        case Operand8::H:
                            value = registers.h;
                            break;
                        case Operand8::L:
                            value = registers.l;
                            break;
                        case Operand8::HL_PTR: {
                            uint16_t addr = registers.get_hl();
                            value = bus.read_byte(addr);
                            break;
                        }
                        case Operand8::NUM:
                            break;
                        default:
                            break;
                    }

                    uint8_t bit_value = (value >> bit_index) & (0b00000001);

                    registers.f.zero = bit_value == 0;
                    registers.f.subtract = false;
                    registers.f.carry = registers.f.carry; // BIT instruction leaves carry flag unaffected
                    registers.f.half_carry = true;

                    pc = pc + 2;

                    break;
                }

                // SET Instruction sets the bit value for the given register at the given bit index to 1
                case InstructionType::SET: {
                    uint8_t bit_index = instruction.bitop.bit_index;

                    switch (instruction.bitop.target) {
                        case Operand8::A:
                            registers.a = registers.a | (1 << bit_index);
                            break;
                        case Operand8::B:
                            registers.b = registers.b | (1 << bit_index);
                            break;
                        case Operand8::C:
                            registers.c = registers.c | (1 << bit_index);
                            break;
                        case Operand8::D:
                            registers.d = registers.d | (1 << bit_index);
                            break;
                        case Operand8::E:
                            registers.e = registers.e | (1 << bit_index);
                            break;
                        case Operand8::H:
                            registers.h = registers.h | (1 << bit_index);
                            break;
                        case Operand8::L:
                            registers.l = registers.l | (1 << bit_index);
                            break;
                        case Operand8::HL_PTR: {
                            uint16_t addr = registers.get_hl();
                            uint8_t value = bus.read_byte(addr);

                            uint8_t result = value | (1 << bit_index);

                            bus.write_byte(addr, result);

                            break;
                        }
                        case Operand8::NUM:
                            break;
                        default:
                            break;
                    }

                    pc = pc + 2;

                    break;
                }

                // RES Instruction sets the bit value for the given register at the given bit index to 0
                case InstructionType::RES: {
                    uint8_t bit_index = instruction.bitop.bit_index;

                    switch (instruction.bitop.target) {
                        case Operand8::A:
                            registers.a = registers.a & ~(1 << bit_index);
                            break;
                        case Operand8::B:
                            registers.b = registers.b & ~(1 << bit_index);
                            break;
                        case Operand8::C:
                            registers.c = registers.c & ~(1 << bit_index);
                            break;
                        case Operand8::D:
                            registers.d = registers.d & ~(1 << bit_index);
                            break;
                        case Operand8::E:
                            registers.e = registers.e & ~(1 << bit_index);
                            break;
                        case Operand8::H:
                            registers.h = registers.h & ~(1 << bit_index);
                            break;
                        case Operand8::L:
                            registers.l = registers.l & ~(1 << bit_index);
                            break;
                        case Operand8::HL_PTR: {
                            uint16_t addr = registers.get_hl();
                            uint8_t value = bus.read_byte(addr);

                            uint8_t result = value & ~(1 << bit_index);

                            bus.write_byte(addr, result);

                            break;
                        }
                        case Operand8::NUM:
                            break;
                        default:
                            break;
                    }

                    pc = pc + 2;

                    break;
                }

                /* 16-bit Arithmetic Operations */

                // ADDHL Instruction adds the contents of the given 16-bit register to the HL register
                case InstructionType::ADDHL: {
                    uint16_t value = 0;

                    switch (instruction.alu16.target) {
                        case Operand16::BC:
                            value = registers.get_bc();
                            break;
                        case Operand16::DE:
                            value = registers.get_de();
                            break;
                        case Operand16::HL:
                            value = registers.get_hl();
                            break;
                        case Operand16::SP:
                            value = sp;
                            break;
                    }

                    uint16_t new_value = add_hl(value);
                    registers.set_hl(new_value);

                    pc = pc + 1;

                    break;
                }

                // INC16 Instruction increments a 16-bit register, no flags are affected
                case InstructionType::INC16: {
                    switch (instruction.alu16.target) {
                        case Operand16::BC:
                            registers.set_bc(registers.get_bc() + 1);
                            break;
                        case Operand16::DE:
                            registers.set_de(registers.get_de() + 1);
                            break;
                        case Operand16::HL:
                            registers.set_hl(registers.get_hl() + 1);
                            break;
                        case Operand16::SP:
                            sp = sp + 1;
                            break;
                    }

                    pc = pc + 1;

                    break;
                }

                // DEC16 Instruction decrements a 16-bit register, no flags are affected
                case InstructionType::DEC16: {
                    switch (instruction.alu16.target) {
                        case Operand16::BC:
                            registers.set_bc(registers.get_bc() - 1);
                            break;
                        case Operand16::DE:
                            registers.set_de(registers.get_de() - 1);
                            break;
                        case Operand16::HL:
                            registers.set_hl(registers.get_hl() - 1);
                            break;
                        case Operand16::SP:
                            sp = sp - 1;
                            break;
                    }

                    pc = pc + 1;

                    break;
                }

                // ADDSP Instruction adds a signed 8-bit offset to SP
                case InstructionType::ADDSP: {
                    sp = add_sp_offset();

                    pc = pc + 2;

                    break;
                }

                /* Rotate & Shift CPU Instructions */

                // RLCA Instruction rotates the contents of register A 1 bit to the left, the carry flag is set to old bit 7
                case InstructionType::RLCA: {
                    uint8_t value = registers.a;
                    bool new_carry = (value & 0b10000000) >> 7;

                    value = (value << 1) | (new_carry);

                    registers.a = value;

                    registers.f.zero = registers.a == 0;
                    registers.f.subtract = false;
                    registers.f.carry = new_carry;
                    registers.f.half_carry = false;

                    pc = pc + 1;

                    break;
                }

                // RLA Instruction rotates the contents of register A 1 bit to the left, the carry flag is set to old bit 7, bit 0 is set to old carry flag value
                case InstructionType::RLA: {
                    uint8_t value = registers.a;
                    bool old_carry = registers.f.carry;
                    bool new_carry = (value & 0b10000000) >> 7;

                    value = (value << 1) | (old_carry);

                    registers.a = value;

                    registers.f.zero = registers.a == 0;
                    registers.f.subtract = false;
                    registers.f.carry = new_carry;
                    registers.f.half_carry = false;

                    pc = pc + 1;

                    break;
                }

                // RRCA Instruction rotates the contents of register A 1 bit to the right, the carry flag is set to old bit 0
                case InstructionType::RRCA: {
                    uint8_t value = registers.a;
                    bool new_carry = value & 1;

                    value = (value >> 1) | (new_carry << 7);

                    registers.a = value;

                    registers.f.zero = registers.a == 0;
                    registers.f.subtract = false;
                    registers.f.carry = new_carry;
                    registers.f.half_carry = false;

                    pc = pc + 1;

                    break;
                }

                // RRA Instruction rotates the contents of register A 1 bit to the right, the carry flag is rotated in for bit 7 (leftmost bit) and bit 0 is rotated out to the carry flag value
                case InstructionType::RRA: {
                    uint8_t value = registers.a;
                    uint8_t old_carry = registers.f.carry;
                    bool new_carry = value & 1;

                    value = (value >> 1) | (old_carry << 7);

                    registers.a = value;

                    registers.f.zero = registers.a == 0;
                    registers.f.subtract = false;
                    registers.f.carry = new_carry;
                    registers.f.half_carry = false;

                    pc = pc + 1;

                    break;
                }

                // RLC Instruction rotates the contents of the proviced register 1 bit to the left, the carry flag is set to old bit 7
                case InstructionType::RLC: {
                    switch (instruction.alu8.target) {
                        case Operand8::A:
                            registers.a = rlc(registers.a);
                            break;
                        case Operand8::B:
                            registers.b = rlc(registers.b);
                            break;
                        case Operand8::C:
                            registers.c = rlc(registers.c);
                            break;
                        case Operand8::D:
                            registers.d = rlc(registers.d);
                            break;
                        case Operand8::E:
                            registers.e = rlc(registers.e);
                            break;
                        case Operand8::H:
                            registers.h = rlc(registers.h);
                            break;
                        case Operand8::L:
                            registers.l = rlc(registers.l);
                            break;
                        case Operand8::HL_PTR: {
                            uint16_t addr = registers.get_hl();
                            uint8_t value = bus.read_byte(addr);
                            uint8_t result = rlc(value);

                            bus.write_byte(addr, result);

                            break;
                        }
                        case Operand8::NUM:
                            break;
                        default:
                            break;
                    }

                    pc = pc + 2;

                    break;
                }

                // RL Instruction rotates the contents of the provided register 1 bit to the left, the carry flag is set to old bit 7, bit 0 is set to old carry flag value
                case InstructionType::RL: {
                    switch (instruction.alu8.target) {
                        case Operand8::A:
                            registers.a = rl(registers.a);
                            break;
                        case Operand8::B:
                            registers.b = rl(registers.b);
                            break;
                        case Operand8::C:
                            registers.c = rl(registers.c);
                            break;
                        case Operand8::D:
                            registers.d = rl(registers.d);
                            break;
                        case Operand8::E:
                            registers.e = rl(registers.e);
                            break;
                        case Operand8::H:
                            registers.h = rl(registers.h);
                            break;
                        case Operand8::L:
                            registers.l = rl(registers.l);
                            break;
                        case Operand8::HL_PTR: {
                            uint16_t addr = registers.get_hl();
                            uint8_t value = bus.read_byte(addr);
                            uint8_t result = rl(value);

                            bus.write_byte(addr, result);

                            break;
                        }
                        case Operand8::NUM:
                            break;
                        default:
                            break;
                    }

                    pc = pc + 2;

                    break;
                }

                // RRC Instruction rotates the contents of the provided register 1 bit to the right, the carry flag is set to old bit 0
                case InstructionType::RRC: {
                    switch (instruction.alu8.target) {
                        case Operand8::A:
                            registers.a = rrc(registers.a);
                            break;
                        case Operand8::B:
                            registers.b = rrc(registers.b);
                            break;
                        case Operand8::C:
                            registers.c = rrc(registers.c);
                            break;
                        case Operand8::D:
                            registers.d = rrc(registers.d);
                            break;
                        case Operand8::E:
                            registers.e = rrc(registers.e);
                            break;
                        case Operand8::H:
                            registers.h = rrc(registers.h);
                            break;
                        case Operand8::L:
                            registers.l = rrc(registers.l);
                            break;
                        case Operand8::HL_PTR: {
                            uint16_t addr = registers.get_hl();
                            uint8_t value = bus.read_byte(addr);
                            uint8_t result = rrc(value);

                            bus.write_byte(addr, result);

                            break;
                        }
                        case Operand8::NUM:
                            break;
                        default:
                            break;
                    }

                    pc = pc + 2;

                    break;
                }

                // RR Instruction rotates the contents of the provided register 1 bit to the right, the carry flag is rotated in for bit 7 (leftmost bit) and bit 0 is rotated out to the carry flag value
                case InstructionType::RR: {
                    switch (instruction.alu8.target) {
                        case Operand8::A:
                            registers.a = rr(registers.a);
                            break;
                        case Operand8::B:
                            registers.b = rr(registers.b);
                            break;
                        case Operand8::C:
                            registers.c = rr(registers.c);
                            break;
                        case Operand8::D:
                            registers.d = rr(registers.d);
                            break;
                        case Operand8::E:
                            registers.e = rr(registers.e);
                            break;
                        case Operand8::H:
                            registers.h = rr(registers.h);
                            break;
                        case Operand8::L:
                            registers.l = rr(registers.l);
                            break;
                        case Operand8::HL_PTR: {
                            uint16_t addr = registers.get_hl();
                            uint8_t value = bus.read_byte(addr);
                            uint8_t result = rr(value);

                            bus.write_byte(addr, result);

                            break;
                        }
                        case Operand8::NUM:
                            break;
                        default:
                            break;
                    }

                    pc = pc + 2;

                    break;
                }

                // SLA Instruction shifts the value of the provided register to the left by 1 bit and stores old bit 7 in carry
                case InstructionType::SLA: {
                    switch (instruction.alu8.target) {
                        case Operand8::A:
                            registers.a = sla(registers.a);
                            break;
                        case Operand8::B:
                            registers.b = sla(registers.b);
                            break;
                        case Operand8::C:
                            registers.c = sla(registers.c);
                            break;
                        case Operand8::D:
                            registers.d = sla(registers.d);
                            break;
                        case Operand8::E:
                            registers.e = sla(registers.e);
                            break;
                        case Operand8::H:
                            registers.h = sla(registers.h);
                            break;
                        case Operand8::L:
                            registers.l = sla(registers.l);
                            break;
                        case Operand8::HL_PTR: {
                            uint16_t addr = registers.get_hl();
                            uint8_t value = bus.read_byte(addr);
                            uint8_t result = sla(value);

                            bus.write_byte(addr, result);

                            break;
                        }
                        case Operand8::NUM:
                            break;
                        default:
                            break;
                    }

                    pc = pc + 2;

                    break;
                }

                // SRA Instruction shifts the value of the provided register to the right by 1 bit and stores old bit 0 in carry, the MSB remains the same (don't shift in 0 automatically)
                case InstructionType::SRA: {
                    switch (instruction.alu8.target) {
                        case Operand8::A:
                            registers.a = sra(registers.a);
                            break;
                        case Operand8::B:
                            registers.b = sra(registers.b);
                            break;
                        case Operand8::C:
                            registers.c = sra(registers.c);
                            break;
                        case Operand8::D:
                            registers.d = sra(registers.d);
                            break;
                        case Operand8::E:
                            registers.e = sra(registers.e);
                            break;
                        case Operand8::H:
                            registers.h = sra(registers.h);
                            break;
                        case Operand8::L:
                            registers.l = sra(registers.l);
                            break;
                        case Operand8::HL_PTR: {
                            uint16_t addr = registers.get_hl();
                            uint8_t value = bus.read_byte(addr);
                            uint8_t result = sra(value);

                            bus.write_byte(addr, result);

                            break;
                        }
                        case Operand8::NUM:
                            break;
                        default:
                            break;
                    }

                    pc = pc + 2;

                    break;
                }

                // SRL Instruction shifts the value of the provided register to the right by 1 bit and stores old bit 0 in carry
                case InstructionType::SRL: {
                    switch (instruction.alu8.target) {
                        case Operand8::A:
                            registers.a = srl(registers.a);
                            break;
                        case Operand8::B:
                            registers.b = srl(registers.b);
                            break;
                        case Operand8::C:
                            registers.c = srl(registers.c);
                            break;
                        case Operand8::D:
                            registers.d = srl(registers.d);
                            break;
                        case Operand8::E:
                            registers.e = srl(registers.e);
                            break;
                        case Operand8::H:
                            registers.h = srl(registers.h);
                            break;
                        case Operand8::L:
                            registers.l = srl(registers.l);
                            break;
                        case Operand8::HL_PTR: {
                            uint16_t addr = registers.get_hl();
                            uint8_t value = bus.read_byte(addr);
                            uint8_t result = srl(value);

                            bus.write_byte(addr, result);

                            break;
                        }
                        case Operand8::NUM:
                            break;
                        default:
                            break;
                    }

                    pc = pc + 2;

                    break;
                }

                /* Jump Instructions */

                case InstructionType::JP: {
                    bool jump_condition = check_jump_condition(instruction.jump.condition);
                    branch_taken = jump_condition;

                    pc = jump(jump_condition);

                    break;
                }

                case InstructionType::JR: {
                    bool jump_condition = check_jump_condition(instruction.jump.condition);
                    branch_taken = jump_condition;

                    pc = jump_relative(jump_condition);

                    break;
                }

                case InstructionType::JP_HL: {
                    pc = registers.get_hl();

                    break;
                }

                /* Call and Return Instructions */

                // CALL Instruction pushes the address of the next instruction and jumps to nn
                case InstructionType::CALL: {
                    bool jump_condition = check_jump_condition(instruction.jump.condition);
                    branch_taken = jump_condition;

                    if (jump_condition) {
                        push(pc + 3); // CALL is 3 bytes wide, so return to the instruction after it
                        pc = read_immediate_word();
                    }
                    else {
                        pc = pc + 3;
                    }

                    break;
                }

                // RET Instruction pops the return address off the stack and jumps to it
                case InstructionType::RET: {
                    bool jump_condition = check_jump_condition(instruction.jump.condition);
                    branch_taken = jump_condition;

                    if (jump_condition) {
                        pc = pop();
                    }
                    else {
                        pc = pc + 1;
                    }

                    break;
                }

                // RETI Instruction returns from an interrupt handler and re-enables interrupts
                case InstructionType::RETI: {
                    pc = pop();
                    ime = true; // Unlike EI, RETI enables interrupts immediately

                    break;
                }

                // RST Instruction is a 1-byte CALL to one of 8 fixed addresses (0x00, 0x08, ..., 0x38)
                case InstructionType::RST: {
                    push(pc + 1);
                    pc = instruction.rst.address;

                    break;
                }

                /* Load Instructions */
                case InstructionType::LD_8: {
                    uint8_t source_value = 0;

                    switch (instruction.load8.source) {
                        case LoadSource::A:
                            source_value = registers.a;
                            break;
                        case LoadSource::B:
                            source_value = registers.b;
                            break;
                        case LoadSource::C:
                            source_value = registers.c;
                            break;
                        case LoadSource::D:
                            source_value = registers.d;
                            break;
                        case LoadSource::E:
                            source_value = registers.e;
                            break;
                        case LoadSource::H:
                            source_value = registers.h;
                            break;
                        case LoadSource::L:
                            source_value = registers.l;
                            break;
                        case LoadSource::HL_PTR: {
                            uint16_t addr = registers.get_hl();
                            source_value = bus.read_byte(addr);
                            break;
                        }
                        default:
                            break;
                    }

                    switch (instruction.load8.target) {
                        case LoadTarget::A:
                            registers.a = source_value;
                            break;
                        case LoadTarget::B:
                            registers.b = source_value;
                            break;
                        case LoadTarget::C:
                            registers.c = source_value;
                            break;
                        case LoadTarget::D:
                            registers.d = source_value;
                            break;
                        case LoadTarget::E:
                            registers.e = source_value;
                            break;
                        case LoadTarget::H:
                            registers.h = source_value;
                            break;
                        case LoadTarget::L:
                            registers.l = source_value;
                            break;
                        case LoadTarget::BC:
                            break;
                        case LoadTarget::DE:
                            break;
                        case LoadTarget::HL:
                            break;
                        case LoadTarget::SP:
                            break;
                        case LoadTarget::HL_PTR: {
                            uint16_t addr = registers.get_hl();
                            bus.write_byte(addr, source_value);
                            break;
                        }
                        default:
                            break;
                    }

                    pc = pc + 1;
                    break;
                }

                case InstructionType::LD_N: {
                    uint8_t value = bus.read_byte(pc + 1);

                    switch (instruction.load8.target) {
                        case LoadTarget::A:
                            registers.a = value;
                            break;
                        case LoadTarget::B:
                            registers.b = value;
                            break;
                        case LoadTarget::C:
                            registers.c = value;
                            break;
                        case LoadTarget::D:
                            registers.d = value;
                            break;
                        case LoadTarget::E:
                            registers.e = value;
                            break;
                        case LoadTarget::H:
                            registers.h = value;
                            break;
                        case LoadTarget::L:
                            registers.l = value;
                            break;
                        case LoadTarget::HL_PTR: {
                            uint16_t addr = registers.get_hl();
                            bus.write_byte(addr, value);
                            break;
                        }
                        default:
                            break;
                    }

                    pc = pc + 2;
                    break;
                }

                case InstructionType::LD_16: {
                    uint16_t value = read_immediate_word();

                    switch (instruction.load16.target) {
                        case Operand16::BC:
                            registers.set_bc(value);
                            break;
                        case Operand16::DE:
                            registers.set_de(value);
                            break;
                        case Operand16::HL:
                            registers.set_hl(value);
                            break;
                        case Operand16::SP:
                            sp = value;
                            break;
                        default:
                            break;
                    }

                    pc = pc + 3;
                    break;
                }

                // LD_A_IND loads the byte at a memory address into register A
                case InstructionType::LD_A_IND: {
                    uint16_t addr = indirect_address(instruction.load_ind.address);
                    registers.a = bus.read_byte(addr);
                    pc = pc + indirect_length(instruction.load_ind.address);
                    break;
                }

                // LD_IND_A stores register A at a memory address
                case InstructionType::LD_IND_A: {
                    uint16_t addr = indirect_address(instruction.load_ind.address);
                    bus.write_byte(addr, registers.a);
                    pc = pc + indirect_length(instruction.load_ind.address);
                    break;
                }

                case InstructionType::LD_SP_HL: {
                    sp = registers.get_hl();
                    pc = pc + 1;
                    break;
                }

                // LD HL,SP+e loads SP plus a signed 8-bit offset into HL
                case InstructionType::LD_HL_SP: {
                    registers.set_hl(add_sp_offset());

                    pc = pc + 2;
                    break;
                }

                // LD (nn),SP stores SP at address nn (little endian)
                case InstructionType::LD_NN_SP: {
                    uint16_t addr = read_immediate_word();
                    bus.write_byte(addr, static_cast<uint8_t>(sp & 0xFF));
                    bus.write_byte(addr + 1, static_cast<uint8_t>((sp & 0xFF00) >> 8));
                    pc = pc + 3;
                    break;
                }

                /* Stack Instructions */
                case InstructionType::PUSH: {
                    uint16_t value = 0;

                    switch (instruction.stack.target) {
                        case StackTarget::AF:
                            value = registers.get_af();
                            break;
                        case StackTarget::BC:
                            value = registers.get_bc();
                            break;
                        case StackTarget::DE:
                            value = registers.get_de();
                            break;
                        case StackTarget::HL:
                            value = registers.get_hl();
                            break;
                        default:
                            break;
                    }

                    push(value);
                    pc = pc + 1;
                    break;
                }

                case InstructionType::POP: {
                    uint16_t value = pop();

                    switch (instruction.stack.target) {
                        case StackTarget::AF:
                            registers.set_af(value);
                            break;
                        case StackTarget::BC:
                            registers.set_bc(value);
                            break;
                        case StackTarget::DE:
                            registers.set_de(value);
                            break;
                        case StackTarget::HL:
                            registers.set_hl(value);
                            break;
                        default:
                            break;
                    }

                    pc = pc + 1;
                    break;
                }

                default:
                    // TODO: support more instructions
                    break;
            }
        }

        uint8_t add(uint8_t value) {
            uint8_t result = registers.a + value;
            bool overflowed = result < registers.a;

            registers.f.zero = result == 0;
            registers.f.subtract = false;
            registers.f.carry = overflowed;

            // Half Carry is set if adding the lower nibbles of the value and register A
            // together result in a value bigger than 0xF. If the result is larger than 0xF
            // than the addition caused a carry from the lower nibble to the upper nibble.
            registers.f.half_carry = (registers.a & 0xF) + (value & 0xF) > 0xF;

            return result;
        }

        uint8_t adc(uint8_t value) {
            uint8_t result = registers.a + value + registers.f.carry;
            bool overflowed = result < registers.a;

            registers.f.zero = result == 0;
            registers.f.subtract = false;
            registers.f.carry = overflowed;
            registers.f.half_carry = (registers.a & 0xF) + (value & 0xF) > 0xF; // Same logic as for add()

            return result;
        }

        uint8_t sub(uint8_t value) {
            uint8_t result = registers.a - value;
            bool underflowed = value > registers.a;

            registers.f.zero = result == 0;
            registers.f.subtract = true;
            registers.f.carry = underflowed;
            registers.f.half_carry = (value & 0xF) > (registers.a & 0xF);

            return result;
        }

        uint8_t sbc(uint8_t value) {
            uint8_t result = registers.a - (value + registers.f.carry);
            bool underflowed = (value + registers.f.carry) > registers.a;

            registers.f.zero = result == 0;
            registers.f.subtract = true;
            registers.f.carry = underflowed;
            registers.f.half_carry = ((value + registers.f.carry) & 0xF) > (registers.a & 0xF);

            return result;
        }

        uint8_t bitwise_and(uint8_t value) {
            uint8_t result = registers.a & value;

            registers.f.zero = result == 0;
            registers.f.subtract = false;
            registers.f.carry = false;
            registers.f.half_carry = true;

            return result;
        }

        uint8_t bitwise_or(uint8_t value) {
            uint8_t result = registers.a & value;

            registers.f.zero = result == 0;
            registers.f.subtract = false;
            registers.f.carry = false;
            registers.f.half_carry = false;

            return result;
        }

        uint8_t bitwise_xor(uint8_t value) {
            uint8_t result = registers.a ^ value;

            registers.f.zero = result == 0;
            registers.f.subtract = false;
            registers.f.carry = false;
            registers.f.half_carry = false;

            return result;
        }

        uint8_t inc(uint8_t value) {
            uint8_t result = value + 1;

            registers.f.zero = result == 0;
            registers.f.subtract = false;
            registers.f.carry = registers.f.carry; // INC doesnt affect the carry flag, it's left alone
            registers.f.half_carry = (value & 0xF) + (1 & 0xF) > 0xF;

            return result;
        }

        uint8_t dec(uint8_t value) {
            uint8_t result = value - 1;

            registers.f.zero = result == 0;
            registers.f.subtract = true;
            registers.f.carry = registers.f.carry; // DEC doesnt affect the carry flag, it's left alone
            registers.f.half_carry = (1 & 0xF) > (value & 0xF);

            return result;
        }

        uint8_t swap(uint8_t value) {
            uint8_t upper_nibble = value & 0b11110000;
            uint8_t lower_nibble = value & 0b00001111;
            uint8_t result = (lower_nibble << 4) | (upper_nibble >> 4);

            registers.f.zero = result == 0;
            registers.f.subtract = false;
            registers.f.carry = false;
            registers.f.half_carry = false;

            return result;
        }

        uint16_t add_hl(uint16_t value) {
            uint16_t result = registers.get_hl() + value;
            bool overflowed = result < registers.get_hl();

            registers.f.zero = registers.f.zero; // ADDHL doesnt affect the zero flag, it's left alone
            registers.f.subtract = false;
            registers.f.carry = overflowed;
            registers.f.half_carry = (registers.get_hl() & 0xFFF) + (value & 0xFFF) > 0xFFF;

            return result;
        }

        uint8_t rlc(uint8_t value) {
            bool new_carry = (value & 0b10000000) >> 7;

            uint8_t result = (value << 1) | (new_carry);

            registers.f.zero = result == 0;
            registers.f.subtract = false;
            registers.f.carry = new_carry;
            registers.f.half_carry = false;

            return result;
        }

        uint8_t rl(uint8_t value) {
            bool old_carry = registers.f.carry;
            bool new_carry = (value & 0b10000000) >> 7;

            uint8_t result = (value << 1) | (old_carry);

            registers.f.zero = result == 0;
            registers.f.subtract = false;
            registers.f.carry = new_carry;
            registers.f.half_carry = false;

            return result;
        }

        uint8_t rrc(uint8_t value) {
            bool new_carry = value & 1;

            uint8_t result = (value >> 1) | (new_carry << 7);

            registers.f.zero = result == 0;
            registers.f.subtract = false;
            registers.f.carry = new_carry;
            registers.f.half_carry = false;

            return result;
        }

        uint8_t rr(uint8_t value) {
            uint8_t old_carry = registers.f.carry;
            bool new_carry = value & 1;

            uint8_t result = (value >> 1) | (old_carry << 7);

            registers.f.zero = result == 0;
            registers.f.subtract = false;
            registers.f.carry = new_carry;
            registers.f.half_carry = false;

            return result;
        }

        uint8_t sla(uint8_t value) {
            bool new_carry = (value & 0b10000000) >> 7;
            uint8_t result = value << 1;

            registers.f.zero = result == 0;
            registers.f.subtract = false;
            registers.f.carry = new_carry;
            registers.f.half_carry = false;

            return result;
        }

        uint8_t sra(uint8_t value) {
            bool new_carry = value & 1;
            bool msb = (value & 0b10000000) >> 7;

            uint8_t result = (value >> 1) | (msb << 7);

            registers.f.zero = result == 0;
            registers.f.subtract = false;
            registers.f.carry = new_carry;
            registers.f.half_carry = false;

            return result;
        }

        uint8_t srl(uint8_t value) {
            bool new_carry = value & 1;

            uint8_t result = value >> 1;

            registers.f.zero = result == 0;
            registers.f.subtract = false;
            registers.f.carry = new_carry;
            registers.f.half_carry = false;

            return result;
        }

        uint16_t jump(bool should_jump) {
            if (should_jump) {
                // Gameboy is little endian so read pc + 2 as most significant bit
                // and pc + 1 as least significant bit
                uint16_t least_significant_byte = bus.read_byte(pc + 1);
                uint16_t most_significant_byte = bus.read_byte(pc + 2);
                return (most_significant_byte << 8) | least_significant_byte;
            }
            else {
                // If we don't jump we need to still move the program
                // counter forward by 3 since the jump instruction is
                // 3 bytes wide (1 byte for tag and 2 bytes for jump address)
                return pc + 3;
            }
        }

        uint16_t jump_relative(bool should_jump) {
            int8_t offset = static_cast<int8_t>(bus.read_byte(pc + 1)); // Offset stored 1 byte after JR instruction

            // Always jump 2 after the instruction
            if (should_jump) {
                return static_cast<uint16_t>(pc + 2 + offset);
            } else {
                return pc + 2;
            }
        }

        // Interrupts that are both requested (IF) and enabled (IE). Only bits 0-4 are interrupts
        uint8_t pending_interrupts() {
            return bus.read_byte(IE_ADDRESS) & bus.read_byte(IF_ADDRESS) & 0x1F;
        }

        // Services the highest-priority pending interrupt (lowest bit first: V-Blank, STAT,
        // Timer, Serial, Joypad) by calling its handler at 0x40, 0x48, 0x50, 0x58 or 0x60
        void service_interrupt(uint8_t pending) {
            for (uint8_t bit = 0; bit < 5; bit++) {
                uint8_t mask = 1 << bit;

                if (pending & mask) {
                    ime = false; // The handler can't be interrupted unless it runs EI
                    bus.write_byte(IF_ADDRESS, bus.read_byte(IF_ADDRESS) & ~mask);
                    push(pc);
                    pc = 0x40 + bit * 8;
                    return;
                }
            }
        }

        bool check_jump_condition(JumpTest condition) {
            switch (condition) {
                case JumpTest::NotZero:
                    return !registers.f.zero;
                case JumpTest::NotCarry:
                    return !registers.f.carry;
                case JumpTest::Zero:
                    return registers.f.zero;
                case JumpTest::Carry:
                    return registers.f.carry;
                case JumpTest::Always:
                    return true;
                default:
                    return false;
            }
        }

        // Returns SP plus the signed 8-bit offset following the current instruction and sets
        // flags, shared by ADD SP,e and LD HL,SP+e. Half Carry and Carry are calculated from the
        // unsigned addition of the offset to the low byte of SP (carry out of bit 3 and bit 7)
        uint16_t add_sp_offset() {
            uint8_t unsigned_offset = bus.read_byte(pc + 1);
            int8_t offset = static_cast<int8_t>(unsigned_offset);

            registers.f.zero = false;
            registers.f.subtract = false;
            registers.f.half_carry = (sp & 0xF) + (unsigned_offset & 0xF) > 0xF;
            registers.f.carry = (sp & 0xFF) + unsigned_offset > 0xFF;

            return static_cast<uint16_t>(sp + offset);
        }

        // Reads the 16-bit immediate value following the current instruction (little endian)
        uint16_t read_immediate_word() {
            uint16_t least_significant_byte = bus.read_byte(pc + 1);
            uint16_t most_significant_byte = bus.read_byte(pc + 2);
            return (most_significant_byte << 8) | least_significant_byte;
        }

        // Resolves the memory address for LD_A_IND / LD_IND_A. HL_INC and HL_DEC
        // return the current HL and then increment/decrement HL afterwards.
        uint16_t indirect_address(IndirectAddress address) {
            switch (address) {
                case IndirectAddress::BC:
                    return registers.get_bc();
                case IndirectAddress::DE:
                    return registers.get_de();
                case IndirectAddress::NN:
                    return read_immediate_word();
                case IndirectAddress::FF00_C:
                    return 0xFF00 + registers.c;
                case IndirectAddress::FF00_N:
                    return 0xFF00 + bus.read_byte(pc + 1);
                case IndirectAddress::HL_INC: {
                    uint16_t hl = registers.get_hl();
                    registers.set_hl(hl + 1);
                    return hl;
                }
                case IndirectAddress::HL_DEC: {
                    uint16_t hl = registers.get_hl();
                    registers.set_hl(hl - 1);
                    return hl;
                }
                default:
                    return 0;
            }
        }

        // Instruction width in bytes for LD_A_IND / LD_IND_A
        uint16_t indirect_length(IndirectAddress address) {
            switch (address) {
                case IndirectAddress::NN:
                    return 3;
                case IndirectAddress::FF00_N:
                    return 2;
                default:
                    return 1;
            }
        }

        // Stack grows downwards: high byte is stored first at SP - 1, then low byte at SP - 2
        void push(uint16_t value) {
            sp = sp - 1;
            bus.write_byte(sp, static_cast<uint8_t>((value & 0xFF00) >> 8));
            sp = sp - 1;
            bus.write_byte(sp, static_cast<uint8_t>(value & 0xFF));
        }

        uint16_t pop() {
            uint16_t least_significant_byte = bus.read_byte(sp);
            sp = sp + 1;
            uint16_t most_significant_byte = bus.read_byte(sp);
            sp = sp + 1;
            return (most_significant_byte << 8) | least_significant_byte;
        }
};