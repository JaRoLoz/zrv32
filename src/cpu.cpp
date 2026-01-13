#include "cpu.hpp"
#include "isa.hpp"
#include "hypercalls.hpp"

#include <iostream>
#include <stdexcept>
#include <fstream>

CPU::CPU(const ProgramInfo &info)
{
    m_memory.resize(info.memory_size);

    std::ifstream file{info.path, std::ios::binary | std::ios::ate};
    if (!file)
    {
        throw std::runtime_error("Failed to open program file: " + info.path.string());
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    if (size > static_cast<std::streamsize>(info.memory_size))
    {
        throw std::runtime_error("Program size exceeds allocated memory size.");
    }

    file.read(reinterpret_cast<char *>(m_memory.data() + info.load_address), size);
    if (!file)
    {
        throw std::runtime_error("Failed to read program file: " + info.path.string());
    }

    m_program_counter = info.entry_point;
}

void CPU::cycle()
{
    if (Hypercalls::instance().check(m_program_counter))
    {
        Hypercalls::instance().call(m_program_counter, *this);
    }
    else
    {
        uint32_t inst = *memory_ptr<uint32_t>(m_program_counter);
        uint8_t opcode = inst & 0x7F;
        ISA::instance().get_instruction(opcode)(inst, *this);

        if (m_increment_pc)
        {
            m_program_counter += 4;
        }
        m_increment_pc = true;
    }
}

void CPU::load_memory(const uint8_t *source, size_t size, uint32_t start_address)
{
    std::copy(source, source + size, m_memory.begin() + start_address);
}

void CPU::print_registers() const
{
    std::cout << "PC: 0x" << std::hex << m_program_counter << std::dec << "\n";
    for (size_t i = 0; i < m_registers.size(); ++i)
    {
        std::cout << "x" << i << ": 0x" << std::hex << m_registers[i] << std::dec << " (" << m_registers[i] << ")\n";
    }
}
