#pragma once
#include <cstdint>
#include <functional>

#include "cpu.hpp"

class Instruction
{
public:
    using instruction_execute_t = std::function<void(uint32_t, CPU &)>;

public:
    Instruction() : m_opcode{NOP_OPCODE}, m_execute{NOP_execute} {}
    Instruction(uint8_t opcode, instruction_execute_t execute)
        : m_opcode{opcode}, m_execute{execute} {}

    inline void operator()(uint32_t inst, CPU &cpu) const { m_execute(inst, cpu); }

private:
    static constexpr uint8_t NOP_OPCODE = 0b01100100; // addi opcode
    static void NOP_execute(uint32_t, CPU &) {}

private:
    uint8_t m_opcode;
    instruction_execute_t m_execute;
};