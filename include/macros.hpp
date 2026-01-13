#pragma once

#include "isa.hpp"
#include "hypercalls.hpp"

#define TO_STRING_IMPL(x) #x
#define TO_STRING(x) TO_STRING_IMPL(x)

#define CONCAT_IMPL(x, y) x##y
#define CONCAT(x, y) CONCAT_IMPL(x, y)

#define INSTRUCTION(opcode, execute)                              \
    static ISA::RegisterInstruction CONCAT(reg_instr_, __LINE__)( \
        opcode,                                                   \
        execute);

#define HYPERCALL(address, handler)                                   \
    static Hypercalls::AddHypercall CONCAT(add_hypercall_, __LINE__)( \
        address,                                                      \
        handler);