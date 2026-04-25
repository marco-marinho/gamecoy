#pragma once

#include "cpu.h"

void rlca(cpu_t *const restrict cpu);
void rrca(cpu_t *const restrict cpu);
void rla(cpu_t *const restrict cpu);
void rra(cpu_t *const restrict cpu);
void rlc_r8(cpu_t *const restrict cpu);
void rrc_r8(cpu_t *const restrict cpu);
void rl_r8(cpu_t *const restrict cpu);
void rr_r8(cpu_t *const restrict cpu);
void sla_r8(cpu_t *const restrict cpu);
void sra_r8(cpu_t *const restrict cpu);
void swap_r8(cpu_t *const restrict cpu);
void srl_r8(cpu_t *const restrict cpu);