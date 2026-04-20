#include "cpu.h"
#include "mmu.h"
#include "util.h"

static inline uint8_t overflow_from_bit3(uint8_t a, uint8_t b) {
  return ((a & 0x0F) + (b & 0x0F)) > 0x0F;
}

static inline uint8_t overflow_from_bit11(uint16_t a, uint16_t b) {
  return ((a & 0x0FFF) + (b & 0x0FFF)) > 0x0FFF;
}

static inline uint8_t overflow_from_bit15(uint16_t a, uint16_t b) {
  return ((uint32_t)a + (uint32_t)b) > 0xFFFF;
}

static inline uint8_t underflow_from_bit3(uint8_t a, uint8_t b) {
  return (a & 0x0F) < (b & 0x0F);
}

void inc_r8(cpu_t *const restrict cpu) {
  r8_t target = r8_from_opcode(cpu->ram[cpu->pc]);
  uint8_t old = cpu->registers[target];
  uint8_t new = old + 1;
  uint8_t half_carry = overflow_from_bit3(old, 1) ? HALF_CARRY : 0;
  cpu->registers[target] = new;
  uint8_t zero = new == 0 ? ZERO : 0;
  uint8_t current_carry = cpu->registers[R8_F] & CARRY ? CARRY : 0;
  cpu->registers[R8_F] = zero | half_carry | current_carry;
  cpu->pc += 1;
  cpu->cycles_left -= 1;
}

void dec_r8(cpu_t *const restrict cpu) {
  r8_t target = r8_from_opcode(cpu->ram[cpu->pc]);
  uint8_t old = cpu->registers[target];
  uint8_t new = old - 1;
  uint8_t half_carry = underflow_from_bit3(old, 1) ? HALF_CARRY : 0;
  cpu->registers[target] = new;
  uint8_t zero = new == 0 ? ZERO : 0;
  uint8_t current_carry = cpu->registers[R8_F] & CARRY ? CARRY : 0;
  cpu->registers[R8_F] = zero | SUBTRACT | half_carry | current_carry;
  cpu->pc += 1;
  cpu->cycles_left -= 1;
}

void add_hl_r16(cpu_t *const restrict cpu) {
  if (cpu->cycles_left == 2) {
    cpu->first_operand = r16_from_opcode(cpu->ram[cpu->pc]);
    cpu->pc += 1;
  } else if (cpu->cycles_left == 1) {
    uint16_t hl = read_r16(cpu, R16_HL);
    uint16_t value = read_r16(cpu, cpu->first_operand);
    uint16_t result = hl + value;
    store_r16(cpu, R16_HL, result);
    uint8_t half_carry = overflow_from_bit11(hl, value) ? HALF_CARRY : 0;
    uint8_t carry = overflow_from_bit15(hl, value) ? CARRY : 0;
    cpu->registers[R8_F] = (cpu->registers[R8_F] & ZERO) | half_carry | carry;
  }
  cpu->cycles_left -= 1;
}

void inc_r16(cpu_t *const restrict cpu) {
  if (cpu->cycles_left == 2) {
    cpu->first_operand = r16_from_opcode(cpu->ram[cpu->pc]);
    cpu->pc += 1;
  } else if (cpu->cycles_left == 1) {
    uint16_t value = read_r16(cpu, cpu->first_operand);
    store_r16(cpu, cpu->first_operand, value + 1);
  }
  cpu->cycles_left -= 1;
}

void dec_r16(cpu_t *const restrict cpu) {
  if (cpu->cycles_left == 2) {
    cpu->first_operand = r16_from_opcode(cpu->ram[cpu->pc]);
    cpu->pc += 1;
  } else if (cpu->cycles_left == 1) {
    uint16_t current = read_r16(cpu, cpu->first_operand);
    uint16_t new = current - 1;
    store_r16(cpu, cpu->first_operand, new);
  }
  cpu->cycles_left -= 1;
}
