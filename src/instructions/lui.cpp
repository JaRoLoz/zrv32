#include "macros.hpp"

INSTRUCTION(0b0110111, [](uint32_t inst, CPU &cpu)
            {
    size_t rd = (inst >> 7) & 0x1F;
    uint32_t imm = inst & 0xFFFFF000;
    cpu.set_reg(rd, imm); })