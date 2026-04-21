#include "cpu.h"

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