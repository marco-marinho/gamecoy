#include "cpu.h"
#include "mmu.h"
#include "util.h"
#include <stdint.h>

void inc_r8(cpu_t *const restrict cpu) {
  r8_t target = r8_from_opcode(bus_read(cpu, cpu->pc));
  uint8_t old = cpu->registers[target];
  uint8_t new = old + 1;
  uint8_t half_carry = OVERFLOW_FROM_BIT3(old, 1) ? HALF_CARRY : 0;
  cpu->registers[target] = new;
  uint8_t zero = new == 0 ? ZERO : 0;
  uint8_t current_carry = cpu->registers[R8_F] & CARRY ? CARRY : 0;
  cpu->registers[R8_F] = zero | half_carry | current_carry;
  cpu->pc += 1;
  cpu->cycles_left -= 1;
}

void dec_r8(cpu_t *const restrict cpu) {
  r8_t target = r8_from_opcode(bus_read(cpu, cpu->pc));
  uint8_t old = cpu->registers[target];
  uint8_t new = old - 1;
  uint8_t half_carry = UNDERFLOW_FROM_BIT3(old, 1) ? HALF_CARRY : 0;
  cpu->registers[target] = new;
  uint8_t zero = new == 0 ? ZERO : 0;
  uint8_t current_carry = cpu->registers[R8_F] & CARRY ? CARRY : 0;
  cpu->registers[R8_F] = zero | SUBTRACT | half_carry | current_carry;
  cpu->pc += 1;
  cpu->cycles_left -= 1;
}

void add_hl_r16(cpu_t *const restrict cpu) {
  if (cpu->cycles_left == 2) {
    cpu->first_operand = r16_from_opcode(bus_read(cpu, cpu->pc));
    cpu->pc += 1;
  } else if (cpu->cycles_left == 1) {
    uint16_t hl = read_r16(cpu, R16_HL);
    uint16_t value = read_r16(cpu, cpu->first_operand);
    uint16_t result = hl + value;
    store_r16(cpu, R16_HL, result);
    uint8_t half_carry = OVERFLOW_FROM_BIT11(hl, value) ? HALF_CARRY : 0;
    uint8_t carry = OVERFLOW_FROM_BIT15(hl, value) ? CARRY : 0;
    cpu->registers[R8_F] = (cpu->registers[R8_F] & ZERO) | half_carry | carry;
  }
  cpu->cycles_left -= 1;
}

void inc_r16(cpu_t *const restrict cpu) {
  if (cpu->cycles_left == 2) {
    cpu->first_operand = r16_from_opcode(bus_read(cpu, cpu->pc));
    cpu->pc += 1;
  } else if (cpu->cycles_left == 1) {
    uint16_t value = read_r16(cpu, cpu->first_operand);
    store_r16(cpu, cpu->first_operand, value + 1);
  }
  cpu->cycles_left -= 1;
}

void dec_r16(cpu_t *const restrict cpu) {
  if (cpu->cycles_left == 2) {
    cpu->first_operand = r16_from_opcode(bus_read(cpu, cpu->pc));
    cpu->pc += 1;
  } else if (cpu->cycles_left == 1) {
    uint16_t current = read_r16(cpu, cpu->first_operand);
    uint16_t new = current - 1;
    store_r16(cpu, cpu->first_operand, new);
  }
  cpu->cycles_left -= 1;
}

void inc_sp(cpu_t *const restrict cpu) {
  if (cpu->cycles_left == 1) {
    cpu->sp += 1;
    cpu->pc += 1;
  }
  cpu->cycles_left -= 1;
}

void inc_hl_ref(cpu_t *const restrict cpu) {
  if (cpu->cycles_left == 2) {
    uint16_t hl = read_r16(cpu, R16_HL);
    cpu->first_operand = bus_read(cpu, hl);
    cpu->pc += 1;
  } else if (cpu->cycles_left == 1) {
    uint8_t old = cpu->first_operand;
    uint8_t new = old + 1;
    uint8_t half_carry = OVERFLOW_FROM_BIT3(old, 1) ? HALF_CARRY : 0;
    bus_write(cpu, read_r16(cpu, R16_HL), new);
    uint8_t zero = new == 0 ? ZERO : 0;
    uint8_t current_carry = cpu->registers[R8_F] & CARRY ? CARRY : 0;
    cpu->registers[R8_F] = zero | half_carry | current_carry;
  }
  cpu->cycles_left -= 1;
}

