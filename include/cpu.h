#pragma once

#include "instructions.h"
#include <stdint.h>

#define ZERO 0x80
#define SUBTRACT 0x40
#define HALF_CARRY 0x20
#define CARRY 0x10

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  uint16_t pc;
  uint16_t sp;
  uint8_t registers[9];
  uint8_t ram[65536];
  uint8_t halted;
  opcode_t current_opcode;
  uint16_t first_operand;
  uint16_t second_operand;
  uint8_t cycles_left;
  uint8_t ie_delay;
  uint8_t ime;
} cpu_t;

typedef enum {
  R16_AF,
  R16_BC,
  R16_DE,
  R16_HL,
} r16_t;

typedef enum {
  R8_A = 0,
  R8_B = 1,
  R8_C = 2,
  R8_D = 3,
  R8_E = 4,
  R8_F = 5,
  R8_G = 6, // Not an actual register, unused
  R8_H = 7,
  R8_L = 8,
  HL_REF = 9, // Not an actual register, used to indicate (HL) in certain instructions
} r8_t;

void tick(cpu_t *const cpu);

#ifdef __cplusplus
}
#endif