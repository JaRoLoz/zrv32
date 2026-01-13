#include "macros.hpp"

static int32_t rv_putc(CPU &cpu)
{
    char ch = static_cast<char>(cpu.get_reg(CPU::REG_X10) & 0xFF);
    std::cout << ch;
    cpu.set_reg(CPU::REG_X10, 0);
    return 0;
}

HYPERCALL(0x80000000, rv_putc)