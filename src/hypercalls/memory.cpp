#include "macros.hpp"

#include <cstdlib>

static int32_t rv_malloc(CPU &cpu)
{
    size_t size = cpu.get_reg(CPU::REG_X10);
    void *ptr = std::malloc(size);
    cpu.set_reg(CPU::REG_X10, reinterpret_cast<uint32_t>(ptr));
    return reinterpret_cast<uint32_t>(ptr);
}

static int32_t rv_free(CPU &cpu)
{
    void *ptr = reinterpret_cast<void *>(cpu.get_reg(CPU::REG_X10));
    std::free(ptr);
    return 0;
}

HYPERCALL(0x8000000C, rv_malloc)
HYPERCALL(0x80000010, rv_free)