#include "macros.hpp"

#include <bit>
#include <cmath>
#include <array>

void flw(uint32_t inst, CPU &cpu)
{
    size_t rd = (inst >> 7) & 0x1F;
    size_t rs1 = (inst >> 15) & 0x1F;
    int32_t imm = static_cast<int32_t>(inst) >> 20;

    uint32_t addr = cpu.get_reg(rs1) + imm;
    uint32_t raw_bits = *cpu.memory_ptr<uint32_t>(addr);
    cpu.set_freg(rd, std::bit_cast<float>(raw_bits));
}

void fsw(uint32_t inst, CPU &cpu)
{
    size_t rs1 = (inst >> 15) & 0x1F;
    size_t rs2 = (inst >> 20) & 0x1F;
    int32_t imm = ((inst >> 25) << 5) | ((inst >> 7) & 0x1F);
    if (imm & 0x800)
    {
        imm |= 0xFFFFF000;
    }

    uint32_t addr = cpu.get_reg(rs1) + imm;
    float val = cpu.get_freg(rs2);
    *cpu.memory_ptr<uint32_t>(addr) = std::bit_cast<uint32_t>(val);
}

void fsgnj_s(uint32_t inst, CPU &cpu)
{
    size_t rd = (inst >> 7) & 0x1F;
    size_t rs1 = (inst >> 15) & 0x1F;
    size_t rs2 = (inst >> 20) & 0x1F;
    size_t funct3 = (inst >> 12) & 0x7;

    uint32_t u1 = std::bit_cast<uint32_t>(cpu.get_freg(rs1));
    uint32_t u2 = std::bit_cast<uint32_t>(cpu.get_freg(rs2));

    uint32_t res_u;

    if (funct3 == 0)
    {
        res_u = (u1 & 0x7FFFFFFF) | (u2 & 0x80000000); // FSGNJ
    }
    else if (funct3 == 1)
    {
        res_u = (u1 & 0x7FFFFFFF) | (~u2 & 0x80000000); // FSGNJN
    }
    else
    {
        res_u = (u1 & 0x7FFFFFFF) | ((u1 ^ u2) & 0x80000000); // FSGNJX
    }

    cpu.set_freg(rd, std::bit_cast<float>(res_u));
}

void fadd_s(uint32_t inst, CPU &cpu)
{
    size_t rd = (inst >> 7) & 0x1F;
    size_t rs1 = (inst >> 15) & 0x1F;
    size_t rs2 = (inst >> 20) & 0x1F;
    cpu.set_freg(rd, cpu.get_freg(rs1) + cpu.get_freg(rs2));
}

void fsub_s(uint32_t inst, CPU &cpu)
{
    size_t rd = (inst >> 7) & 0x1F;
    size_t rs1 = (inst >> 15) & 0x1F;
    size_t rs2 = (inst >> 20) & 0x1F;
    cpu.set_freg(rd, cpu.get_freg(rs1) - cpu.get_freg(rs2));
}

void fmul_s(uint32_t inst, CPU &cpu)
{
    size_t rd = (inst >> 7) & 0x1F;
    size_t rs1 = (inst >> 15) & 0x1F;
    size_t rs2 = (inst >> 20) & 0x1F;
    cpu.set_freg(rd, cpu.get_freg(rs1) * cpu.get_freg(rs2));
}

void fdiv_s(uint32_t inst, CPU &cpu)
{
    size_t rd = (inst >> 7) & 0x1F;
    size_t rs1 = (inst >> 15) & 0x1F;
    size_t rs2 = (inst >> 20) & 0x1F;
    cpu.set_freg(rd, cpu.get_freg(rs1) / cpu.get_freg(rs2));
}

void fsqrt_s(uint32_t inst, CPU &cpu)
{
    size_t rd = (inst >> 7) & 0x1F;
    size_t rs1 = (inst >> 15) & 0x1F;

    cpu.set_freg(rd, std::sqrt(cpu.get_freg(rs1)));
}

void fcvt_w_s(uint32_t inst, CPU &cpu)
{
    size_t rd = (inst >> 7) & 0x1F;
    size_t rs1 = (inst >> 15) & 0x1F;

    cpu.set_reg(rd, static_cast<int32_t>(cpu.get_freg(rs1)));
}

void fcvt_s_w(uint32_t inst, CPU &cpu)
{
    size_t rd = (inst >> 7) & 0x1F;
    size_t rs1 = (inst >> 15) & 0x1F;

    cpu.set_freg(rd, static_cast<float>(static_cast<int32_t>(cpu.get_reg(rs1))));
}

void fmv_x_w(uint32_t inst, CPU &cpu)
{
    size_t rd = (inst >> 7) & 0x1F;
    size_t rs1 = (inst >> 15) & 0x1F;

    cpu.set_reg(rd, std::bit_cast<uint32_t>(cpu.get_freg(rs1)));
}

