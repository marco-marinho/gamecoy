#pragma once

#include "cpu.h"

typedef struct
{
    r8_t first_operand;
    r8_t second_operand;
} r8_pair_t;

r8_t r8_from_opcode(uint8_t opcode);
r16_t r16_from_opcode(uint8_t opcode);
r8_pair_t r8_pair_from_opcode(uint8_t opcode);