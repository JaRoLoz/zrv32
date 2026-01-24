#pragma once

#include <cstdint>
#include <filesystem>
#include <cmath>
#include <unordered_map>
#include <vector>
#include <array>
#include <string>
#include <functional>
#include <span>
#include <stdexcept>
#include <format>

#include "pow.hpp"

class CPU
{
public:
    typedef struct
    {
        std::filesystem::path path;
        uint32_t load_address;
        uint32_t entry_point;
        uint32_t memory_size;
    } ProgramInfo;

public:
    enum Registers
    {
        REG_X0 = 0,
        REG_X1 = 1,
        REG_X2 = 2,
        REG_X3 = 3,
        REG_X4 = 4,
        REG_X5 = 5,
        REG_X6 = 6,
        REG_X7 = 7,
        REG_X8 = 8,
        REG_X9 = 9,
        REG_X10 = 10,
        REG_X11 = 11,
        REG_X12 = 12,
        REG_X13 = 13,
        REG_X14 = 14,
        REG_X15 = 15,
        REG_X16 = 16,
        REG_X17 = 17,
        REG_X18 = 18,
        REG_X19 = 19,
        REG_X20 = 20,
        REG_X21 = 21,
        REG_X22 = 22,
        REG_X23 = 23,
        REG_X24 = 24,
        REG_X25 = 25,
        REG_X26 = 26,
        REG_X27 = 27,
        REG_X28 = 28,
        REG_X29 = 29,
        REG_X30 = 30,
        REG_X31 = 31
    };

    enum FRegisters
    {
        REG_F0 = 0,
        REG_F1 = 1,
        REG_F2 = 2,
        REG_F3 = 3,
        REG_F4 = 4,
        REG_F5 = 5,
        REG_F6 = 6,
        REG_F7 = 7,
        REG_F8 = 8,
        REG_F9 = 9,
        REG_F10 = 10,
        REG_F11 = 11,
        REG_F12 = 12,
        REG_F13 = 13,
        REG_F14 = 14,
        REG_F15 = 15,
        REG_F16 = 16,
        REG_F17 = 17,
        REG_F18 = 18,
        REG_F19 = 19,
        REG_F20 = 20,
        REG_F21 = 21,
        REG_F22 = 22,
        REG_F23 = 23,
        REG_F24 = 24,
        REG_F25 = 25,
        REG_F26 = 26,
        REG_F27 = 27,
        REG_F28 = 28,
        REG_F29 = 29,
        REG_F30 = 30,
        REG_F31 = 31
    };

    class Exception : public std::runtime_error
    {
    public:
        Exception(
            const std::string &message,
            uint32_t program_counter,
            std::span<const uint32_t> registers,
            std::span<const float> fregisters)
            : std::runtime_error{message},
              m_program_counter{program_counter}
        {
            std::copy(registers.begin(), registers.end(), m_registers.begin());
            std::copy(fregisters.begin(), fregisters.end(), m_fregisters.begin());
        }

    private:
        uint32_t m_program_counter;
        std::array<uint32_t, 32> m_registers;
        std::array<float, 32> m_fregisters;
    };

    class MemoryAccessException : public Exception
    {
    public:
        MemoryAccessException(
            size_t address,
            uint32_t program_counter,
            std::span<const uint32_t> registers,
            std::span<const float> fregisters)
            : Exception{std::format("Memory access violation at address 0x{:08X}", address), program_counter, registers, fregisters}
        {
            // too expensive to use right now, will be used when memory bounds checking is implemented through page tables
        }
    };

    class IllegalInstructionException : public Exception
    {
        friend class CPU;

    public:
        IllegalInstructionException(
            uint32_t instruction,
            uint32_t program_counter,
            std::span<const uint32_t> registers,
            std::span<const float> fregisters)
            : Exception{std::format("Illegal instruction: 0x{:08X}", instruction), program_counter, registers, fregisters}
        {
        }

        IllegalInstructionException(
            uint32_t instruction,
            const CPU &cpu)
            : Exception{std::format("Illegal instruction: 0x{:08X}", instruction), cpu.m_program_counter, cpu.m_registers, cpu.m_fregisters}
        {
        }
    };

public:
    CPU(size_t memory_size)
    {
        m_memory.resize(memory_size);
    };
    CPU(const ProgramInfo &info);

    void cycle();
    void load_memory(const uint8_t *source, size_t size, uint32_t start_address);
    void print_registers() const;

    inline uint32_t get_reg(size_t index) { return m_registers[index & 0x1F]; }
    inline void set_reg(size_t index, uint32_t value)
    {
        if (index == REG_X0)
            return;
        m_registers[index & 0x1F] = value;
    }
    inline float get_freg(size_t index) { return m_fregisters[index & 0x1F]; }
    inline void set_freg(size_t index, float value)
    {
        m_fregisters[index & 0x1F] = value;
    }

    inline uint32_t &program_counter() { return m_program_counter; }
    inline void skip_pc_increment() { m_increment_pc = false; }

    template <typename T>
    T *memory_ptr(size_t address)
    {
        return reinterpret_cast<T *>(m_memory.data() + address);
    }
    size_t memory_size() const { return m_memory.size(); }

private:
    std::vector<uint8_t> m_memory;
    std::array<uint32_t, 32> m_registers{};
    std::array<float, 32> m_fregisters{};
    uint32_t m_program_counter = 0;
    bool m_increment_pc = true;
    std::unordered_map<uint32_t, std::function<void(CPU &)>> m_hypercalls;
};