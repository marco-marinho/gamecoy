#pragma once

#include "cpu.h"

void jr_s8(cpu_t *const restrict cpu);
void jr_nz_s8(cpu_t *const restrict cpu);
void jr_z_s8(cpu_t *const restrict cpu);
void jr_nc_s8(cpu_t *const restrict cpu);
void jr_c_s8(cpu_t *const restrict cpu);
void ret_nz(cpu_t *const restrict cpu);
void ret_z(cpu_t *const restrict cpu);
void ret(cpu_t *const restrict cpu);
void ret_nc(cpu_t *const restrict cpu);
void ret_c(cpu_t *const restrict cpu);
void reti(cpu_t *const restrict cpu);
void jp_nz_imm16(cpu_t * const restrict cpu);
void jp_imm16(cpu_t * const restrict cpu);
void jp_z_imm16(cpu_t * const restrict cpu);
void jp_nc_imm16(cpu_t * const restrict cpu);
void jp_c_imm16(cpu_t * const restrict cpu);
void call_nz_imm16(cpu_t * const restrict cpu);
void call_z_imm16(cpu_t * const restrict cpu);
void call_imm16(cpu_t * const restrict cpu);
void call_nc_imm16(cpu_t * const restrict cpu);
void call_c_imm16(cpu_t * const restrict cpu);
void rst(cpu_t * const restrict cpu);