#include "cpu.h"
#include "util.h"
#include "mmu.h"


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

void bus_write(cpu_t *const restrict cpu, uint16_t address, uint8_t value) {
  if (address == TIMER_DIVIDER) {
    // Writing to the divider register resets it to 0
    cpu->_ram[TIMER_DIVIDER] = 0;
    return;
  }
  cpu->_ram[address] = value;
}

uint8_t bus_read(cpu_t *const restrict cpu, uint16_t address) {
  return cpu->_ram[address];
}


void ld_a_r16ref(cpu_t *const restrict cpu) {
  if (cpu->cycles_left == 2) {
    cpu->first_operand = r16_from_opcode(bus_read(cpu, cpu->pc));
    cpu->pc += 1;
  } else if (cpu->cycles_left == 1) {
    uint16_t address = read_r16(cpu, cpu->first_operand);
    cpu->registers[R8_A] = bus_read(cpu, address);
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
    bus_write(cpu, curr_hl, cpu->first_operand);
    cpu->cycles_left -= 1;
  }
}

void ld_r16_imm16(cpu_t *const restrict cpu) {
  if (cpu->cycles_left == 3) {
    cpu->first_operand = r16_from_opcode(bus_read(cpu, cpu->pc));
  } else if (cpu->cycles_left == 2) {
    cpu->second_operand = bus_read(cpu, cpu->pc);
  } else if (cpu->cycles_left == 1) {
    cpu->second_operand |= bus_read(cpu, cpu->pc) << 8;
    store_r16(cpu, cpu->first_operand, cpu->second_operand);
  }
  cpu->pc += 1;
  cpu->cycles_left -= 1;
}

void ld_r16ref_a(cpu_t *const restrict cpu) {
  if (cpu->cycles_left == 2) {
    r16_t r16 = r16_from_opcode(bus_read(cpu, cpu->pc));
    cpu->first_operand = read_r16(cpu, r16);
    cpu->pc += 1;
  } else if (cpu->cycles_left == 1) {
    bus_write(cpu, cpu->first_operand, cpu->registers[R8_A]);
  }
  cpu->cycles_left -= 1;
}

void ld_r8_imm8(cpu_t *const restrict cpu) {
  if (cpu->cycles_left == 2) {
    cpu->first_operand = r8_from_opcode(bus_read(cpu, cpu->pc));
  } else if (cpu->cycles_left == 1) {
    store_r8(cpu, cpu->first_operand, bus_read(cpu, cpu->pc));
  }
  cpu->pc += 1;
  cpu->cycles_left -= 1;
}

void ld_ref16_sp(cpu_t *const restrict cpu) {
  if (cpu->cycles_left == 5) {
    cpu->pc += 1;
  } else if (cpu->cycles_left == 4) {
    cpu->first_operand = bus_read(cpu, cpu->pc);
    cpu->pc += 1;
  } else if (cpu->cycles_left == 3) {
    cpu->first_operand |= bus_read(cpu, cpu->pc) << 8;
    cpu->pc += 1;
  } else if (cpu->cycles_left == 2) {
    bus_write(cpu, cpu->first_operand, (cpu->sp & 0xFF));
  } else if (cpu->cycles_left == 1) {
    bus_write(cpu, cpu->first_operand + 1, (cpu->sp >> 8));
  }
  cpu->cycles_left -= 1;
}

void ld_a_hl_plus_ref(cpu_t *const restrict cpu) {
  if (cpu->cycles_left == 2) {
    uint16_t addr = read_r16(cpu, R16_HL);
    cpu->first_operand = bus_read(cpu, addr);
    store_r16(cpu, R16_HL, addr + 1);
    cpu->cycles_left -= 1;
    cpu->pc += 1;
  } else if (cpu->cycles_left == 1) {
    cpu->registers[R8_A] = cpu->first_operand;
    cpu->cycles_left -= 1;
  }
}

void ld_r8_r8(cpu_t *const restrict cpu) {
  r8_pair_t pair = r8_pair_from_opcode(bus_read(cpu, cpu->pc));
  cpu->registers[pair.first_operand] = cpu->registers[pair.second_operand];
  cpu->pc += 1;
  cpu->cycles_left -= 1;
}

void ld_sp_imm16(cpu_t *const restrict cpu) {
  if (cpu->cycles_left == 2) {
    cpu->sp = bus_read(cpu, cpu->pc);
  } else if (cpu->cycles_left == 1) {
    cpu->sp |= bus_read(cpu, cpu->pc) << 8;
  }
  cpu->pc += 1;
  cpu->cycles_left -= 1;
}

