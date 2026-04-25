#include "util.h"
#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>

r8_t r8_from_opcode(uint8_t opcode) {
  switch (opcode) {
  case 0x04: // INC B
  case 0x05: // DEC B
  case 0x06: // LD B, d8
  case 0x46: // LD B, (HL)
  case 0x70: // LD (HL), B
  case 0x80: // ADD A, B
  case 0x88: // ADC A, B
  case 0x90: // SUB A, B
  case 0x98: // SBC A, B
  case 0xA0: // AND A, B
  case 0xA8: // XOR A, B
  case 0xB0: // OR A, B
  case 0xB8: // CP A, B
    return R8_B;
  case 0x0C: // INC C
  case 0x0D: // DEC C
  case 0x0E: // LD C, d8
  case 0x4E: // LD C, (HL)
  case 0x71: // LD (HL), C
  case 0x81: // ADD A, C
  case 0x89: // ADC A, C
  case 0x91: // SUB A, C
  case 0x99: // SBC A, C
  case 0xA1: // AND A, C
  case 0xA9: // XOR A, C
  case 0xB1: // OR A, C
  case 0xB9: // CP A, C
    return R8_C;
  case 0x14: // INC D
  case 0x15: // DEC D
  case 0x16: // LD D, d8
  case 0x56: // LD D, (HL)
  case 0x72: // LD (HL), D
  case 0x82: // ADD A, D
  case 0x8A: // ADC A, D
  case 0x92: // SUB A, D
  case 0x9A: // SBC A, D
  case 0xA2: // AND A, D
  case 0xAA: // XOR A, D
  case 0xB2: // OR A, D
  case 0xBA: // CP A, D
    return R8_D;
  case 0x1C: // INC E
  case 0x1D: // DEC E
  case 0x1E: // LD E, d8
  case 0x5E: // LD E, (HL)
  case 0x73: // LD (HL), E
  case 0x83: // ADD A, E
  case 0x8B: // ADC A, E
  case 0x93: // SUB A, E
  case 0x9B: // SBC A, E
  case 0xA3: // AND A, E
  case 0xAB: // XOR A, E
  case 0xB3: // OR A, E
  case 0xBB: // CP A, E
    return R8_E;
  case 0x24: // INC H
  case 0x25: // DEC H
  case 0x26: // LD H, d8
  case 0x66: // LD H, (HL)
  case 0x74: // LD (HL), H
  case 0x84: // ADD A, H
  case 0x8C: // ADC A, H
  case 0x94: // SUB A, H
  case 0x9C: // SBC A, H
  case 0xA4: // AND A, H
  case 0xAC: // XOR A, H
  case 0xB4: // OR A, H
  case 0xBC: // CP A, H
    return R8_H;
  case 0x2C: // INC L
  case 0x2D: // DEC L
  case 0x2E: // LD L, d8
  case 0x6E: // LD L, (HL)
  case 0x75: // LD (HL), L
  case 0x85: // ADD A, L
  case 0x8D: // ADC A, L
  case 0x95: // SUB A, L
  case 0x9D: // SBC A, L
  case 0xA5: // AND A, L
  case 0xAD: // XOR A, L
  case 0xB5: // OR A, L
  case 0xBD: // CP A, L
    return R8_L;
  case 0x3C: // INC A
  case 0x3D: // DEC A
  case 0x3E: // LD A, d8
  case 0x77: // LD (HL), A
  case 0x7E: // LD A, (HL)
  case 0x87: // ADD A, A
  case 0x8F: // ADC A, A
  case 0x97: // SUB A, A
  case 0x9F: // SBC A, A
  case 0xA7: // AND A, A
  case 0xAF: // XOR A, A
  case 0xB7: // OR A, A
  case 0xBF: // CP A, A
    return R8_A;
  default:
    fprintf(stderr, "[Util - r8_from_opcode] Unknown opcode: 0x%02X\n", opcode);
    exit(EXIT_FAILURE);
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
  case 0xC1: // POP BC
  case 0xC5: // PUSH BC
    return R16_BC;
  case 0x11: // LD DE, d16
  case 0x12: // LD (DE), A
  case 0x13: // INC DE
  case 0x19: // ADD HL, DE
  case 0x1A: // LD A, (DE)
  case 0x1B: // DEC DE
  case 0xD1: // POP DE
  case 0xD5: // PUSH DE
    return R16_DE;
  case 0x21: // LD HL, d16
  case 0x23: // INC HL
  case 0x29: // ADD HL, HL
  case 0x2B: // DEC HL
  case 0xE1: // POP HL
  case 0xE5: // PUSH HL
    return R16_HL;
  case 0xF1: // POP AF
  case 0xF5: // PUSH AF
    return R16_AF;
  default:
    fprintf(stderr, "[Util - r16_from_opcode] Unknown opcode: 0x%02X\n",
            opcode);
    exit(EXIT_FAILURE);
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
    fprintf(stderr, "[Util - r8_pair_from_opcode] Unknown opcode: 0x%02X\n",
            opcode);
    exit(EXIT_FAILURE);
  }
}

