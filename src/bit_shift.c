#include "cpu.h"
#include "util.h"
#include <stdint.h>

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

void rlc_r8(cpu_t *const restrict cpu) {
  if (cpu->cycles_left == 1) {
    r8_t reg = r8_from_cb_opcode(cpu->ram[cpu->pc]);
    uint8_t value = cb_load(cpu, reg);
    uint8_t carry = (value >> 7) & 0x01;
    uint8_t result = (value << 1) | carry;
    uint8_t zero_flag = result == 0 ? ZERO : 0;
    uint8_t carry_flag = carry ? CARRY : 0;
    cb_store(cpu, reg, result);
    cpu->registers[R8_F] = zero_flag | carry_flag;
    cpu->pc += 1;
  }
  cpu->cycles_left -= 1;
}

void rrc_r8(cpu_t *const restrict cpu) {
  if (cpu->cycles_left == 1) {
    r8_t reg = r8_from_cb_opcode(cpu->ram[cpu->pc]);
    uint8_t value = cb_load(cpu, reg);
    uint8_t carry = value & 0x01;
    uint8_t result = (value >> 1) | (carry << 7);
    uint8_t zero_flag = result == 0 ? ZERO : 0;
    uint8_t carry_flag = carry ? CARRY : 0;
    cb_store(cpu, reg, result);
    cpu->registers[R8_F] = zero_flag | carry_flag;
    cpu->pc += 1;
  }
  cpu->cycles_left -= 1;
}