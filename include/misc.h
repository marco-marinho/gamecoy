#pragma once

#include "cpu.h"

void nop(cpu_t *const restrict cpu);
void stop(cpu_t *const restrict cpu);
void daa(cpu_t *const restrict cpu);
void scf(cpu_t *const restrict cpu);
void ccf(cpu_t *const restrict cpu);