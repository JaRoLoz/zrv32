#include <iostream>
#include <cmath>
#include <chrono>
#include <thread>
#include <fstream>
#include <filesystem>
#include "cpu.hpp"
#include "isa.hpp"
#include "pow.hpp"

/*
    Memory map:
    0x00000000 - 0x0003FFFF -> Program text and data (256 KB)
    0x00040000 - 0x0007FFFF -> Framebuffer (256 KB)
    0x00080000 - 0x000FFFFF -> Stack (512 KB)

    0x80000000 - 0xFFFFFFFF -> Hypercalls
*/

int main(int argc, char **argv)
{
    uint32_t ram_size = ipow(2, 20); // 1 MB

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