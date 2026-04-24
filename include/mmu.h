#pragma once

#include "cpu.h"

uint16_t read_r16(cpu_t *const restrict cpu, r16_t r16);
void store_r16(cpu_t *const restrict cpu, r16_t r16, uint16_t value);
void store_r8(cpu_t *const restrict cpu, r8_t r8, uint8_t value);
void ld_a_r16ref(cpu_t *const restrict cpu);
void ld_hl_plus_ref_a(cpu_t *const restrict cpu);
void ld_r16_imm16(cpu_t *const restrict cpu);
void ld_r16ref_a(cpu_t *const restrict cpu);
void ld_r8_imm8(cpu_t *const restrict cpu);
void ld_imm16ref_sp(cpu_t *const restrict cpu);
void ld_a_hl_plus_ref(cpu_t *const restrict cpu);
void ld_r8_r8(cpu_t *const restrict cpu);
void ld_sp_imm16(cpu_t *const restrict cpu);
void ld_hl_minus_ref_a(cpu_t *const restrict cpu);
void ld_hl_ref_imm8(cpu_t *const restrict cpu);
void ld_a_hl_minus_ref(cpu_t *const restrict cpu);
void ld_r8_hl_ref(cpu_t *const restrict cpu);
void ld_hl_ref_r8(cpu_t *const restrict cpu);
void pop_r16(cpu_t *const restrict cpu);
void push_r16(cpu_t *const restrict cpu);
void ldh_imm8_ref_a(cpu_t *const restrict cpu);
void ldh_c_ref_a(cpu_t *const restrict cpu);