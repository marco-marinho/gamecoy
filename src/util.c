#include "cpu.h"

r8_t r8_from_opcode(uint8_t opcode) {
  switch (opcode) {
  case 0x04: // INC B
  case 0x05: // DEC B
  case 0x06: // LD B, d8
    return R8_B;
  case 0x0C: // INC C
  case 0x0D: // DEC C
  case 0x0E: // LD C, d8
    return R8_C;
  case 0x14: // INC D
  case 0x15: // DEC D
  case 0x16: // LD D, d8
    return R8_D;
  case 0x1C: // INC E
  case 0x1D: // DEC E
  case 0x1E: // LD E, d8
    return R8_E;
  case 0x24: // INC H
  case 0x25: // DEC H
  case 0x26: // LD H, d8
    return R8_H;
  case 0x2C: // INC L
  case 0x2D: // DEC L
  case 0x2E: // LD L, d8
    return R8_L;
  default:
    return -1;
  }
}

r16_t r16_from_opcode(uint8_t opcode) {
  switch (opcode) {
  case 0x01: // LD BC, d16
  case 0x02: // LD (BC), A
  case 0x03: // INC BC
  case 0x09: // ADD HL, BC
  case 0x0A: // LD A, (BC)
  case 0x0B: // DEC BC
    return R16_BC;
  case 0x11: // LD DE, d16
  case 0x12: // LD (DE), A
  case 0x13: // INC DE
  case 0x19: // ADD HL, DE
  case 0x1A: // LD A, (DE)
  case 0x1B: // DEC DE
    return R16_DE;
  case 0x21: // LD HL, d16
  case 0x23: // INC HL
  case 0x29: // ADD HL, HL
  case 0x2B: // DEC HL
    return R16_HL;
  default:
    return -1;
  }
}