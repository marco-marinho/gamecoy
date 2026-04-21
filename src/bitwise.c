#include "cpu.h"

void cpl(cpu_t *const restrict cpu)
{
  cpu->registers[R8_A] = ~cpu->registers[R8_A];
  cpu->registers[R8_F] |= SUBTRACT | HALF_CARRY;
  cpu->pc += 1;
  cpu->cycles_left -= 1;
}