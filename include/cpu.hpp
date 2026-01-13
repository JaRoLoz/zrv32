#pragma once

#include <cstdint>
#include <filesystem>
#include <cmath>
#include <unordered_map>
#include <vector>
#include <array>
#include <string>
#include <functional>

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

public:
    CPU(size_t memory_size)
    {
        m_memory.resize(memory_size);
    };
    CPU(const ProgramInfo &info);

    void cycle();
    void load_memory(const uint8_t *source, size_t size, uint32_t start_address);
    void print_registers() const;
    void add_hypercall(uint32_t address, std::function<void(CPU &)> func)
    {
        m_hypercalls[address] = func;
    }
    void remove_hypercall(uint32_t address)
    {
        m_hypercalls.erase(address);
    }

    inline uint32_t get_reg(size_t index) { return m_registers[index]; }
    inline void set_reg(size_t index, uint32_t value)
    {
        if (index == REG_X0)
            return;
        m_registers[index] = value;
    }

    inline uint32_t &program_counter() { return m_program_counter; }
    inline void skip_pc_increment() { m_increment_pc = false; }

    template <typename T>
    T *memory_ptr(size_t address) { return reinterpret_cast<T *>(m_memory.data() + address); }
    size_t memory_size() const { return m_memory.size(); }

private:
    bool check_hypercall(uint32_t address)
    {
        if (!m_hypercalls.contains(address))
        {
            return false;
        }

        m_hypercalls[address](*this);
        m_program_counter = m_registers[REG_X1];
        return true;
    }

private:
    std::vector<uint8_t> m_memory;
    std::array<uint32_t, 32> m_registers{};
    uint32_t m_program_counter = 0;
    bool m_increment_pc = true;
    std::unordered_map<uint32_t, std::function<void(CPU &)>> m_hypercalls;
};