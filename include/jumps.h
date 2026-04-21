#pragma once

#include "cpu.h"

void jr_s8(cpu_t *const restrict cpu);
void jr_nz_s8(cpu_t *const restrict cpu);
void jr_z_s8(cpu_t *const restrict cpu);
void jr_nc_s8(cpu_t *const restrict cpu);
void jr_c_s8(cpu_t *const restrict cpu);