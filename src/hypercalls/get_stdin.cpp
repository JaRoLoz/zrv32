#include "macros.hpp"

#include <iostream>
#include <string>

static int32_t rv_get_stdin(CPU &cpu)
{
    std::string input;
    std::getline(std::cin, input);

    size_t buffer_address = cpu.get_reg(CPU::REG_X10);
    size_t max_length = cpu.get_reg(CPU::REG_X11);

    size_t length = std::min(input.size(), max_length - 1);
    uint8_t *buffer = cpu.memory_ptr<uint8_t>(buffer_address);
    for (size_t i = 0; i < length; i++)
    {
        buffer[i] = static_cast<uint8_t>(input[i]);
    }
    buffer[length] = '\0';
    return static_cast<uint32_t>(length);
}

HYPERCALL(0x80000014, rv_get_stdin)