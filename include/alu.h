#pragma once

#include "cpu.h"

void inc_r8(cpu_t *const restrict cpu);
void dec_r8(cpu_t *const restrict cpu);
void add_hl_r16(cpu_t *const restrict cpu);
void dec_r16(cpu_t *const restrict cpu);
void inc_r16(cpu_t *const restrict cpu);