void dec_hl_ref(cpu_t *const restrict cpu) {
  if (cpu->cycles_left == 2) {
    uint16_t hl = read_r16(cpu, R16_HL);
    cpu->first_operand = bus_read(cpu, hl);
    cpu->pc += 1;
  } else if (cpu->cycles_left == 1) {
    uint8_t old = cpu->first_operand;
    uint8_t new = old - 1;
    uint8_t half_carry = UNDERFLOW_FROM_BIT3(old, 1) ? HALF_CARRY : 0;
    bus_write(cpu, read_r16(cpu, R16_HL), new);
    uint8_t zero = new == 0 ? ZERO : 0;
    uint8_t current_carry = cpu->registers[R8_F] & CARRY ? CARRY : 0;
    cpu->registers[R8_F] = zero | SUBTRACT | half_carry | current_carry;
  }
  cpu->cycles_left -= 1;
}

void add_hl_sp(cpu_t *const restrict cpu) {
  if (cpu->cycles_left == 2) {
    cpu->pc += 1;
  } else if (cpu->cycles_left == 1) {
    uint16_t hl = read_r16(cpu, R16_HL);
    uint16_t value = cpu->sp;
    uint16_t result = hl + value;
    store_r16(cpu, R16_HL, result);
    uint8_t half_carry = OVERFLOW_FROM_BIT11(hl, value) ? HALF_CARRY : 0;
    uint8_t carry = OVERFLOW_FROM_BIT15(hl, value) ? CARRY : 0;
    cpu->registers[R8_F] = (cpu->registers[R8_F] & ZERO) | half_carry | carry;
  }
  cpu->cycles_left -= 1;
}

void dec_sp(cpu_t *const restrict cpu) {
  if (cpu->cycles_left == 1) {
    cpu->sp -= 1;
    cpu->pc += 1;
  }
  cpu->cycles_left -= 1;
}

void add_a_r8(cpu_t *const restrict cpu) {
  r8_t target_reg = r8_from_opcode(bus_read(cpu, cpu->pc));
  uint8_t a = cpu->registers[R8_A];
  uint8_t value = cpu->registers[target_reg];
  uint8_t result = a + value;
  cpu->registers[R8_A] = result;
  uint8_t half_carry = OVERFLOW_FROM_BIT3(a, value) ? HALF_CARRY : 0;
  uint8_t carry = OVERFLOW_FROM_BIT7(a, value) ? CARRY : 0;
  uint8_t zero = result == 0 ? ZERO : 0;
  cpu->registers[R8_F] = zero | half_carry | carry;
  cpu->pc += 1;
  cpu->cycles_left -= 1;
}

void add_a_hl_ref(cpu_t *const restrict cpu) {
  if (cpu->cycles_left == 2) {
    cpu->first_operand = bus_read(cpu, read_r16(cpu, R16_HL));
    cpu->pc += 1;
  } else if (cpu->cycles_left == 1) {
    uint8_t a = cpu->registers[R8_A];
    uint8_t value = cpu->first_operand;
    uint8_t result = a + value;
    cpu->registers[R8_A] = result;
    uint8_t half_carry = OVERFLOW_FROM_BIT3(a, value) ? HALF_CARRY : 0;
    uint8_t carry = OVERFLOW_FROM_BIT7(a, value) ? CARRY : 0;
    uint8_t zero = result == 0 ? ZERO : 0;
    cpu->registers[R8_F] = zero | half_carry | carry;
  }
  cpu->cycles_left -= 1;
}

