#include "util.h"
#include "cpu.h"

r8_t r8_from_opcode(uint8_t opcode) {
  switch (opcode) {
  case 0x04: // INC B
  case 0x05: // DEC B
  case 0x06: // LD B, d8
  case 0x46: // LD B, (HL)
  case 0x70: // LD (HL), B
  case 0x80: // ADD A, B
    return R8_B;
  case 0x0C: // INC C
  case 0x0D: // DEC C
  case 0x0E: // LD C, d8
  case 0x4E: // LD C, (HL)
  case 0x71: // LD (HL), C
  case 0x81: // ADD A, C
    return R8_C;
  case 0x14: // INC D
  case 0x15: // DEC D
  case 0x16: // LD D, d8
  case 0x56: // LD D, (HL)
  case 0x72: // LD (HL), D
  case 0x82: // ADD A, D
    return R8_D;
  case 0x1C: // INC E
  case 0x1D: // DEC E
  case 0x1E: // LD E, d8
  case 0x5E: // LD E, (HL)
  case 0x73: // LD (HL), E
  case 0x83: // ADD A, E
    return R8_E;
  case 0x24: // INC H
  case 0x25: // DEC H
  case 0x26: // LD H, d8
  case 0x66: // LD H, (HL)
  case 0x74: // LD (HL), H
  case 0x84: // ADD A, H
    return R8_H;
  case 0x2C: // INC L
  case 0x2D: // DEC L
  case 0x2E: // LD L, d8
  case 0x6E: // LD L, (HL)
  case 0x75: // LD (HL), L
  case 0x85: // ADD A, L
    return R8_L;
  case 0x3C: // INC A
  case 0x3D: // DEC A
  case 0x3E: // LD A, d8
  case 0x77: // LD (HL), A
  case 0x7E: // LD A, (HL)
  case 0x87: // ADD A, A
    return R8_A;
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

r8_pair_t r8_pair_from_opcode(uint8_t opcode) {
  switch (opcode) {
  case 0x40:
    return (r8_pair_t){.first_operand = R8_B, .second_operand = R8_B};
  case 0x41:
    return (r8_pair_t){.first_operand = R8_B, .second_operand = R8_C};
  case 0x42:
    return (r8_pair_t){.first_operand = R8_B, .second_operand = R8_D};
  case 0x43:
    return (r8_pair_t){.first_operand = R8_B, .second_operand = R8_E};
  case 0x44:
    return (r8_pair_t){.first_operand = R8_B, .second_operand = R8_H};
  case 0x45:
    return (r8_pair_t){.first_operand = R8_B, .second_operand = R8_L};
  case 0x47:
    return (r8_pair_t){.first_operand = R8_B, .second_operand = R8_A};
  case 0x48:
    return (r8_pair_t){.first_operand = R8_C, .second_operand = R8_B};
  case 0x49:
    return (r8_pair_t){.first_operand = R8_C, .second_operand = R8_C};
  case 0x4A:
    return (r8_pair_t){.first_operand = R8_C, .second_operand = R8_D};
  case 0x4B:
    return (r8_pair_t){.first_operand = R8_C, .second_operand = R8_E};
  case 0x4C:
    return (r8_pair_t){.first_operand = R8_C, .second_operand = R8_H};
  case 0x4D:
    return (r8_pair_t){.first_operand = R8_C, .second_operand = R8_L};
  case 0x4F:
    return (r8_pair_t){.first_operand = R8_C, .second_operand = R8_A};
  case 0x50:
    return (r8_pair_t){.first_operand = R8_D, .second_operand = R8_B};
  case 0x51:
    return (r8_pair_t){.first_operand = R8_D, .second_operand = R8_C};
  case 0x52:
    return (r8_pair_t){.first_operand = R8_D, .second_operand = R8_D};
  case 0x53:
    return (r8_pair_t){.first_operand = R8_D, .second_operand = R8_E};
  case 0x54:
    return (r8_pair_t){.first_operand = R8_D, .second_operand = R8_H};
  case 0x55:
    return (r8_pair_t){.first_operand = R8_D, .second_operand = R8_L};
  case 0x57:
    return (r8_pair_t){.first_operand = R8_D, .second_operand = R8_A};
  case 0x58:
    return (r8_pair_t){.first_operand = R8_E, .second_operand = R8_B};
  case 0x59:
    return (r8_pair_t){.first_operand = R8_E, .second_operand = R8_C};
  case 0x5A:
    return (r8_pair_t){.first_operand = R8_E, .second_operand = R8_D};
  case 0x5B:
    return (r8_pair_t){.first_operand = R8_E, .second_operand = R8_E};
  case 0x5C:
    return (r8_pair_t){.first_operand = R8_E, .second_operand = R8_H};
  case 0x5D:
    return (r8_pair_t){.first_operand = R8_E, .second_operand = R8_L};
  case 0x5F:
    return (r8_pair_t){.first_operand = R8_E, .second_operand = R8_A};
  case 0x60:
    return (r8_pair_t){.first_operand = R8_H, .second_operand = R8_B};
  case 0x61:
    return (r8_pair_t){.first_operand = R8_H, .second_operand = R8_C};
  case 0x62:
    return (r8_pair_t){.first_operand = R8_H, .second_operand = R8_D};
  case 0x63:
    return (r8_pair_t){.first_operand = R8_H, .second_operand = R8_E};
  case 0x64:
    return (r8_pair_t){.first_operand = R8_H, .second_operand = R8_H};
  case 0x65:
    return (r8_pair_t){.first_operand = R8_H, .second_operand = R8_L};
  case 0x67:
    return (r8_pair_t){.first_operand = R8_H, .second_operand = R8_A};
  case 0x68:
    return (r8_pair_t){.first_operand = R8_L, .second_operand = R8_B};
  case 0x69:
    return (r8_pair_t){.first_operand = R8_L, .second_operand = R8_C};
  case 0x6A:
    return (r8_pair_t){.first_operand = R8_L, .second_operand = R8_D};
  case 0x6B:
    return (r8_pair_t){.first_operand = R8_L, .second_operand = R8_E};
  case 0x6C:
    return (r8_pair_t){.first_operand = R8_L, .second_operand = R8_H};
  case 0x6D:
    return (r8_pair_t){.first_operand = R8_L, .second_operand = R8_L};
  case 0x6F:
    return (r8_pair_t){.first_operand = R8_L, .second_operand = R8_A};
  case 0x78:
    return (r8_pair_t){.first_operand = R8_A, .second_operand = R8_B};
  case 0x79:
    return (r8_pair_t){.first_operand = R8_A, .second_operand = R8_C};
  case 0x7A:
    return (r8_pair_t){.first_operand = R8_A, .second_operand = R8_D};
  case 0x7B:
    return (r8_pair_t){.first_operand = R8_A, .second_operand = R8_E};
  case 0x7C:
    return (r8_pair_t){.first_operand = R8_A, .second_operand = R8_H};
  case 0x7D:
    return (r8_pair_t){.first_operand = R8_A, .second_operand = R8_L};
  case 0x7F:
    return (r8_pair_t){.first_operand = R8_A, .second_operand = R8_A};
  default:
    return (r8_pair_t){.first_operand = -1, .second_operand = -1};
  }
}