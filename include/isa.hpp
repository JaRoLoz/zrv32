#pragma once
#include <iostream>
#include <array>
#include <string_view>
#include <functional>

#include "cpu.hpp"
#include "instruction.hpp"

class ISA
{
public:
    struct RegisterInstruction
    {
        RegisterInstruction(uint8_t opcode, Instruction::instruction_execute_t execute)
        {
            ISA::instance().register_instruction(opcode, execute);
        }

        RegisterInstruction() = delete;
        RegisterInstruction(const RegisterInstruction &) = delete;
        RegisterInstruction &operator=(const RegisterInstruction &) = delete;
        RegisterInstruction(RegisterInstruction &&) = delete;
        RegisterInstruction &operator=(RegisterInstruction &&) = delete;
    };

public:
    static ISA &instance()
    {
        static ISA instance;
        return instance;
    }

public:
    void register_instruction(uint8_t opcode, Instruction::instruction_execute_t execute)
    {
        m_opcodes[opcode] = Instruction{opcode, execute};
    }

    const Instruction &get_instruction(uint8_t opcode) const
    {
        return m_opcodes[opcode];
    }

    const Instruction &operator[](uint8_t opcode) const
    {
        return m_opcodes[opcode];
    }

private:
    inline static constexpr const size_t MAX_INSTRUCTIONS = ipow(2, 7);

private:
    ISA() = default;

private:
    std::array<Instruction, MAX_INSTRUCTIONS> m_opcodes{};
};
