#pragma once

#include "cpu.h"

void nop(cpu_t *const restrict cpu);
void stop(cpu_t *const restrict cpu);
void halt(cpu_t *const restrict cpu);
void daa(cpu_t *const restrict cpu);
void scf(cpu_t *const restrict cpu);
void ccf(cpu_t *const restrict cpu);
void di(cpu_t *const restrict cpu);
void ei(cpu_t *const restrict cpu);