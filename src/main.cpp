#include <iostream>
#include <cmath>
#include <chrono>
#include <thread>
#include <fstream>
#include <filesystem>
#include "cpu.hpp"
#include "isa.hpp"
#include "pow.hpp"

bool running = true;

void rv_putc(CPU &cpu)
{
    char ch = static_cast<char>(cpu.get_reg(CPU::REG_X10) & 0xFF);
    std::cout << ch;

    cpu.set_reg(CPU::REG_X10, 0);
}

void rv_memset(CPU &cpu)
{
    uint32_t dest = cpu.get_reg(CPU::REG_X10);
    uint8_t value = static_cast<uint8_t>(cpu.get_reg(CPU::REG_X11) & 0xFF);
    size_t length = cpu.get_reg(CPU::REG_X12);

    if (dest + length > cpu.memory_size())
    {
        cpu.set_reg(CPU::REG_X10, 1);
        return;
    }

    std::memset(cpu.memory_ptr<uint8_t>(dest), value, length);
    cpu.set_reg(CPU::REG_X10, length);
}

void rv_exit(CPU &cpu)
{
    std::cout << "Exiting program through hypercall.\n";
    running = false;
    cpu.set_reg(CPU::REG_X10, 0);
}

int main(int argc, char **argv)
{
    uint32_t ram_size = 1024 * 1024; // 1 MB

    CPU cpu{CPU::ProgramInfo{
        .path = std::filesystem::path{argv[1]},
        .load_address = 0x00000000,
        .entry_point = 0x00000000,
        .memory_size = ram_size}};

    cpu.add_hypercall(0xFFFFFFF0, rv_memset);
    cpu.add_hypercall(0xFFFFFFF4, rv_putc);
    cpu.add_hypercall(0xFFFFFFF8, rv_exit);

    while (running)
    {
        cpu.cycle();
    }
    return 0;
}