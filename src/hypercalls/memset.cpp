#include "macros.hpp"

static int32_t rv_memset(CPU &cpu)
{
    uint32_t dest = cpu.get_reg(CPU::REG_X10);
    uint8_t value = static_cast<uint8_t>(cpu.get_reg(CPU::REG_X11) & 0xFF);
    size_t length = cpu.get_reg(CPU::REG_X12);

    if (dest + length > cpu.memory_size())
    {
        cpu.set_reg(CPU::REG_X10, 1);
        return 0;
    }

    std::memset(cpu.memory_ptr<uint8_t>(dest), value, length);
    return length;
}

HYPERCALL(0x80000004, rv_memset)