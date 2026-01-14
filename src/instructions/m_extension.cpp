#include "instructions/m_extension.hpp"

void mul(uint32_t inst, CPU &cpu)
{
    size_t rd = (inst >> 7) & 0x1F;
    size_t rs1 = (inst >> 15) & 0x1F;
    size_t rs2 = (inst >> 20) & 0x1F;
    cpu.set_reg(rd, static_cast<int32_t>(cpu.get_reg(rs1)) * static_cast<int32_t>(cpu.get_reg(rs2)));
}

void mulh(uint32_t inst, CPU &cpu)
{
    size_t rd = (inst >> 7) & 0x1F;
    size_t rs1 = (inst >> 15) & 0x1F;
    size_t rs2 = (inst >> 20) & 0x1F;
    int64_t result = static_cast<int64_t>(static_cast<int32_t>(cpu.get_reg(rs1))) * static_cast<int64_t>(static_cast<int32_t>(cpu.get_reg(rs2)));
    cpu.set_reg(rd, static_cast<uint32_t>(result >> 32));
}

void mulhsu(uint32_t inst, CPU &cpu)
{
    size_t rd = (inst >> 7) & 0x1F;
    size_t rs1 = (inst >> 15) & 0x1F;
    size_t rs2 = (inst >> 20) & 0x1F;
    int64_t result = static_cast<int64_t>(static_cast<int32_t>(cpu.get_reg(rs1))) * static_cast<uint64_t>(cpu.get_reg(rs2));
    cpu.set_reg(rd, static_cast<uint32_t>(result >> 32));
}

void mulhu(uint32_t inst, CPU &cpu)
{
    size_t rd = (inst >> 7) & 0x1F;
    size_t rs1 = (inst >> 15) & 0x1F;
    size_t rs2 = (inst >> 20) & 0x1F;
    uint64_t result = static_cast<uint64_t>(cpu.get_reg(rs1)) * static_cast<uint64_t>(cpu.get_reg(rs2));
    cpu.set_reg(rd, static_cast<uint32_t>(result >> 32));
}

void div_op(uint32_t inst, CPU &cpu)
{
    size_t rd = (inst >> 7) & 0x1F;
    size_t rs1 = (inst >> 15) & 0x1F;
    size_t rs2 = (inst >> 20) & 0x1F;
    int32_t val1 = static_cast<int32_t>(cpu.get_reg(rs1));
    int32_t val2 = static_cast<int32_t>(cpu.get_reg(rs2));

    if (val2 == 0)
    {
        cpu.set_reg(rd, -1); // RISC-V: x / 0 = -1
    }
    else if (val1 == -2147483648 && val2 == -1)
    {
        cpu.set_reg(rd, val1); // Overflow: INT_MIN / -1 = INT_MIN
    }
    else
    {
        cpu.set_reg(rd, val1 / val2);
    }
}

void divu(uint32_t inst, CPU &cpu)
{
    size_t rd = (inst >> 7) & 0x1F;
    size_t rs1 = (inst >> 15) & 0x1F;
    size_t rs2 = (inst >> 20) & 0x1F;
    uint32_t val1 = cpu.get_reg(rs1);
    uint32_t val2 = cpu.get_reg(rs2);

    if (val2 == 0)
    {
        cpu.set_reg(rd, 0xFFFFFFFF); // RISC-V: x / 0 = MAX_UINT
    }
    else
    {
        cpu.set_reg(rd, val1 / val2);
    }
}

void rem(uint32_t inst, CPU &cpu)
{
    size_t rd = (inst >> 7) & 0x1F;
    size_t rs1 = (inst >> 15) & 0x1F;
    size_t rs2 = (inst >> 20) & 0x1F;
    int32_t val1 = static_cast<int32_t>(cpu.get_reg(rs1));
    int32_t val2 = static_cast<int32_t>(cpu.get_reg(rs2));

    if (val2 == 0)
    {
        cpu.set_reg(rd, val1); // RISC-V: x % 0 = x
    }
    else if (val1 == -2147483648 && val2 == -1)
    {
        cpu.set_reg(rd, 0); // Overflow
    }
    else
    {
        cpu.set_reg(rd, val1 % val2);
    }
}

void remu(uint32_t inst, CPU &cpu)
{
    size_t rd = (inst >> 7) & 0x1F;
    size_t rs1 = (inst >> 15) & 0x1F;
    size_t rs2 = (inst >> 20) & 0x1F;
    uint32_t val2 = cpu.get_reg(rs2);

    if (val2 == 0)
    {
        cpu.set_reg(rd, cpu.get_reg(rs1));
    }
    else
    {
        cpu.set_reg(rd, cpu.get_reg(rs1) % val2);
    }
}
