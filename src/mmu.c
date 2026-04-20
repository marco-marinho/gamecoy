#include "cpu.h"
#include "util.h"

uint16_t read_r16(cpu_t *const restrict cpu, r16_t r16) {
  switch (r16) {
  case R16_AF:
    return (uint16_t)cpu->registers[R8_A] << 8 | (uint16_t)cpu->registers[R8_F];
  case R16_BC:
    return (uint16_t)cpu->registers[R8_B] << 8 | (uint16_t)cpu->registers[R8_C];
  case R16_DE:
    return (uint16_t)cpu->registers[R8_D] << 8 | (uint16_t)cpu->registers[R8_E];
  case R16_HL:
    return (uint16_t)cpu->registers[R8_H] << 8 | (uint16_t)cpu->registers[R8_L];
  }
  return 0;
}

void store_r16(cpu_t *const restrict cpu, r16_t r16, uint16_t value) {
  switch (r16) {
  case R16_AF:
    cpu->registers[R8_A] = (value >> 8) & 0xFF;
    cpu->registers[R8_F] = value & 0xFF;
    break;
  case R16_BC:
    cpu->registers[R8_B] = (value >> 8) & 0xFF;
    cpu->registers[R8_C] = value & 0xFF;
    break;
  case R16_DE:
    cpu->registers[R8_D] = (value >> 8) & 0xFF;
    cpu->registers[R8_E] = value & 0xFF;
    break;
  case R16_HL:
    cpu->registers[R8_H] = (value >> 8) & 0xFF;
    cpu->registers[R8_L] = value & 0xFF;
    break;
  }
  return;
}

void store_r8(cpu_t *const restrict cpu, r8_t r8, uint8_t value) {
  cpu->registers[r8] = value;
}

void ld_a_r16ref(cpu_t *const restrict cpu) {
  if (cpu->cycles_left == 2) {
    cpu->first_operand = r16_from_opcode(cpu->ram[cpu->pc]);
    cpu->pc += 1;
  } else if (cpu->cycles_left == 1) {
    uint16_t address = read_r16(cpu, cpu->first_operand);
    cpu->registers[R8_A] = cpu->ram[address];
  }
  cpu->cycles_left -= 1;
}

void ld_hl_plus_ref_a(cpu_t *const restrict cpu) {
  if (cpu->cycles_left == 2) {
    cpu->first_operand = cpu->registers[R8_A];
    cpu->pc += 1;
    cpu->cycles_left -= 1;
  } else if (cpu->cycles_left == 1) {
    uint16_t curr_hl = read_r16(cpu, R16_HL);
    store_r16(cpu, R16_HL, curr_hl + 1);
    cpu->ram[curr_hl] = cpu->first_operand;
    cpu->cycles_left -= 1;
  }
}

void ld_r16_imm16(cpu_t *const restrict cpu) {
  if (cpu->cycles_left == 3) {
    cpu->first_operand = r16_from_opcode(cpu->ram[cpu->pc]);
  } else if (cpu->cycles_left == 2) {
    cpu->second_operand = cpu->ram[cpu->pc];
  } else if (cpu->cycles_left == 1) {
    cpu->second_operand |= cpu->ram[cpu->pc] << 8;
    store_r16(cpu, cpu->first_operand, cpu->second_operand);
  }
  cpu->pc += 1;
  cpu->cycles_left -= 1;
}

void ld_r16ref_a(cpu_t *const restrict cpu) {
  if (cpu->cycles_left == 2) {
    r16_t r16 = r16_from_opcode(cpu->ram[cpu->pc]);
    cpu->first_operand = read_r16(cpu, r16);
    cpu->pc += 1;
  } else if (cpu->cycles_left == 1) {
    cpu->ram[cpu->first_operand] = cpu->registers[R8_A];
  }
  cpu->cycles_left -= 1;
}

void ld_r8_imm8(cpu_t *const restrict cpu) {
  if (cpu->cycles_left == 2) {
    cpu->first_operand = r8_from_opcode(cpu->ram[cpu->pc]);
  } else if (cpu->cycles_left == 1) {
    store_r8(cpu, cpu->first_operand, cpu->ram[cpu->pc]);
  }
  cpu->pc += 1;
  cpu->cycles_left -= 1;
}

void ld_imm16ref_sp(cpu_t *const restrict cpu) {
  if (cpu->cycles_left == 5) {
    cpu->pc += 1;
  } else if (cpu->cycles_left == 4) {
    cpu->first_operand = cpu->ram[cpu->pc];
    cpu->pc += 1;
  } else if (cpu->cycles_left == 3) {
    cpu->first_operand |= cpu->ram[cpu->pc] << 8;
    cpu->pc += 1;
  } else if (cpu->cycles_left == 2) {
    cpu->ram[cpu->first_operand] = (cpu->sp & 0xFF);
  } else if (cpu->cycles_left == 1) {
    cpu->ram[cpu->first_operand + 1] = (cpu->sp >> 8);
  }
  cpu->cycles_left -= 1;
}

void ld_a_hl_plus_ref(cpu_t *const restrict cpu) {
  if (cpu->cycles_left == 2) {
    uint16_t addr = read_r16(cpu, R16_HL);
    cpu->first_operand = cpu->ram[addr];
    store_r16(cpu, R16_HL, addr + 1);
    cpu->cycles_left -= 1;
    cpu->pc += 1;
  } else if (cpu->cycles_left == 1) {
    cpu->registers[R8_A] = cpu->first_operand;
    cpu->cycles_left -= 1;
  }
}