void adc_a_r8(cpu_t *const restrict cpu) {
  r8_t target_reg = r8_from_opcode(bus_read(cpu, cpu->pc));
  uint8_t a = cpu->registers[R8_A];
  uint8_t value = cpu->registers[target_reg];
  uint8_t carry_in = cpu->registers[R8_F] & CARRY ? 1 : 0;
  uint8_t result = a + value + carry_in;
  cpu->registers[R8_A] = result;
  uint8_t half_carry;
  if (carry_in) {
    half_carry = OVERFLOW_FROM_BIT3_WITH_CARRY(a, value) ? HALF_CARRY : 0;
  } else {
    half_carry = OVERFLOW_FROM_BIT3(a, value) ? HALF_CARRY : 0;
  }
  uint8_t carry;
  if (carry_in) {
    carry = OVERFLOW_FROM_BIT_WITH_CARRY(a, value) ? CARRY : 0;
  } else {
    carry = OVERFLOW_FROM_BIT7(a, value) ? CARRY : 0;
  }
  uint8_t zero = result == 0 ? ZERO : 0;
  cpu->registers[R8_F] = zero | half_carry | carry;
  cpu->pc += 1;
  cpu->cycles_left -= 1;
}

void adc_a_hl_ref(cpu_t *const restrict cpu) {
  if (cpu->cycles_left == 2) {
    cpu->first_operand = bus_read(cpu, read_r16(cpu, R16_HL));
    cpu->pc += 1;
  } else if (cpu->cycles_left == 1) {
    uint8_t a = cpu->registers[R8_A];
    uint8_t value = cpu->first_operand;
    uint8_t carry_in = cpu->registers[R8_F] & CARRY ? 1 : 0;
    uint8_t result = a + value + carry_in;
    cpu->registers[R8_A] = result;
    uint8_t half_carry;
    if (carry_in) {
      half_carry = OVERFLOW_FROM_BIT3_WITH_CARRY(a, value) ? HALF_CARRY : 0;
    } else {
      half_carry = OVERFLOW_FROM_BIT3(a, value) ? HALF_CARRY : 0;
    }
    uint8_t carry;
    if (carry_in) {
      carry = OVERFLOW_FROM_BIT_WITH_CARRY(a, value) ? CARRY : 0;
    } else {
      carry = OVERFLOW_FROM_BIT7(a, value) ? CARRY : 0;
    }
    uint8_t zero = result == 0 ? ZERO : 0;
    cpu->registers[R8_F] = zero | half_carry | carry;
  }
  cpu->cycles_left -= 1;
}

void sub_a_r8(cpu_t *const restrict cpu) {
  r8_t target_reg = r8_from_opcode(bus_read(cpu, cpu->pc));
  uint8_t a = cpu->registers[R8_A];
  uint8_t value = cpu->registers[target_reg];
  uint8_t result = a - value;
  cpu->registers[R8_A] = result;
  uint8_t half_carry = UNDERFLOW_FROM_BIT3(a, value) ? HALF_CARRY : 0;
  uint8_t carry = UNDERFLOW_FROM_BIT7(a, value) ? CARRY : 0;
  uint8_t zero = result == 0 ? ZERO : 0;
  cpu->registers[R8_F] = zero | half_carry | carry | SUBTRACT;
  cpu->pc += 1;
  cpu->cycles_left -= 1;
}

void sub_a_hl_ref(cpu_t *const restrict cpu) {
  if (cpu->cycles_left == 2) {
    cpu->first_operand = bus_read(cpu, read_r16(cpu, R16_HL));
    cpu->pc += 1;
  } else if (cpu->cycles_left == 1) {
    uint8_t a = cpu->registers[R8_A];
    uint8_t value = cpu->first_operand;
    uint8_t result = a - value;
    cpu->registers[R8_A] = result;
    uint8_t half_carry = UNDERFLOW_FROM_BIT3(a, value) ? HALF_CARRY : 0;
    uint8_t carry = UNDERFLOW_FROM_BIT7(a, value) ? CARRY : 0;
    uint8_t zero = result == 0 ? ZERO : 0;
    cpu->registers[R8_F] = zero | half_carry | carry | SUBTRACT;
  }
  cpu->cycles_left -= 1;
}

