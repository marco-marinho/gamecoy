#pragma once

#include "cpu.h"

void inc_r8(cpu_t *const restrict cpu);
void dec_r8(cpu_t *const restrict cpu);
void add_hl_r16(cpu_t *const restrict cpu);
void dec_r16(cpu_t *const restrict cpu);
void inc_r16(cpu_t *const restrict cpu);
void inc_sp(cpu_t *const restrict cpu);
void inc_hl_ref(cpu_t *const restrict cpu);
void dec_hl_ref(cpu_t *const restrict cpu);
void add_hl_sp(cpu_t *const restrict cpu);
void dec_sp(cpu_t *const restrict cpu);