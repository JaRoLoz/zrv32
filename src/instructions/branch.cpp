#include "macros.hpp"
#include <array>
#include <iostream>

inline void decode_branch(uint32_t inst, CPU &cpu, uint32_t &val1, uint32_t &val2, int32_t &offset)
{
    size_t rs1 = (inst >> 15) & 0x1F;
    size_t rs2 = (inst >> 20) & 0x1F;

    val1 = cpu.get_reg(rs1);
    val2 = cpu.get_reg(rs2);

    int32_t imm = (static_cast<int32_t>(inst) >> 31) << 12;

    imm |= ((inst << 4) & 0x800);
    imm |= ((inst >> 20) & 0x7E0);
    imm |= ((inst >> 7) & 0x1E);

    offset = imm;
}

void beq(uint32_t inst, CPU &cpu)
{
    uint32_t val1, val2;
    int32_t offset;

    decode_branch(inst, cpu, val1, val2, offset);

    if (val1 == val2)
    {
        cpu.program_counter() += offset;
        cpu.skip_pc_increment();
    }
}

void bne(uint32_t inst, CPU &cpu)
{
    uint32_t val1, val2;
    int32_t offset;
    decode_branch(inst, cpu, val1, val2, offset);

    if (val1 != val2)
    {
        cpu.program_counter() += offset;
        cpu.skip_pc_increment();
    }
}

void blt(uint32_t inst, CPU &cpu)
{
    uint32_t val1, val2;
    int32_t offset;
    decode_branch(inst, cpu, val1, val2, offset);

    if (static_cast<int32_t>(val1) < static_cast<int32_t>(val2))
    {
        cpu.program_counter() += offset;
        cpu.skip_pc_increment();
    }
}

void bge(uint32_t inst, CPU &cpu)
{
    uint32_t val1, val2;
    int32_t offset;
    decode_branch(inst, cpu, val1, val2, offset);

    if (static_cast<int32_t>(val1) >= static_cast<int32_t>(val2))
    {
        cpu.program_counter() += offset;
        cpu.skip_pc_increment();
    }
}

void bltu(uint32_t inst, CPU &cpu)
{
    uint32_t val1, val2;
    int32_t offset;
    decode_branch(inst, cpu, val1, val2, offset);

    if (val1 < val2)
    {
        cpu.program_counter() += offset;
        cpu.skip_pc_increment();
    }
}

void bgeu(uint32_t inst, CPU &cpu)
{
    uint32_t val1, val2;
    int32_t offset;
    decode_branch(inst, cpu, val1, val2, offset);

    if (val1 >= val2)
    {
        cpu.program_counter() += offset;
        cpu.skip_pc_increment();
    }
}

void illegal_branch(uint32_t inst, CPU &cpu)
{
    std::cerr << "Illegal Branch funct3: " << ((inst >> 12) & 0x7) << std::endl;
}

static void dispatch(uint32_t inst, CPU &cpu)
{
    static const std::array<Instruction::instruction_execute_t, 8> operations = {
        beq, bne, illegal_branch, illegal_branch, blt, bge, bltu, bgeu};

    size_t discriminator = (inst >> 12) & 0x7;
    operations[discriminator](inst, cpu);
}

INSTRUCTION(0b1100011, dispatch)