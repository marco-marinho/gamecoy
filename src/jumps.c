#include "cpu.h"
#include "util.h"

void jr_s8(cpu_t *const restrict cpu) {
  if (cpu->cycles_left == 3) {
    cpu->pc += 1;
  } else if (cpu->cycles_left == 2) {
    cpu->first_operand = (int8_t)cpu->ram[cpu->pc];
    cpu->pc += 1;
  } else if (cpu->cycles_left == 1) {
    cpu->pc += cpu->first_operand;
  }
  cpu->cycles_left -= 1;
}

void jr_nz_s8(cpu_t *const restrict cpu) {
  if (cpu->cycles_left == 3) {
    cpu->pc += 1;
  } else if (cpu->cycles_left == 2) {
    uint8_t zero_flag = cpu->registers[R8_F] & ZERO;
    if (zero_flag) {
      cpu->cycles_left -= 1;
      cpu->pc += 1;
    }
  } else if (cpu->cycles_left == 1) {
    cpu->pc += (int8_t)cpu->ram[cpu->pc];
    cpu->pc += 1;
  }
  cpu->cycles_left -= 1;
}

void jr_nc_s8(cpu_t *const restrict cpu) {
  if (cpu->cycles_left == 3) {
    cpu->pc += 1;
  } else if (cpu->cycles_left == 2) {
    uint8_t carry_flag = cpu->registers[R8_F] & CARRY;
    if (carry_flag) {
      cpu->cycles_left -= 1;
      cpu->pc += 1;
    }
  } else if (cpu->cycles_left == 1) {
    cpu->pc += (int8_t)cpu->ram[cpu->pc];
    cpu->pc += 1;
  }
  cpu->cycles_left -= 1;
}

void jr_z_s8(cpu_t *const restrict cpu) {
  if (cpu->cycles_left == 3) {
    cpu->pc += 1;
  } else if (cpu->cycles_left == 2) {
    uint8_t zero_flag = cpu->registers[R8_F] & ZERO;
    if (!zero_flag) {
      cpu->cycles_left -= 1;
      cpu->pc += 1;
    }
  } else if (cpu->cycles_left == 1) {
    cpu->pc += (int8_t)cpu->ram[cpu->pc];
    cpu->pc += 1;
  }
  cpu->cycles_left -= 1;
}

void jr_c_s8(cpu_t *const restrict cpu) {
  if (cpu->cycles_left == 3) {
    cpu->pc += 1;
  } else if (cpu->cycles_left == 2) {
    uint8_t carry_flag = cpu->registers[R8_F] & CARRY;
    if (!carry_flag) {
      cpu->cycles_left -= 1;
      cpu->pc += 1;
    }
  } else if (cpu->cycles_left == 1) {
    cpu->pc += (int8_t)cpu->ram[cpu->pc];
    cpu->pc += 1;
  }
  cpu->cycles_left -= 1;
}

void ret_cc(cpu_t *const restrict cpu, uint8_t flag) {
  if (cpu->cycles_left == 4) {
    if (!flag){
      cpu->pc += 1;
      cpu->cycles_left = 1;
    }
    else {
      cpu->pc = cpu->ram[cpu->sp] & 0xFF;
    }
  }
  else if (cpu->cycles_left == 3){
    cpu->sp += 1;
  }
  else if (cpu->cycles_left == 2){
    cpu->pc |= (cpu->ram[cpu->sp] << 8);
  }
  else if (cpu->cycles_left == 1){
    cpu->sp += 1;
  }
  cpu->cycles_left -= 1;
}

void ret_nz(cpu_t *const restrict cpu) {
  uint8_t not_zero_flag = cpu->registers[R8_F] & ZERO ? 0 : 1;
  ret_cc(cpu, not_zero_flag);
}

void ret_z(cpu_t *const restrict cpu) {
  uint8_t zero_flag = cpu->registers[R8_F] & ZERO ? 1 : 0;
  ret_cc(cpu, zero_flag);
}

void ret(cpu_t *const restrict cpu) {
  ret_cc(cpu, 1);
}

void ret_nc(cpu_t *const restrict cpu) {
  uint8_t not_carry_flag = cpu->registers[R8_F] & CARRY ? 0 : 1;
  ret_cc(cpu, not_carry_flag);
}

void ret_c(cpu_t *const restrict cpu) {
  uint8_t carry_flag = cpu->registers[R8_F] & CARRY ? 1 : 0;
  ret_cc(cpu, carry_flag);
}

void reti(cpu_t *const restrict cpu) {
  cpu->ie_delay = 1;
  ret_cc(cpu, 1);
}

