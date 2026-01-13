#include "macros.hpp"
#include <array>

void lb(uint32_t inst, CPU &cpu)
{
    size_t rd = (inst >> 7) & 0x1F;
    size_t rs1 = (inst >> 15) & 0x1F;
    int32_t imm = static_cast<int32_t>(inst) >> 20;
    uint32_t address = cpu.get_reg(rs1) + imm;
    cpu.set_reg(rd, cpu.memory_ptr<int8_t>(address)[0]);
}

void lh(uint32_t inst, CPU &cpu)
{
    size_t rd = (inst >> 7) & 0x1F;
    size_t rs1 = (inst >> 15) & 0x1F;
    int32_t imm = static_cast<int32_t>(inst) >> 20;
    uint32_t address = cpu.get_reg(rs1) + imm;
    cpu.set_reg(rd, cpu.memory_ptr<int16_t>(address)[0]);
}

void lw(uint32_t inst, CPU &cpu)
{
    size_t rd = (inst >> 7) & 0x1F;
    size_t rs1 = (inst >> 15) & 0x1F;
    int32_t imm = static_cast<int32_t>(inst) >> 20;

    uint32_t address = cpu.get_reg(rs1) + imm;

    int32_t value = cpu.memory_ptr<int32_t>(address)[0];

    cpu.set_reg(rd, value);
}

void lbu(uint32_t inst, CPU &cpu)
{
    size_t rd = (inst >> 7) & 0x1F;
    size_t rs1 = (inst >> 15) & 0x1F;
    int32_t imm = static_cast<int32_t>(inst) >> 20;
    uint32_t address = cpu.get_reg(rs1) + imm;
    cpu.set_reg(rd, cpu.memory_ptr<uint8_t>(address)[0]);
}

void lhu(uint32_t inst, CPU &cpu)
{
    size_t rd = (inst >> 7) & 0x1F;
    size_t rs1 = (inst >> 15) & 0x1F;
    int32_t imm = static_cast<int32_t>(inst) >> 20;
    uint32_t address = cpu.get_reg(rs1) + imm;
    cpu.set_reg(rd, cpu.memory_ptr<uint16_t>(address)[0]);
}

void illegal_load(uint32_t inst, CPU &cpu)
{
    std::cerr << "Illegal Load funct3: " << ((inst >> 12) & 0x7) << std::endl;
}

static void dispatch_load(uint32_t inst, CPU &cpu)
{
    static const std::array<Instruction::instruction_execute_t, 8> operations = {
        lb, lh, lw, illegal_load, lbu, lhu, illegal_load, illegal_load};

    size_t funct3 = (inst >> 12) & 0x7;
    operations[funct3](inst, cpu);
}

INSTRUCTION(0b0000011, dispatch_load)