void sbc_a_r8(cpu_t *const restrict cpu) {
  r8_t target_reg = r8_from_opcode(bus_read(cpu, cpu->pc));
  uint8_t a = cpu->registers[R8_A];
  uint8_t value = cpu->registers[target_reg];
  uint8_t carry_in = cpu->registers[R8_F] & CARRY ? 1 : 0;
  uint8_t result = a - value - carry_in;
  cpu->registers[R8_A] = result;
  uint8_t half_carry;
  if (carry_in) {
    half_carry = UNDERFLOW_FROM_BIT3_WITH_CARRY(a, value) ? HALF_CARRY : 0;
  } else {
    half_carry = UNDERFLOW_FROM_BIT3(a, value) ? HALF_CARRY : 0;
  }
  uint8_t carry;
  if (carry_in) {
    carry = UNDERFLOW_FROM_BIT7_WITH_CARRY(a, value) ? CARRY : 0;
  } else {
    carry = UNDERFLOW_FROM_BIT7(a, value) ? CARRY : 0;
  }
  uint8_t zero = result == 0 ? ZERO : 0;
  cpu->registers[R8_F] = zero | half_carry | carry | SUBTRACT;
  cpu->pc += 1;
  cpu->cycles_left -= 1;
}

void sbc_a_hl_ref(cpu_t *const restrict cpu) {
  if (cpu->cycles_left == 2) {
    cpu->first_operand = bus_read(cpu, read_r16(cpu, R16_HL));
    cpu->pc += 1;
  } else if (cpu->cycles_left == 1) {
    uint8_t a = cpu->registers[R8_A];
    uint8_t value = cpu->first_operand;
    uint8_t carry_in = cpu->registers[R8_F] & CARRY ? 1 : 0;
    uint8_t result = a - value - carry_in;
    cpu->registers[R8_A] = result;
    uint8_t half_carry;
    if (carry_in) {
      half_carry = UNDERFLOW_FROM_BIT3_WITH_CARRY(a, value) ? HALF_CARRY : 0;
    } else {
      half_carry = UNDERFLOW_FROM_BIT3(a, value) ? HALF_CARRY : 0;
    }
    uint8_t carry;
    if (carry_in) {
      carry = UNDERFLOW_FROM_BIT7_WITH_CARRY(a, value) ? CARRY : 0;
    } else {
      carry = UNDERFLOW_FROM_BIT7(a, value) ? CARRY : 0;
    }
    uint8_t zero = result == 0 ? ZERO : 0;
    cpu->registers[R8_F] = zero | half_carry | carry | SUBTRACT;
  }
  cpu->cycles_left -= 1;
}

void add_a_imm8(cpu_t *const restrict cpu) {
  if (cpu->cycles_left == 1) {
    uint8_t a = cpu->registers[R8_A];
    uint8_t value = bus_read(cpu, cpu->pc);
    uint8_t result = a + value;
    cpu->registers[R8_A] = result;
    uint8_t half_carry = OVERFLOW_FROM_BIT3(a, value) ? HALF_CARRY : 0;
    uint8_t carry = OVERFLOW_FROM_BIT7(a, value) ? CARRY : 0;
    uint8_t zero = result == 0 ? ZERO : 0;
    cpu->registers[R8_F] = zero | half_carry | carry;
  }
  cpu->pc += 1;
  cpu->cycles_left -= 1;
}

void adc_a_imm8(cpu_t *const restrict cpu) {
  if (cpu->cycles_left == 1) {
    uint8_t a = cpu->registers[R8_A];
    uint8_t value = bus_read(cpu, cpu->pc);
    uint8_t carry_in = cpu->registers[R8_F] & CARRY ? 1 : 0;
    uint8_t result = a + value + carry_in;
    cpu->registers[R8_A] = result;
    uint8_t half_carry;
    if (carry_in) {
      half_carry = OVERFLOW_FROM_BIT3_WITH_CARRY(a, value) ? HALF_CARRY : 0;
    } else {
      half_carry = OVERFLOW_FROM_BIT3(a, value) ? HALF_CARRY : 0;
    }
    uint8_t carry;
    if (carry_in) {
      carry = OVERFLOW_FROM_BIT_WITH_CARRY(a, value) ? CARRY : 0;
    } else {
      carry = OVERFLOW_FROM_BIT7(a, value) ? CARRY : 0;
    }
    uint8_t zero = result == 0 ? ZERO : 0;
    cpu->registers[R8_F] = zero | half_carry | carry;
  }
  cpu->pc += 1;
  cpu->cycles_left -= 1;
}