void jp_cc_imm16(cpu_t * const restrict cpu, uint8_t flag) {
  if (cpu->cycles_left == 4){
    cpu->pc += 1;
    cpu->first_operand = cpu->ram[cpu->pc];
  } else if (cpu->cycles_left == 3){
    cpu->pc += 1;
    cpu->second_operand = cpu->ram[cpu->pc];
  } else if (cpu->cycles_left == 2){
    if(!flag){
      cpu->pc += 1;
      cpu->cycles_left = 1;
    }
    else {
      cpu->pc = cpu->first_operand;
    }
  } else if (cpu->cycles_left == 1){
    cpu->pc |= (cpu->second_operand << 8);
  }
  cpu->cycles_left -= 1;
}

void jp_nz_imm16(cpu_t * const restrict cpu) {
  uint8_t not_zero_flag = cpu->registers[R8_F] & ZERO ? 0 : 1;
  jp_cc_imm16(cpu, not_zero_flag);
}

void jp_imm16(cpu_t * const restrict cpu) {
  jp_cc_imm16(cpu, 1);
}

void jp_z_imm16(cpu_t * const restrict cpu) {
  uint8_t zero_flag = cpu->registers[R8_F] & ZERO ? 1 : 0;
  jp_cc_imm16(cpu, zero_flag);
}

void jp_nc_imm16(cpu_t * const restrict cpu) {
  uint8_t not_carry_flag = cpu->registers[R8_F] & CARRY ? 0 : 1;
  jp_cc_imm16(cpu, not_carry_flag);
}

void jp_c_imm16(cpu_t * const restrict cpu) {
  uint8_t carry_flag = cpu->registers[R8_F] & CARRY ? 1 : 0;
  jp_cc_imm16(cpu, carry_flag);
}

void call_cc_imm16(cpu_t * const restrict cpu, uint8_t flag){
  if (cpu->cycles_left == 6){
    cpu->pc += 1;
    cpu->first_operand = cpu->ram[cpu->pc];
  } else if (cpu->cycles_left == 5){
    cpu->pc += 1;
    cpu->second_operand = cpu->ram[cpu->pc];
  } else if (cpu->cycles_left == 4){
    cpu->pc += 1;
    if(!flag){
      cpu->cycles_left = 1;
    }
    else {
      cpu->sp -= 1;
      cpu->ram[cpu->sp] = (cpu->pc >> 8);
    }
  } else if (cpu->cycles_left == 3){
    cpu->sp -= 1;
    cpu->ram[cpu->sp] |= cpu->pc & 0xFF;
  } else if (cpu->cycles_left == 2){
    cpu->pc = cpu->first_operand;
  } else if (cpu->cycles_left == 1){
    cpu->pc |= (cpu->second_operand << 8);
  }
  cpu->cycles_left -= 1;
}

void call_nz_imm16(cpu_t * const restrict cpu) {
  uint8_t not_zero_flag = cpu->registers[R8_F] & ZERO ? 0 : 1;
  call_cc_imm16(cpu, not_zero_flag);
}

void call_z_imm16(cpu_t * const restrict cpu) {
  uint8_t zero_flag = cpu->registers[R8_F] & ZERO ? 1 : 0;
  call_cc_imm16(cpu, zero_flag);
}

void call_imm16(cpu_t * const restrict cpu) {
  call_cc_imm16(cpu, 1);
}

void call_nc_imm16(cpu_t * const restrict cpu) {
  uint8_t not_carry_flag = cpu->registers[R8_F] & CARRY ? 0 : 1;
  call_cc_imm16(cpu, not_carry_flag);
}

void call_c_imm16(cpu_t * const restrict cpu) {
  uint8_t carry_flag = cpu->registers[R8_F] & CARRY ? 1 : 0;
  call_cc_imm16(cpu, carry_flag);
}

void rst(cpu_t * const restrict cpu){
  if (cpu->cycles_left == 4){
    cpu->first_operand = get_rst_vector(cpu->ram[cpu->pc]);
    cpu->pc += 1;
  } else if (cpu->cycles_left == 3){
    cpu->sp -= 1;
    cpu->ram[cpu->sp] = (cpu->pc >> 8);
  } else if (cpu->cycles_left == 2){
    cpu->sp -= 1;
    cpu->ram[cpu->sp] |= cpu->pc & 0xFF;
  } else if (cpu->cycles_left == 1){
    cpu->pc = cpu->first_operand;
  }
  cpu->cycles_left -= 1;
}