void ld_hl_minus_ref_a(cpu_t *const restrict cpu) {
  if (cpu->cycles_left == 2) {
    cpu->first_operand = cpu->registers[R8_A];
    cpu->pc += 1;
    cpu->cycles_left -= 1;
  } else if (cpu->cycles_left == 1) {
    uint16_t curr_hl = read_r16(cpu, R16_HL);
    store_r16(cpu, R16_HL, curr_hl - 1);
    bus_write(cpu, curr_hl, cpu->first_operand);
    cpu->cycles_left -= 1;
  }
}

void ld_hl_ref_imm8(cpu_t *const restrict cpu) {
  if (cpu->cycles_left == 2) {
    cpu->first_operand = read_r16(cpu, R16_HL);
    cpu->pc += 1;
  } else if (cpu->cycles_left == 1) {
    bus_write(cpu, cpu->first_operand, bus_read(cpu, cpu->pc));
    cpu->pc += 1;
  }
  cpu->cycles_left -= 1;
}

void ld_a_hl_minus_ref(cpu_t *const restrict cpu) {
  if (cpu->cycles_left == 2) {
    uint16_t addr = read_r16(cpu, R16_HL);
    cpu->first_operand = bus_read(cpu, addr);
    store_r16(cpu, R16_HL, addr - 1);
    cpu->cycles_left -= 1;
    cpu->pc += 1;
  } else if (cpu->cycles_left == 1) {
    cpu->registers[R8_A] = cpu->first_operand;
    cpu->cycles_left -= 1;
  }
}

void ld_r8_hl_ref(cpu_t *const restrict cpu) {
  if (cpu->cycles_left == 2) {
    cpu->first_operand = r8_from_opcode(bus_read(cpu, cpu->pc));
    cpu->pc += 1;
  } else if (cpu->cycles_left == 1) {
    uint16_t hl = read_r16(cpu, R16_HL);
    cpu->registers[cpu->first_operand] = bus_read(cpu, hl);
  }
  cpu->cycles_left -= 1;
}

void ld_hl_ref_r8(cpu_t *const restrict cpu) {
  if (cpu->cycles_left == 2) {
    cpu->first_operand = r8_from_opcode(bus_read(cpu, cpu->pc));
    cpu->pc += 1;
  } else if (cpu->cycles_left == 1) {
    uint16_t hl = read_r16(cpu, R16_HL);
    bus_write(cpu, hl, cpu->registers[cpu->first_operand]);
  }
  cpu->cycles_left -= 1;
}

void pop_r16(cpu_t *const restrict cpu) {
  if (cpu->cycles_left == 3) {
    r16_t r16 = r16_from_opcode(bus_read(cpu, cpu->pc));
    r8_pair_t pair = r8_pair_from_r16(r16);
    cpu->first_operand = pair.first_operand;
    cpu->second_operand = pair.second_operand;
    cpu->pc += 1;
  } else if (cpu->cycles_left == 2) {
    cpu->registers[cpu->second_operand] = bus_read(cpu, cpu->sp);
    if (cpu->second_operand == R8_F) {
      cpu->registers[cpu->second_operand] &= 0xF0; // Clear lower 4 bits of F
    }
    cpu->sp += 1;
  } else if (cpu->cycles_left == 1) {
    cpu->registers[cpu->first_operand] = bus_read(cpu, cpu->sp);
    cpu->sp += 1;
  }
  cpu->cycles_left -= 1;
}

void push_r16(cpu_t *const restrict cpu) {
  if (cpu->cycles_left == 3) {
    r16_t r16 = r16_from_opcode(bus_read(cpu, cpu->pc));
    r8_pair_t pair = r8_pair_from_r16(r16);
    cpu->first_operand = pair.first_operand;
    cpu->second_operand = pair.second_operand;
    cpu->pc += 1;
  } else if (cpu->cycles_left == 2) {
    cpu->sp -= 1;
    bus_write(cpu, cpu->sp, cpu->registers[cpu->first_operand]);
  } else if (cpu->cycles_left == 1) {
    cpu->sp -= 1;
    bus_write(cpu, cpu->sp, cpu->registers[cpu->second_operand]);
  }
  cpu->cycles_left -= 1;
}