r8_pair_t r8_pair_from_r16(r16_t r16) {
  switch (r16) {
  case R16_BC:
    return (r8_pair_t){.first_operand = R8_B, .second_operand = R8_C};
  case R16_DE:
    return (r8_pair_t){.first_operand = R8_D, .second_operand = R8_E};
  case R16_HL:
    return (r8_pair_t){.first_operand = R8_H, .second_operand = R8_L};
  case R16_AF:
    return (r8_pair_t){.first_operand = R8_A, .second_operand = R8_F};
  default:
    fprintf(stderr, "[Util - r8_pair_from_r16] Unknown r16: 0x%02X\n", r16);
    exit(EXIT_FAILURE);
  }
}

uint16_t get_rst_vector(uint8_t opcode) {
  switch (opcode) {
  case 0xC7: // RST 00H
    return 0x00;
  case 0xCF: // RST 08H
    return 0x08;
  case 0xD7: // RST 10H
    return 0x10;
  case 0xDF: // RST 18H
    return 0x18;
  case 0xE7: // RST 20H
    return 0x20;
  case 0xEF: // RST 28H
    return 0x28;
  case 0xF7: // RST 30H
    return 0x30;
  case 0xFF: // RST 38H
    return 0x38;
  default:
    fprintf(stderr, "[Util - get_rst_vector] Unknown opcode: 0x%02X\n", opcode);
    exit(EXIT_FAILURE);
  }
}

r8_t r8_from_cb_opcode(uint8_t opcode) {
  uint8_t lower_nibble = opcode & 0x0F;
  switch (lower_nibble) {
  case 0x00:
  case 0x08:
    return R8_B;
  case 0x01:
  case 0x09:
    return R8_C;
  case 0x02:
  case 0x0A:
    return R8_D;
  case 0x03:
  case 0x0B:
    return R8_E;
  case 0x04:
  case 0x0C:
    return R8_H;
  case 0x05:
  case 0x0D:
    return R8_L;
  case 0x07:
  case 0x0F:
    return R8_A;
  case 0x06:
  case 0x0E:
    return HL_REF;
  default:
    fprintf(stderr, "[Util - r8_from_cb_opcode] Unknown CB opcode: 0x%02X\n",
            opcode);
    exit(EXIT_FAILURE);
  }
};

uint8_t cb_load(cpu_t *const restrict cpu, r8_t reg) {
  if (reg == HL_REF) {
    return cpu->ram[(cpu->registers[R8_H] << 8) | cpu->registers[R8_L]];
  } else {
    return cpu->registers[reg];
  }
}

void cb_store(cpu_t *const restrict cpu, r8_t reg, uint8_t value) {
  if (reg == HL_REF) {
    cpu->ram[(cpu->registers[R8_H] << 8) | cpu->registers[R8_L]] = value;
  } else {
    cpu->registers[reg] = value;
  }
}

uint8_t bit_bit(uint8_t opcode){
  return (opcode - 0x40) / 8;
}

uint8_t bit_res(uint8_t opcode){
  return (opcode - 0x80) / 8;
}

uint8_t bit_set(uint8_t opcode){
  return (opcode - 0xC0) / 8;
}