#include "macros.hpp"

/* --- JAL --- */
INSTRUCTION(0b1101111, [](uint32_t inst, CPU &cpu)
            {
    size_t rd = (inst >> 7) & 0x1F;

    int32_t imm = static_cast<int32_t>(inst & 0x80000000) >> 11;
    imm |= (inst & 0x000FF000);
    imm |= ((inst >> 20) & 1) << 11;
    imm |= ((inst >> 21) & 0x3FF) << 1;

    uint32_t current_pc = cpu.program_counter();
    cpu.set_reg(rd, current_pc + 4);
    cpu.program_counter() = current_pc + imm; 
    cpu.skip_pc_increment(); });

/* --- JALR --- */
INSTRUCTION(0b1100111, [](uint32_t inst, CPU &cpu)
            {
    size_t rd = (inst >> 7) & 0x1F;
    size_t rs1 = (inst >> 15) & 0x1F;
    int32_t imm = static_cast<int32_t>(inst) >> 20;

    uint32_t current_pc = cpu.program_counter();
    uint32_t target_pc = (cpu.get_reg(rs1) + imm) & 0xFFFFFFFE;

    cpu.set_reg(rd, current_pc + 4);

    cpu.program_counter() = target_pc; 
    cpu.skip_pc_increment(); })