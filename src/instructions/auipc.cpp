#include "macros.hpp"

INSTRUCTION(0b0010111, [](uint32_t inst, CPU &cpu)
            {
    size_t rd = (inst >> 7) & 0x1F;
    uint32_t imm = inst & 0xFFFFF000;
    cpu.set_reg(rd, cpu.program_counter() + imm); })