#include "cpu.h"
#include "util.h"
#include "mmu.h"

void bit_pos_r8(cpu_t *const restrict cpu) {
  if (cpu->cycles_left == 1) {
    uint8_t memory_value = bus_read(cpu, cpu->pc);
    r8_t reg = r8_from_cb_opcode(memory_value);
    uint8_t bit_pos = bit_bit(memory_value);
    uint8_t value = cb_load(cpu, reg);
    uint8_t bit_value = (value >> bit_pos) & 0x01;
    uint8_t zero_flag = bit_value == 0 ? ZERO : 0;
    uint8_t current_carry = cpu->registers[R8_F] & CARRY;
    cpu->registers[R8_F] = zero_flag | HALF_CARRY | current_carry;
    cpu->pc += 1;
  }
  cpu->cycles_left -= 1;
}

void res_pos_r8(cpu_t *const restrict cpu) {
  if (cpu->cycles_left == 1) {
    uint8_t memory_value = bus_read(cpu, cpu->pc);
    r8_t reg = r8_from_cb_opcode(memory_value);
    uint8_t bit_pos = bit_res(memory_value);
    uint8_t value = cb_load(cpu, reg);
    uint8_t mask = ~(1 << bit_pos);
    uint8_t result = value & mask;
    cb_store(cpu, reg, result);
    cpu->pc += 1;
  }
  cpu->cycles_left -= 1;
}

void set_pos_r8(cpu_t *const restrict cpu) {
  if (cpu->cycles_left == 1) {
    uint8_t memory_value = bus_read(cpu, cpu->pc);
    r8_t reg = r8_from_cb_opcode(memory_value);
    uint8_t bit_pos = bit_set(memory_value);
    uint8_t value = cb_load(cpu, reg);
    uint8_t mask = 1 << bit_pos;
    uint8_t result = value | mask;
    cb_store(cpu, reg, result);
    cpu->pc += 1;
  }
  cpu->cycles_left -= 1;
}
