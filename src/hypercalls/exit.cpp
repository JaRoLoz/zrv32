#include "macros.hpp"

static int32_t rv_exit(CPU &cpu)
{
    std::exit(0);
    return 0;
}

HYPERCALL(0x80000008, rv_exit)