void fmv_w_x(uint32_t inst, CPU &cpu)
{
    size_t rd = (inst >> 7) & 0x1F;
    size_t rs1 = (inst >> 15) & 0x1F;

    cpu.set_freg(rd, std::bit_cast<float>(cpu.get_reg(rs1)));
}

void fcmp_s(uint32_t inst, CPU &cpu)
{
    size_t rd = (inst >> 7) & 0x1F;
    size_t rs1 = (inst >> 15) & 0x1F;
    size_t rs2 = (inst >> 20) & 0x1F;
    size_t funct3 = (inst >> 12) & 0x7;

    float val1 = cpu.get_freg(rs1);
    float val2 = cpu.get_freg(rs2);
    bool res = false;

    if (funct3 == 0x0)
    {
        res = (val1 <= val2); // fle.s
    }
    else if (funct3 == 0x1)
    {
        res = (val1 < val2); // flt.s
    }
    else if (funct3 == 0x2)
    {
        res = (val1 == val2); // feq.s
    }

    cpu.set_reg(rd, res ? 1 : 0);
}

void fmadd_s(uint32_t inst, CPU &cpu)
{
    size_t rd = (inst >> 7) & 0x1F;
    size_t rs1 = (inst >> 15) & 0x1F;
    size_t rs2 = (inst >> 20) & 0x1F;
    size_t rs3 = (inst >> 27) & 0x1F;

    float val1 = cpu.get_freg(rs1);
    float val2 = cpu.get_freg(rs2);
    float val3 = cpu.get_freg(rs3);

    cpu.set_freg(rd, (val1 * val2) + val3);
}

void fmsub_s(uint32_t inst, CPU &cpu)
{
    size_t rd = (inst >> 7) & 0x1F;
    size_t rs1 = (inst >> 15) & 0x1F;
    size_t rs2 = (inst >> 20) & 0x1F;
    size_t rs3 = (inst >> 27) & 0x1F;

    float val1 = cpu.get_freg(rs1);
    float val2 = cpu.get_freg(rs2);
    float val3 = cpu.get_freg(rs3);

    cpu.set_freg(rd, (val1 * val2) - val3);
}

void fnmsub_s(uint32_t inst, CPU &cpu)
{
    size_t rd = (inst >> 7) & 0x1F;
    size_t rs1 = (inst >> 15) & 0x1F;
    size_t rs2 = (inst >> 20) & 0x1F;
    size_t rs3 = (inst >> 27) & 0x1F;

    float val1 = cpu.get_freg(rs1);
    float val2 = cpu.get_freg(rs2);
    float val3 = cpu.get_freg(rs3);

    cpu.set_freg(rd, -(val1 * val2) + val3);
}

void fnmadd_s(uint32_t inst, CPU &cpu)
{
    size_t rd = (inst >> 7) & 0x1F;
    size_t rs1 = (inst >> 15) & 0x1F;
    size_t rs2 = (inst >> 20) & 0x1F;
    size_t rs3 = (inst >> 27) & 0x1F;

    float val1 = cpu.get_freg(rs1);
    float val2 = cpu.get_freg(rs2);
    float val3 = cpu.get_freg(rs3);

    cpu.set_freg(rd, -(val1 * val2) - val3);
}

void illegal_fp(uint32_t inst, CPU &cpu)
{
    std::cerr << "Illegal FP funct7: " << ((inst >> 25) & 0x7F) << std::endl;
}

static void dispatch(uint32_t inst, CPU &cpu)
{
    size_t funct7 = ((inst >> 25) & 0x7F) >> 2;

    static const std::array<Instruction::instruction_execute_t, 32> fp_ops = {
        fadd_s,
        fsub_s,
        fmul_s,
        fdiv_s,
        fsgnj_s,
        illegal_fp,
        illegal_fp,
        illegal_fp,
        illegal_fp,
        illegal_fp,
        illegal_fp,
        fsqrt_s,
        illegal_fp,
        illegal_fp,
        illegal_fp,
        illegal_fp,
        illegal_fp,
        illegal_fp,
        illegal_fp,
        illegal_fp,
        fcmp_s,
        illegal_fp,
        illegal_fp,
        illegal_fp,
        fcvt_w_s,
        illegal_fp,
        fcvt_s_w,
        illegal_fp,
        fmv_x_w,
        illegal_fp,
        fmv_w_x,
        illegal_fp};

    if (funct7 < fp_ops.size())
    {
        fp_ops[funct7](inst, cpu);
    }
    else
    {
        illegal_fp(inst, cpu);
    }
}

INSTRUCTION(0b0000111, flw)
INSTRUCTION(0b0100111, fsw)
INSTRUCTION(0b1000011, fmadd_s)
INSTRUCTION(0b1000111, fmsub_s)
INSTRUCTION(0b1001011, fnmsub_s)
INSTRUCTION(0b1001111, fnmadd_s)
INSTRUCTION(0b1010011, dispatch)
