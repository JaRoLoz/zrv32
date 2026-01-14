#pragma once

#include <cstdint>
#include "cpu.hpp"

constexpr const uint32_t M_EXTENSION_FUNC7 = 0x01;

void mul(uint32_t inst, CPU &cpu);
void mulh(uint32_t inst, CPU &cpu);
void mulhsu(uint32_t inst, CPU &cpu);
void mulhu(uint32_t inst, CPU &cpu);
void div_op(uint32_t inst, CPU &cpu);
void divu(uint32_t inst, CPU &cpu);
void rem(uint32_t inst, CPU &cpu);
void remu(uint32_t inst, CPU &cpu);