void sub_a_imm8(cpu_t *const restrict cpu) {
  if (cpu->cycles_left == 1) {
    uint8_t a = cpu->registers[R8_A];
    uint8_t value = bus_read(cpu, cpu->pc);
    uint8_t result = a - value;
    cpu->registers[R8_A] = result;
    uint8_t half_carry = UNDERFLOW_FROM_BIT3(a, value) ? HALF_CARRY : 0;
    uint8_t carry = UNDERFLOW_FROM_BIT7(a, value) ? CARRY : 0;
    uint8_t zero = result == 0 ? ZERO : 0;
    cpu->registers[R8_F] = zero | half_carry | carry | SUBTRACT;
  }
  cpu->pc += 1;
  cpu->cycles_left -= 1;
}

void sbc_a_imm8(cpu_t *const restrict cpu) {
  if (cpu->cycles_left == 1) {
    uint8_t a = cpu->registers[R8_A];
    uint8_t value = bus_read(cpu, cpu->pc);
    uint8_t carry_in = cpu->registers[R8_F] & CARRY ? 1 : 0;
    uint8_t result = a - value - carry_in;
    cpu->registers[R8_A] = result;
    uint8_t half_carry;
    if (carry_in) {
      half_carry = UNDERFLOW_FROM_BIT3_WITH_CARRY(a, value) ? HALF_CARRY : 0;
    } else {
      half_carry = UNDERFLOW_FROM_BIT3(a, value) ? HALF_CARRY : 0;
    }
    uint8_t carry;
    if (carry_in) {
      carry = UNDERFLOW_FROM_BIT7_WITH_CARRY(a, value) ? CARRY : 0;
    } else {
      carry = UNDERFLOW_FROM_BIT7(a, value) ? CARRY : 0;
    }
    uint8_t zero = result == 0 ? ZERO : 0;
    cpu->registers[R8_F] = zero | half_carry | carry | SUBTRACT;
  }
  cpu->pc += 1;
  cpu->cycles_left -= 1;
}

void add_sp_s8(cpu_t *const restrict cpu) {
  if (cpu->cycles_left == 4) {
    cpu->pc += 1;
  } else if (cpu->cycles_left == 3) {
    cpu->first_operand = bus_read(cpu, cpu->pc);
  } else if (cpu->cycles_left == 2) {
  } else if (cpu->cycles_left == 1) {
    uint16_t sp = cpu->sp;
    int8_t value = (int8_t)bus_read(cpu, cpu->pc);
    uint16_t result = sp + value;
    cpu->sp = result;
    uint8_t half_carry = OVERFLOW_FROM_BIT3((sp & 0xFF), (uint8_t)value) ? HALF_CARRY : 0;
    uint8_t carry = OVERFLOW_FROM_BIT7((sp & 0xFF), (uint8_t)value) ? CARRY : 0;
    cpu->registers[R8_F] = half_carry | carry;
    cpu->pc += 1;
  }
  cpu->cycles_left -= 1;
}

void cp_a_imm8(cpu_t *const restrict cpu) {
  if (cpu->cycles_left == 1) {
    uint8_t a = cpu->registers[R8_A];
    uint8_t value = bus_read(cpu, cpu->pc);
    uint8_t result = a - value;
    uint8_t half_carry = UNDERFLOW_FROM_BIT3(a, value) ? HALF_CARRY : 0;
    uint8_t carry = UNDERFLOW_FROM_BIT7(a, value) ? CARRY : 0;
    uint8_t zero = result == 0 ? ZERO : 0;
    cpu->registers[R8_F] = zero | half_carry | carry | SUBTRACT;
  }
  cpu->pc += 1;
  cpu->cycles_left -= 1;
}