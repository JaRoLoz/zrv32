#include "macros.hpp"

#include <array>
#include "instructions/m_extension.hpp"

void add(uint32_t inst, CPU &cpu)
{
    size_t rd = (inst >> 7) & 0x1F;
    size_t rs1 = (inst >> 15) & 0x1F;
    size_t rs2 = (inst >> 20) & 0x1F;
    cpu.set_reg(rd, cpu.get_reg(rs1) + cpu.get_reg(rs2));
}

void sub(uint32_t inst, CPU &cpu)
{
    size_t rd = (inst >> 7) & 0x1F;
    size_t rs1 = (inst >> 15) & 0x1F;
    size_t rs2 = (inst >> 20) & 0x1F;
    cpu.set_reg(rd, cpu.get_reg(rs1) - cpu.get_reg(rs2));
}

void dispatch_add_sub(uint32_t inst, CPU &cpu)
{
    size_t discriminator = (inst >> 25) & 0x7F;
    if (discriminator == 0b0000000)
    {
        add(inst, cpu);
    }
    else if (discriminator == 0b0100000)
    {
        sub(inst, cpu);
    }
}

void sll(uint32_t inst, CPU &cpu)
{
    size_t rd = (inst >> 7) & 0x1F;
    size_t rs1 = (inst >> 15) & 0x1F;
    size_t rs2 = (inst >> 20) & 0x1F;
    cpu.set_reg(rd, cpu.get_reg(rs1) << (cpu.get_reg(rs2) & 0x1F));
}

void slt(uint32_t inst, CPU &cpu)
{
    size_t rd = (inst >> 7) & 0x1F;
    size_t rs1 = (inst >> 15) & 0x1F;
    size_t rs2 = (inst >> 20) & 0x1F;
    cpu.set_reg(rd, (static_cast<int32_t>(cpu.get_reg(rs1)) < static_cast<int32_t>(cpu.get_reg(rs2))) ? 1 : 0);
}

void sltu(uint32_t inst, CPU &cpu)
{
    size_t rd = (inst >> 7) & 0x1F;
    size_t rs1 = (inst >> 15) & 0x1F;
    size_t rs2 = (inst >> 20) & 0x1F;
    cpu.set_reg(rd, (cpu.get_reg(rs1) < cpu.get_reg(rs2)) ? 1 : 0);
}

void xor_op(uint32_t inst, CPU &cpu)
{
    size_t rd = (inst >> 7) & 0x1F;
    size_t rs1 = (inst >> 15) & 0x1F;
    size_t rs2 = (inst >> 20) & 0x1F;
    cpu.set_reg(rd, cpu.get_reg(rs1) ^ cpu.get_reg(rs2));
}

void srl(uint32_t inst, CPU &cpu)
{
    size_t rd = (inst >> 7) & 0x1F;
    size_t rs1 = (inst >> 15) & 0x1F;
    size_t rs2 = (inst >> 20) & 0x1F;
    cpu.set_reg(rd, cpu.get_reg(rs1) >> (cpu.get_reg(rs2) & 0x1F));
}

void sra(uint32_t inst, CPU &cpu)
{
    size_t rd = (inst >> 7) & 0x1F;
    size_t rs1 = (inst >> 15) & 0x1F;
    size_t rs2 = (inst >> 20) & 0x1F;
    cpu.set_reg(rd, static_cast<int32_t>(cpu.get_reg(rs1)) >> (cpu.get_reg(rs2) & 0x1F));
}

void dispatch_srl_sra(uint32_t inst, CPU &cpu)
{
    size_t discriminator = (inst >> 25) & 0x7F;
    if (discriminator == 0b0000000)
    {
        srl(inst, cpu);
    }
    else if (discriminator == 0b0100000)
    {
        sra(inst, cpu);
    }
}

void or_op(uint32_t inst, CPU &cpu)
{
    size_t rd = (inst >> 7) & 0x1F;
    size_t rs1 = (inst >> 15) & 0x1F;
    size_t rs2 = (inst >> 20) & 0x1F;
    cpu.set_reg(rd, cpu.get_reg(rs1) | cpu.get_reg(rs2));
}

void and_op(uint32_t inst, CPU &cpu)
{
    size_t rd = (inst >> 7) & 0x1F;
    size_t rs1 = (inst >> 15) & 0x1F;
    size_t rs2 = (inst >> 20) & 0x1F;
    cpu.set_reg(rd, cpu.get_reg(rs1) & cpu.get_reg(rs2));
}

static void instruction(uint32_t inst, CPU &cpu)
{
    size_t funct3 = (inst >> 12) & 0x7;
    size_t funct7 = (inst >> 25) & 0x7F;

    if (funct7 == M_EXTENSION_FUNCT7)
    {
        static const std::array<Instruction::instruction_execute_t, 8> m_ops = {
            mul, mulh, mulhsu, mulhu, div_op, divu, rem, remu};
        m_ops[funct3](inst, cpu);
    }
    else
    {
        static const std::array<Instruction::instruction_execute_t, 8> ops = {
            dispatch_add_sub, sll, slt, sltu, xor_op, dispatch_srl_sra, or_op, and_op};
        ops[funct3](inst, cpu);
    }
}

INSTRUCTION(0b0110011, instruction);
