#pragma once

#include "cpu.h"

void cpl(cpu_t *const restrict cpu);
void and_a_r8(cpu_t *const restrict cpu);
void and_a_hl_ref(cpu_t *const restrict cpu);
void and_a_imm8(cpu_t *const restrict cpu);
void xor_a_r8(cpu_t *const restrict cpu);
void xor_a_hl_ref(cpu_t *const restrict cpu);
void xor_a_imm8(cpu_t *const restrict cpu);
void or_a_r8(cpu_t *const restrict cpu);
void or_a_hl_ref(cpu_t *const restrict cpu);
void or_a_imm8(cpu_t *const restrict cpu);
void cp_a_r8(cpu_t *const restrict cpu);
void cp_a_hl_ref(cpu_t *const restrict cpu);