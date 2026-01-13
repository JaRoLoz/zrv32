#pragma once

#include <cstdint>
#include <functional>
#include <unordered_map>
#include "cpu.hpp"

class Hypercalls
{
public:
    using hypercall_handler_t = std::function<int32_t(CPU &)>;
    struct AddHypercall
    {
        AddHypercall(uint32_t address, hypercall_handler_t handler)
        {
            Hypercalls::instance().m_hypercalls[address] = handler;
        }
    };

public:
    friend class AddHypercall;

    Hypercalls(const Hypercalls &) = delete;
    Hypercalls &operator=(const Hypercalls &) = delete;
    Hypercalls(Hypercalls &&) = delete;
    Hypercalls &operator=(Hypercalls &&) = delete;

    static Hypercalls &instance()
    {
        static Hypercalls instance;
        return instance;
    }

    bool check(uint32_t address) { return m_hypercalls.contains(address); }

    void call(uint32_t address, CPU &cpu)
    {
        int32_t result = m_hypercalls[address](cpu);
        cpu.set_reg(CPU::REG_X10, result);
        cpu.program_counter() = cpu.get_reg(CPU::REG_X1);
    }

private:
    Hypercalls() = default;
    ~Hypercalls() = default;

    std::unordered_map<uint32_t, hypercall_handler_t> m_hypercalls;
};