void ldh_ref8_a(cpu_t *const restrict cpu) {
  if (cpu->cycles_left == 3) {
    cpu->pc += 1;
  } else if (cpu->cycles_left == 2) {
    cpu->first_operand = bus_read(cpu, cpu->pc);
    cpu->pc += 1;
  } else if (cpu->cycles_left == 1) {
    uint8_t offset = cpu->first_operand;
    bus_write(cpu, 0xFF00 + offset, cpu->registers[R8_A]);
  }
  cpu->cycles_left -= 1;
}

void ldh_c_ref_a(cpu_t *const restrict cpu) {
  if (cpu->cycles_left == 2) {
    cpu->first_operand = cpu->registers[R8_C];
    cpu->pc += 1;
  } else if (cpu->cycles_left == 1) {
    uint8_t offset = cpu->first_operand;
    bus_write(cpu, 0xFF00 + offset, cpu->registers[R8_A]);
  }
  cpu->cycles_left -= 1;
}

void ld_ref16_a(cpu_t *const restrict cpu) {
  if (cpu->cycles_left == 4) {
    cpu->pc += 1;
  } else if (cpu->cycles_left == 3) {
    cpu->first_operand = bus_read(cpu, cpu->pc);
    cpu->pc += 1;
  } else if (cpu->cycles_left == 2) {
    cpu->first_operand |= bus_read(cpu, cpu->pc) << 8;
    cpu->pc += 1;
  } else if (cpu->cycles_left == 1) {
    uint16_t address = cpu->first_operand;
    bus_write(cpu, address, cpu->registers[R8_A]);
  }
  cpu->cycles_left -= 1;
}

void ldh_a_ref8(cpu_t *const restrict cpu) {
  if (cpu->cycles_left == 3) {
    cpu->pc += 1;
  } else if (cpu->cycles_left == 2) {
    cpu->first_operand = bus_read(cpu, cpu->pc);
    cpu->pc += 1;
  } else if (cpu->cycles_left == 1) {
    uint8_t offset = cpu->first_operand;
    cpu->registers[R8_A] = bus_read(cpu, 0xFF00 + offset);
  }
  cpu->cycles_left -= 1;
}

void ldh_a_c_ref(cpu_t *const restrict cpu) {
  if (cpu->cycles_left == 2) {
    cpu->first_operand = cpu->registers[R8_C];
    cpu->pc += 1;
  } else if (cpu->cycles_left == 1) {
    uint8_t offset = cpu->first_operand;
    cpu->registers[R8_A] = bus_read(cpu, 0xFF00 + offset);
  }
  cpu->cycles_left -= 1;
}

void ld_hl_sp_plus_s8(cpu_t *const restrict cpu) {
  if (cpu->cycles_left == 3) {
    cpu->pc += 1;
  } else if (cpu->cycles_left == 2) {
    cpu->first_operand = bus_read(cpu, cpu->pc);
    cpu->pc += 1;
  } else if (cpu->cycles_left == 1) {
    uint16_t sp = cpu->sp;
    int8_t e8 = (int8_t)cpu->first_operand;
    uint16_t result = sp + e8;
    uint8_t half_carry =
        OVERFLOW_FROM_BIT3((sp & 0xFF), (uint8_t)e8) ? HALF_CARRY : 0;
    uint8_t carry = OVERFLOW_FROM_BIT7((sp & 0xFF), (uint8_t)e8) ? CARRY : 0;
    cpu->registers[R8_F] = half_carry | carry;
    store_r16(cpu, R16_HL, result);
  }
  cpu->cycles_left -= 1;
}

void ld_sp_hl(cpu_t *const restrict cpu) {
  if (cpu->cycles_left == 2) {
    uint16_t hl = read_r16(cpu, R16_HL);
    cpu->first_operand = hl;
    cpu->pc += 1;
  } else if (cpu->cycles_left == 1) {
    cpu->sp = cpu->first_operand;
  }
  cpu->cycles_left -= 1;
}

void ld_a_ref16(cpu_t *const restrict cpu) {
  if (cpu->cycles_left == 4) {
    cpu->pc += 1;
  } else if (cpu->cycles_left == 3) {
    cpu->first_operand = bus_read(cpu, cpu->pc);
    cpu->pc += 1;
  } else if (cpu->cycles_left == 2) {
    cpu->first_operand |= bus_read(cpu, cpu->pc) << 8;
    cpu->pc += 1;
  } else if (cpu->cycles_left == 1) {
    uint16_t address = cpu->first_operand;
    cpu->registers[R8_A] = bus_read(cpu, address);
  }
  cpu->cycles_left -= 1;
}