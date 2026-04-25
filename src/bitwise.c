#include "cpu.h"
#include "mmu.h"
#include "util.h"

void cpl(cpu_t *const restrict cpu) {
  cpu->registers[R8_A] = ~cpu->registers[R8_A];
  cpu->registers[R8_F] |= SUBTRACT | HALF_CARRY;
  cpu->pc += 1;
  cpu->cycles_left -= 1;
}

void and_a_r8(cpu_t *const restrict cpu) {
  r8_t target_reg = r8_from_opcode(cpu->ram[cpu->pc]);
  uint8_t a = cpu->registers[R8_A];
  uint8_t value = cpu->registers[target_reg];
  uint8_t result = a & value;
  cpu->registers[R8_A] = result;
  uint8_t zero = result == 0 ? ZERO : 0;
  cpu->registers[R8_F] = zero | HALF_CARRY;
  cpu->pc += 1;
  cpu->cycles_left -= 1;
}

void and_a_hl_ref(cpu_t *const restrict cpu) {
  if (cpu->cycles_left == 2) {
    cpu->first_operand = cpu->ram[read_r16(cpu, R16_HL)];
    cpu->pc += 1;
  } else if (cpu->cycles_left == 1) {
    uint8_t a = cpu->registers[R8_A];
    uint8_t result = a & cpu->first_operand;
    cpu->registers[R8_A] = result;
    uint8_t zero = result == 0 ? ZERO : 0;
    cpu->registers[R8_F] = zero | HALF_CARRY;
  }
  cpu->cycles_left -= 1;
}

void xor_a_r8(cpu_t *const restrict cpu) {
  r8_t target_reg = r8_from_opcode(cpu->ram[cpu->pc]);
  uint8_t a = cpu->registers[R8_A];
  uint8_t value = cpu->registers[target_reg];
  uint8_t result = a ^ value;
  cpu->registers[R8_A] = result;
  uint8_t zero = result == 0 ? ZERO : 0;
  cpu->registers[R8_F] = zero;
  cpu->pc += 1;
  cpu->cycles_left -= 1;
}

void xor_a_hl_ref(cpu_t *const restrict cpu) {
  if (cpu->cycles_left == 2) {
    cpu->first_operand = cpu->ram[read_r16(cpu, R16_HL)];
    cpu->pc += 1;
  } else if (cpu->cycles_left == 1) {
    uint8_t a = cpu->registers[R8_A];
    uint8_t result = a ^ cpu->first_operand;
    cpu->registers[R8_A] = result;
    uint8_t zero = result == 0 ? ZERO : 0;
    cpu->registers[R8_F] = zero;
  }
  cpu->cycles_left -= 1;
}

void or_a_r8(cpu_t *const restrict cpu) {
  r8_t target_reg = r8_from_opcode(cpu->ram[cpu->pc]);
  uint8_t a = cpu->registers[R8_A];
  uint8_t value = cpu->registers[target_reg];
  uint8_t result = a | value;
  cpu->registers[R8_A] = result;
  uint8_t zero = result == 0 ? ZERO : 0;
  cpu->registers[R8_F] = zero;
  cpu->pc += 1;
  cpu->cycles_left -= 1;
}

void or_a_hl_ref(cpu_t *const restrict cpu) {
  if (cpu->cycles_left == 2) {
    cpu->first_operand = cpu->ram[read_r16(cpu, R16_HL)];
    cpu->pc += 1;
  } else if (cpu->cycles_left == 1) {
    uint8_t a = cpu->registers[R8_A];
    uint8_t result = a | cpu->first_operand;
    cpu->registers[R8_A] = result;
    uint8_t zero = result == 0 ? ZERO : 0;
    cpu->registers[R8_F] = zero;
  }
  cpu->cycles_left -= 1;
}

void cp_a_r8(cpu_t *const restrict cpu) {
  r8_t target_reg = r8_from_opcode(cpu->ram[cpu->pc]);
  uint8_t a = cpu->registers[R8_A];
  uint8_t value = cpu->registers[target_reg];
  uint8_t half_carry = UNDERFLOW_FROM_BIT3(a, value) ? HALF_CARRY : 0;
  uint8_t carry = UNDERFLOW_FROM_BIT7(a, value) ? CARRY : 0;
  uint8_t zero = (a - value) == 0 ? ZERO : 0;
  cpu->registers[R8_F] = zero | half_carry | carry | SUBTRACT;
  cpu->pc += 1;
  cpu->cycles_left -= 1;
}

void cp_a_hl_ref(cpu_t *const restrict cpu) {
  if (cpu->cycles_left == 2) {
    cpu->first_operand = cpu->ram[read_r16(cpu, R16_HL)];
    cpu->pc += 1;
  } else if (cpu->cycles_left == 1) {
    uint8_t a = cpu->registers[R8_A];
    uint8_t value = cpu->first_operand;
    uint8_t half_carry = UNDERFLOW_FROM_BIT3(a, value) ? HALF_CARRY : 0;
    uint8_t carry = UNDERFLOW_FROM_BIT7(a, value) ? CARRY : 0;
    uint8_t zero = (a - value) == 0 ? ZERO : 0;
    cpu->registers[R8_F] = zero | half_carry | carry | SUBTRACT;
  }
  cpu->cycles_left -= 1;
}

void and_a_imm8(cpu_t *const restrict cpu) {
  if (cpu->cycles_left == 1) {
    uint8_t a = cpu->registers[R8_A];
    uint8_t value = cpu->ram[cpu->pc];
    uint8_t result = a & value;
    cpu->registers[R8_A] = result;
    uint8_t zero = result == 0 ? ZERO : 0;
    cpu->registers[R8_F] = zero | HALF_CARRY;
  }
  cpu->pc += 1;
  cpu->cycles_left -= 1;
}

void xor_a_imm8(cpu_t *const restrict cpu) {
  if (cpu->cycles_left == 1) {
    uint8_t a = cpu->registers[R8_A];
    uint8_t value = cpu->ram[cpu->pc];
    uint8_t result = a ^ value;
    cpu->registers[R8_A] = result;
    uint8_t zero = result == 0 ? ZERO : 0;
    cpu->registers[R8_F] = zero;
  }
  cpu->pc += 1;
  cpu->cycles_left -= 1;
}

void or_a_imm8(cpu_t *const restrict cpu) {
  if (cpu->cycles_left == 1) {
    uint8_t a = cpu->registers[R8_A];
    uint8_t value = cpu->ram[cpu->pc];
    uint8_t result = a | value;
    cpu->registers[R8_A] = result;
    uint8_t zero = result == 0 ? ZERO : 0;
    cpu->registers[R8_F] = zero;
  }
  cpu->pc += 1;
  cpu->cycles_left -= 1;
}