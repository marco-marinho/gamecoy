#pragma once

#include "cpu.h"

#define OVERFLOW_FROM_BIT3(a, b)            (((a) & 0x0F) + ((b) & 0x0F) > 0x0F)
#define OVERFLOW_FROM_BIT3_WITH_CARRY(a, b) (((a) & 0x0F) + ((b) & 0x0F) > 0x0E)
#define OVERFLOW_FROM_BIT11(a, b)           (((a) & 0x0FFF) + ((b) & 0x0FFF) > 0x0FFF)
#define OVERFLOW_FROM_BIT15(a, b)           ((uint32_t)(a) + (uint32_t)(b) > 0xFFFF)
#define OVERFLOW_FROM_BIT7(a, b)            ((uint16_t)(a) + (uint16_t)(b) > 0xFF)
#define OVERFLOW_FROM_BIT_WITH_CARRY(a, b)  ((uint16_t)(a) + (uint16_t)(b) > 0xFE)
#define UNDERFLOW_FROM_BIT3(a, b)           (((a) & 0x0F) < ((b) & 0x0F))
#define UNDERFLOW_FROM_BIT7(a, b)           ((a) < (b))
#define UNDERFLOW_FROM_BIT3_WITH_CARRY(a, b) (((a) & 0x0F) < (((b) & 0x0F) + 1))
#define UNDERFLOW_FROM_BIT7_WITH_CARRY(a, b) ((a) < (b) + 1)

typedef struct
{
    r8_t first_operand;
    r8_t second_operand;
} r8_pair_t;

r8_t r8_from_opcode(uint8_t opcode);
r16_t r16_from_opcode(uint8_t opcode);
r8_pair_t r8_pair_from_opcode(uint8_t opcode);
r8_pair_t r8_pair_from_r16(r16_t r16);
uint16_t get_rst_vector(uint8_t opcode);
r8_t r8_from_cb_opcode(uint8_t opcode);
uint8_t cb_load(cpu_t *const restrict cpu, r8_t reg);
void cb_store(cpu_t *const restrict cpu, r8_t reg, uint8_t value);
uint8_t bit_bit(uint8_t opcode);
uint8_t bit_res(uint8_t opcode);
uint8_t bit_set(uint8_t opcode);
