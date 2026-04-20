#include "cpu.h"

void rlca(cpu_t *const restrict cpu) {
  uint8_t a = cpu->registers[R8_A];
  uint8_t carry = (a >> 7) & 0x01;
  cpu->registers[R8_A] = (a << 1) | carry;
  cpu->registers[R8_F] = carry ? CARRY : 0;
  cpu->pc += 1;
  cpu->cycles_left -= 1;
}

void rrca(cpu_t *const restrict cpu) {
  uint8_t a = cpu->registers[R8_A];
  uint8_t carry = a & 0x01;
  cpu->registers[R8_A] = (a >> 1) | (carry << 7);
  cpu->registers[R8_F] = carry ? CARRY : 0;
  cpu->pc += 1;
  cpu->cycles_left -= 1;
}

void rla(cpu_t *const restrict cpu) {
  uint8_t a = cpu->registers[R8_A];
  uint8_t current_carry = cpu->registers[R8_F] & CARRY ? 1 : 0;
  uint8_t new_carry = (a >> 7) & 0x01;
  cpu->registers[R8_A] = (a << 1) | current_carry;
  cpu->registers[R8_F] = new_carry ? CARRY : 0;
  cpu->pc += 1;
  cpu->cycles_left -= 1;
}

void rra(cpu_t *const restrict cpu) {
  uint8_t a = cpu->registers[R8_A];
  uint8_t current_carry = cpu->registers[R8_F] & CARRY ? 1 : 0;
  uint8_t new_carry = a & 0x01;
  cpu->registers[R8_A] = (a >> 1) | (current_carry << 7);
  cpu->registers[R8_F] = new_carry ? CARRY : 0;
  cpu->pc += 1;
  cpu->cycles_left -= 1;
}