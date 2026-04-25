#pragma once

#include "cpu.h"

void decode_instruction(cpu_t *const restrict cpu);
void decode_cb_instruction(cpu_t *const restrict cpu);