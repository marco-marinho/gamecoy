#include "cpu.h"

void nop(cpu_t *const restrict cpu) {
  cpu->pc += 1;
  cpu->cycles_left -= 1;
}

void stop(cpu_t *const restrict cpu) {
  cpu->halted = 1;
  cpu->pc += 1;
  cpu->cycles_left -= 1;
}

void halt(cpu_t *const restrict cpu) {
  cpu->halted = 1;
  cpu->pc += 1;
  cpu->cycles_left -= 1;
}

void daa(cpu_t *const restrict cpu) {
  uint8_t a = cpu->registers[R8_A];
  uint8_t f = cpu->registers[R8_F];
  uint8_t n_flag = f & SUBTRACT ? 1 : 0;
  uint8_t h_flag;
  if (n_flag) {
    h_flag = f & HALF_CARRY ? 1 : 0;
  } else {
    h_flag = (f & HALF_CARRY) || ((a & 0x0F) > 0x09) ? 1 : 0;
  }
  uint8_t c_flag;
  if (n_flag) {
    c_flag = f & CARRY ? 1 : 0;
  } else {
    c_flag = (f & CARRY) || (a > 0x99) ? 1 : 0;
  }
  uint8_t result = a;
  if (n_flag && h_flag && !c_flag) {
    result -= 0x06;
  } else if (n_flag && !h_flag && c_flag) {
    result -= 0x60;
  } else if (n_flag && h_flag && c_flag) {
    result -= 0x66;
  } else if (!n_flag && h_flag && !c_flag) {
    result += 0x06;
  } else if (!n_flag && !h_flag && c_flag) {
    result += 0x60;
  } else if (!n_flag && h_flag && c_flag) {
    result += 0x66;
  }
  uint8_t new_zero = result == 0 ? ZERO : 0;
  uint8_t new_carry = (c_flag || (a > 0x99 && !n_flag)) ? CARRY : 0;
  uint8_t new_subtract = n_flag ? SUBTRACT : 0;
  cpu->registers[R8_A] = result;
  cpu->registers[R8_F] = new_zero | new_carry | new_subtract;
  cpu->pc += 1;
  cpu->cycles_left -= 1;
}

void scf(cpu_t *const restrict cpu) {
  cpu->registers[R8_F] = (cpu->registers[R8_F] & ZERO) | CARRY;
  cpu->pc += 1;
  cpu->cycles_left -= 1;
}

void ccf(cpu_t *const restrict cpu) {
  uint8_t c_flag = cpu->registers[R8_F] & CARRY ? 0 : CARRY;
  uint8_t zero_flag = cpu->registers[R8_F] & ZERO;
  cpu->registers[R8_F] = zero_flag | c_flag;
  cpu->pc += 1;
  cpu->cycles_left -= 1;
}

void di(cpu_t *const restrict cpu) {
  cpu->ime = 0;
  cpu->pc += 1;
  cpu->cycles_left -= 1;
}

void ei(cpu_t *const restrict cpu) {
  cpu->ie_delay = 2;
  cpu->pc += 1;
  cpu->cycles_left -= 1;
}