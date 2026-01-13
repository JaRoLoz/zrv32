#include "macros.hpp"

void sb(uint32_t inst, CPU &cpu)
{
    size_t rs1 = (inst >> 15) & 0x1F;
    size_t rs2 = (inst >> 20) & 0x1F;

    int32_t imm = ((static_cast<int32_t>(inst) >> 25) << 5) | ((inst >> 7) & 0x1F);
    uint32_t address = cpu.get_reg(rs1) + imm;

    *cpu.memory_ptr<uint8_t>(address) = static_cast<uint8_t>(cpu.get_reg(rs2));
}

void sh(uint32_t inst, CPU &cpu)
{
    size_t rs1 = (inst >> 15) & 0x1F;
    size_t rs2 = (inst >> 20) & 0x1F;
    int32_t imm = ((static_cast<int32_t>(inst) >> 25) << 5) | ((inst >> 7) & 0x1F);
    uint32_t address = cpu.get_reg(rs1) + imm;

    *cpu.memory_ptr<uint16_t>(address) = static_cast<uint16_t>(cpu.get_reg(rs2));
}

void sw(uint32_t inst, CPU &cpu)
{
    size_t rs1 = (inst >> 15) & 0x1F;
    size_t rs2 = (inst >> 20) & 0x1F;
    int32_t imm = ((static_cast<int32_t>(inst) >> 25) << 5) | ((inst >> 7) & 0x1F);

    uint32_t address = cpu.get_reg(rs1) + imm;
    uint32_t value = cpu.get_reg(rs2);

    *cpu.memory_ptr<uint32_t>(address) = value;
}

void illegal_store(uint32_t inst, CPU &cpu)
{
    std::cerr << "Illegal Store funct3: " << ((inst >> 12) & 0x7) << std::endl;
}

static void dispatch(uint32_t inst, CPU &cpu)
{
    static const std::array<Instruction::instruction_execute_t, 8> ops = {
        sb, sh, sw, illegal_store, illegal_store, illegal_store, illegal_store, illegal_store};

    size_t funct3 = (inst >> 12) & 0x7;
    ops[funct3](inst, cpu);
}

INSTRUCTION(0b0100011, dispatch)