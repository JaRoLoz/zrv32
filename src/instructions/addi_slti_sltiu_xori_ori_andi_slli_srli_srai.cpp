#include "macros.hpp"
#include <array>

inline int32_t get_i_imm(uint32_t inst)
{
    return static_cast<int32_t>(inst) >> 20;
}

void addi(uint32_t inst, CPU &cpu)
{
    size_t rd = (inst >> 7) & 0x1F;
    size_t rs1 = (inst >> 15) & 0x1F;
    cpu.set_reg(rd, cpu.get_reg(rs1) + get_i_imm(inst));
}

void slti(uint32_t inst, CPU &cpu)
{
    size_t rd = (inst >> 7) & 0x1F;
    size_t rs1 = (inst >> 15) & 0x1F;
    int32_t imm = get_i_imm(inst);
    cpu.set_reg(rd, (static_cast<int32_t>(cpu.get_reg(rs1)) < imm) ? 1 : 0);
}

void sltiu(uint32_t inst, CPU &cpu)
{
    size_t rd = (inst >> 7) & 0x1F;
    size_t rs1 = (inst >> 15) & 0x1F;
    int32_t imm_signed = get_i_imm(inst);
    cpu.set_reg(rd, (cpu.get_reg(rs1) < static_cast<uint32_t>(imm_signed)) ? 1 : 0);
}

void xori(uint32_t inst, CPU &cpu)
{
    size_t rd = (inst >> 7) & 0x1F;
    size_t rs1 = (inst >> 15) & 0x1F;
    cpu.set_reg(rd, cpu.get_reg(rs1) ^ get_i_imm(inst));
}

void ori(uint32_t inst, CPU &cpu)
{
    size_t rd = (inst >> 7) & 0x1F;
    size_t rs1 = (inst >> 15) & 0x1F;
    cpu.set_reg(rd, cpu.get_reg(rs1) | get_i_imm(inst));
}

void andi(uint32_t inst, CPU &cpu)
{
    size_t rd = (inst >> 7) & 0x1F;
    size_t rs1 = (inst >> 15) & 0x1F;
    cpu.set_reg(rd, cpu.get_reg(rs1) & get_i_imm(inst));
}

void slli(uint32_t inst, CPU &cpu)
{
    size_t rd = (inst >> 7) & 0x1F;
    size_t rs1 = (inst >> 15) & 0x1F;
    size_t shamt = (inst >> 20) & 0x1F;
    cpu.set_reg(rd, cpu.get_reg(rs1) << shamt);
}

void srli(uint32_t inst, CPU &cpu)
{
    size_t rd = (inst >> 7) & 0x1F;
    size_t rs1 = (inst >> 15) & 0x1F;
    size_t shamt = (inst >> 20) & 0x1F;
    cpu.set_reg(rd, cpu.get_reg(rs1) >> shamt);
}

void srai(uint32_t inst, CPU &cpu)
{
    size_t rd = (inst >> 7) & 0x1F;
    size_t rs1 = (inst >> 15) & 0x1F;
    size_t shamt = (inst >> 20) & 0x1F;
    cpu.set_reg(rd, static_cast<int32_t>(cpu.get_reg(rs1)) >> shamt);
}

void sr_dispatch(uint32_t inst, CPU &cpu)
{
    size_t funct7 = (inst >> 25) & 0x7F;
    if (funct7 == 0b0000000)
    {
        srli(inst, cpu);
    }
    else if (funct7 == 0b0100000)
    {
        srai(inst, cpu);
    }
}

static void instruction(uint32_t inst, CPU &cpu)
{
    static const std::array<Instruction::instruction_execute_t, 8> operations = {
        addi, slli, slti, sltiu, xori, sr_dispatch, ori, andi};

    size_t discriminator = (inst >> 12) & 0x7; // funct3

    operations[discriminator](inst, cpu);
}

INSTRUCTION(0b0010011, instruction)
