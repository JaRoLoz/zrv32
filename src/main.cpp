#include <iostream>
#include <cmath>
#include <chrono>
#include <thread>
#include <fstream>
#include <filesystem>
#include "cpu.hpp"
#include "isa.hpp"
#include "pow.hpp"

int main(int argc, char **argv)
{
    uint32_t ram_size = 1024 * 1024; // 1 MB

    CPU cpu{CPU::ProgramInfo{
        .path = std::filesystem::path{argv[1]},
        .load_address = 0x00000000,
        .entry_point = 0x00000000,
        .memory_size = ram_size}};

    while (true)
    {
        cpu.cycle();
    }
    return 0;
}