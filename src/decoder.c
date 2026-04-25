#include "cpu.h"
#include "executor.h"
#include "instructions.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void decode_instruction(cpu_t *const restrict cpu) {
  uint8_t opcode = cpu->ram[cpu->pc];
  switch (opcode) {
  case 0x00: // NOP
    cpu->current_opcode = OP_NOP;
    cpu->cycles_left = 1;
    break;
  case 0x01: // LD BC, d16
  case 0x11: // LD DE, d16
  case 0x21: // LD HL, d16
    cpu->current_opcode = OP_LD_R16_IMM16;
    cpu->cycles_left = 3;
    break;
  case 0x02: // LD (BC), A
  case 0x12: // LD (DE), A
    cpu->current_opcode = OP_LD_R16REF_A;
    cpu->cycles_left = 2;
    break;
  case 0x03: // INC BC
  case 0x13: // INC DE
  case 0x23: // INC HL
    cpu->current_opcode = OP_INC_R16;
    cpu->cycles_left = 2;
    break;
  case 0x04: // INC B
  case 0x0C: // INC C
  case 0x14: // INC D
  case 0x1C: // INC E
  case 0x24: // INC H
  case 0x2C: // INC L
  case 0x3C: // INC A
    cpu->current_opcode = OP_INC_R8;
    cpu->cycles_left = 1;
    break;
  case 0x05: // DEC B
  case 0x0D: // DEC C
  case 0x15: // DEC D
  case 0x1D: // DEC E
  case 0x25: // DEC H
  case 0x2D: // DEC L
  case 0x3D: // DEC A
    cpu->current_opcode = OP_DEC_R8;
    cpu->cycles_left = 1;
    break;
  case 0x06: // LD B, d8
  case 0x0E: // LD C, d8
  case 0x16: // LD D, d8
  case 0x1E: // LD E, d8
  case 0x26: // LD H, d8
  case 0x2E: // LD L, d8
  case 0x3E: // LD A, d8
    cpu->current_opcode = OP_LD_R8_IMM8;
    cpu->cycles_left = 2;
    break;
  case 0x07: // RLCA
    cpu->current_opcode = OP_RLCA;
    cpu->cycles_left = 1;
    break;
  case 0x08: // LD (a16), SP
    cpu->current_opcode = OP_LD_REF16_SP;
    cpu->cycles_left = 5;
    break;
  case 0x09: // ADD HL, BC
  case 0x19: // ADD HL, DE
  case 0x29: // ADD HL, HL
    cpu->current_opcode = OP_ADD_HL_R16;
    cpu->cycles_left = 2;
    break;
  case 0x0A: // LD A, (BC)
  case 0x1A: // LD A, (DE)
    cpu->current_opcode = OP_LD_A_R16REF;
    cpu->cycles_left = 2;
    break;
  case 0x0B: // DEC BC
  case 0x1B: // DEC DE
  case 0x2B: // DEC HL
    cpu->current_opcode = OP_DEC_R16;
    cpu->cycles_left = 2;
    break;
  case 0x0F: // RRCA
    cpu->current_opcode = OP_RRCA;
    cpu->cycles_left = 1;
    break;
  case 0x10: // STOP
    cpu->current_opcode = OP_STOP;
    cpu->cycles_left = 1;
    break;
  case 0x17: // RLA
    cpu->current_opcode = OP_RLA;
    cpu->cycles_left = 1;
    break;
  case 0x18: // JR r8
    cpu->current_opcode = OP_JR_S8;
    cpu->cycles_left = 3;
    break;
  case 0x1F: // RRA
    cpu->current_opcode = OP_RRA;
    cpu->cycles_left = 1;
    break;
  case 0x20: // JR NZ, r8
    cpu->current_opcode = OP_JR_NZ_S8;
    cpu->cycles_left = 3;
    break;
  case 0x22: // LD (HL+), A
    cpu->current_opcode = OP_LD_HL_PLUS_REF_A;
    cpu->cycles_left = 2;
    break;
  case 0x27: // DAA
    cpu->current_opcode = OP_DAA;
    cpu->cycles_left = 1;
    break;
  case 0x28: // JR Z, r8
    cpu->current_opcode = OP_JR_Z_S8;
    cpu->cycles_left = 3;
    break;
  case 0x2A: // LD A, (HL+)
    cpu->current_opcode = OP_LD_A_HL_PLUS_REF;
    cpu->cycles_left = 2;
    break;
  case 0x2F: // CPL
    cpu->current_opcode = OP_CPL;
    cpu->cycles_left = 1;
    break;
  case 0x30: // JR NC, r8
    cpu->current_opcode = OP_JR_NC_S8;
    cpu->cycles_left = 3;
    break;
  case 0x31: // LD SP, d16
    cpu->current_opcode = OP_LD_SP_IMM16;
    cpu->cycles_left = 3;
    break;
  case 0x32: // LD (HL-), A
    cpu->current_opcode = OP_LD_HL_MINUS_REF_A;
    cpu->cycles_left = 2;
    break;
  case 0x33: // INC SP
    cpu->current_opcode = OP_INC_SP;
    cpu->cycles_left = 2;
    break;
  case 0x34: // INC (HL)
    cpu->current_opcode = OP_INC_HL_REF;
    cpu->cycles_left = 3;
    break;
  case 0x35: // DEC (HL)
    cpu->current_opcode = OP_DEC_HL_REF;
    cpu->cycles_left = 3;
    break;
  case 0x36: // LD (HL), d8
    cpu->current_opcode = OP_LD_HL_REF_IMM8;
    cpu->cycles_left = 3;
    break;
  case 0x37: // SCF
    cpu->current_opcode = OP_SCF;
    cpu->cycles_left = 1;
    break;
  case 0x38: // JR C, r8
    cpu->current_opcode = OP_JR_C_S8;
    cpu->cycles_left = 3;
    break;
  case 0x39: // ADD HL, SP
    cpu->current_opcode = OP_ADD_HL_SP;
    cpu->cycles_left = 2;
    break;
  case 0x3A: // LD A, (HL-)
    cpu->current_opcode = OP_LD_A_HL_MINUS_REF;
    cpu->cycles_left = 2;
    break;
  case 0x3B: // DEC SP
    cpu->current_opcode = OP_DEC_SP;
    cpu->cycles_left = 2;
    break;
  case 0x3F: // CCF
    cpu->current_opcode = OP_CCF;
    cpu->cycles_left = 1;
    break;
  case 0x40: // LD B, B
  case 0x41: // LD B, C
  case 0x42: // LD B, D
  case 0x43: // LD B, E
  case 0x44: // LD B, H
  case 0x45: // LD B, L
  case 0x47: // LD B, A
  case 0x48: // LD C, B
  case 0x49: // LD C, C
  case 0x4A: // LD C, D
  case 0x4B: // LD C, E
  case 0x4C: // LD C, H
  case 0x4D: // LD C, L
  case 0x4F: // LD C, A
  case 0x50: // LD D, B
  case 0x51: // LD D, C
  case 0x52: // LD D, D
  case 0x53: // LD D, E
  case 0x54: // LD D, H
  case 0x55: // LD D, L
  case 0x57: // LD D, A
  case 0x58: // LD E, B
  case 0x59: // LD E, C
  case 0x5A: // LD E, D
  case 0x5B: // LD E, E
  case 0x5C: // LD E, H
  case 0x5D: // LD E, L
  case 0x5F: // LD E, A
  case 0x60: // LD H, B
  case 0x61: // LD H, C
  case 0x62: // LD H, D
  case 0x63: // LD H, E
  case 0x64: // LD H, H
  case 0x65: // LD H, L
  case 0x67: // LD H, A
  case 0x68: // LD L, B
  case 0x69: // LD L, C
  case 0x6A: // LD L, D
  case 0x6B: // LD L, E
  case 0x6C: // LD L, H
  case 0x6D: // LD L, L
  case 0x6F: // LD L, A
  case 0x78: // LD A, B
  case 0x79: // LD A, C
  case 0x7A: // LD A, D
  case 0x7B: // LD A, E
  case 0x7C: // LD A, H
  case 0x7D: // LD A, L
  case 0x7F: // LD A, A
    cpu->current_opcode = OP_LD_R8_R8;
    cpu->cycles_left = 1;
    break;
  case 0x46: // LD B, (HL)
  case 0x4E: // LD C, (HL)
  case 0x56: // LD D, (HL)
  case 0x5E: // LD E, (HL)
  case 0x66: // LD H, (HL)
  case 0x6E: // LD L, (HL)
  case 0x7E: // LD A, (HL)
    cpu->current_opcode = OP_LD_R8_HL_REF;
    cpu->cycles_left = 2;
    break;
  case 0x70: // LD (HL), B
  case 0x71: // LD (HL), C
  case 0x72: // LD (HL), D
  case 0x73: // LD (HL), E
  case 0x74: // LD (HL), H
  case 0x75: // LD (HL), L
  case 0x77: // LD (HL), A
    cpu->current_opcode = OP_LD_HL_REF_R8;
    cpu->cycles_left = 2;
    break;
  case 0x76: // HALT
    cpu->current_opcode = OP_HALT;
    cpu->cycles_left = 1;
    break;
  case 0x80: // ADD A, B
  case 0x81: // ADD A, C
  case 0x82: // ADD A, D
  case 0x83: // ADD A, E
  case 0x84: // ADD A, H
  case 0x85: // ADD A, L
  case 0x87: // ADD A, A
    cpu->current_opcode = OP_ADD_A_R8;
    cpu->cycles_left = 1;
    break;
  case 0x86: // ADD A, (HL)
    cpu->current_opcode = OP_ADD_A_HL_REF;
    cpu->cycles_left = 2;
    break;
  case 0x88: // ADC A, B
  case 0x89: // ADC A, C
  case 0x8A: // ADC A, D
  case 0x8B: // ADC A, E
  case 0x8C: // ADC A, H
  case 0x8D: // ADC A, L
  case 0x8F: // ADC A, A
    cpu->current_opcode = OP_ADC_A_R8;
    cpu->cycles_left = 1;
    break;
  case 0x8E: // ADC A, (HL)
    cpu->current_opcode = OP_ADC_A_HL_REF;
    cpu->cycles_left = 2;
    break;
  case 0x90: // SUB A, B
  case 0x91: // SUB A, C
  case 0x92: // SUB A, D
  case 0x93: // SUB A, E
  case 0x94: // SUB A, H
  case 0x95: // SUB A, L
  case 0x97: // SUB A, A
    cpu->current_opcode = OP_SUB_A_R8;
    cpu->cycles_left = 1;
    break;
  case 0x96: // SUB A, (HL)
    cpu->current_opcode = OP_SUB_A_HL_REF;
    cpu->cycles_left = 2;
    break;
  case 0x98: // SBC A, B
  case 0x99: // SBC A, C
  case 0x9A: // SBC A, D
  case 0x9B: // SBC A, E
  case 0x9C: // SBC A, H
  case 0x9D: // SBC A, L
  case 0x9F: // SBC A, A
    cpu->current_opcode = OP_SBC_A_R8;
    cpu->cycles_left = 1;
    break;
  case 0x9E: // SBC A, (HL)
    cpu->current_opcode = OP_SBC_A_HL_REF;
    cpu->cycles_left = 2;
    break;
  case 0xA0: // AND A, B
  case 0xA1: // AND A, C
  case 0xA2: // AND A, D
  case 0xA3: // AND A, E
  case 0xA4: // AND A, H
  case 0xA5: // AND A, L
  case 0xA7: // AND A, A
    cpu->current_opcode = OP_AND_A_R8;
    cpu->cycles_left = 1;
    break;
  case 0xA6: // AND A, (HL)
    cpu->current_opcode = OP_AND_A_HL_REF;
    cpu->cycles_left = 2;
    break;
  case 0xA8: // XOR A, B
  case 0xA9: // XOR A, C
  case 0xAA: // XOR A, D
  case 0xAB: // XOR A, E
  case 0xAC: // XOR A, H
  case 0xAD: // XOR A, L
  case 0xAF: // XOR A, A
    cpu->current_opcode = OP_XOR_A_R8;
    cpu->cycles_left = 1;
    break;
  case 0xAE: // XOR A, (HL)
    cpu->current_opcode = OP_XOR_A_HL_REF;
    cpu->cycles_left = 2;
    break;
  case 0xB0: // OR A, B
  case 0xB1: // OR A, C
  case 0xB2: // OR A, D
  case 0xB3: // OR A, E
  case 0xB4: // OR A, H
  case 0xB5: // OR A, L
  case 0xB7: // OR A, A
    cpu->current_opcode = OP_OR_A_R8;
    cpu->cycles_left = 1;
    break;
  case 0xB6: // OR A, (HL)
    cpu->current_opcode = OP_OR_A_HL_REF;
    cpu->cycles_left = 2;
    break;
  case 0xB8: // CP A, B
  case 0xB9: // CP A, C
  case 0xBA: // CP A, D
  case 0xBB: // CP A, E
  case 0xBC: // CP A, H
  case 0xBD: // CP A, L
  case 0xBF: // CP A, A
    cpu->current_opcode = OP_CP_A_R8;
    cpu->cycles_left = 1;
    break;
  case 0xBE: // CP A, (HL)
    cpu->current_opcode = OP_CP_A_HL_REF;
    cpu->cycles_left = 2;
    break;
  case 0xC0: // RET NZ
    cpu->current_opcode = OP_RET_NZ;
    cpu->cycles_left = 5;
    break;
  case 0xC1: // POP BC
  case 0xD1: // POP DE
  case 0xE1: // POP HL
  case 0xF1: // POP AF
    cpu->current_opcode = OP_POP_R16;
    cpu->cycles_left = 3;
    break;
  case 0xC2: // JP NZ, a16
    cpu->current_opcode = OP_JP_NZ_IMM16;
    cpu->cycles_left = 4;
    break;
  case 0xC3: // JP a16
    cpu->current_opcode = OP_JP_IMM16;
    cpu->cycles_left = 4;
    break;
  case 0xC4: // CALL NZ, a16
    cpu->current_opcode = OP_CALL_NZ_IMM16;
    cpu->cycles_left = 6;
    break;
  case 0xC5: // PUSH BC
  case 0xD5: // PUSH DE
  case 0xE5: // PUSH HL
  case 0xF5: // PUSH AF
    cpu->current_opcode = OP_PUSH_R16;
    cpu->cycles_left = 4;
    break;
  case 0xC6: // ADD A, d8
    cpu->current_opcode = OP_ADD_A_IMM8;
    cpu->cycles_left = 2;
    break;
  case 0xC7: // RST 00H
  case 0xCF: // RST 08H
  case 0xD7: // RST 10H
  case 0xDF: // RST 18H
  case 0xE7: // RST 20H
  case 0xEF: // RST 28H
  case 0xF7: // RST 30H
  case 0xFF: // RST 38H
    cpu->current_opcode = OP_RST;
    cpu->cycles_left = 4;
    break;
  case 0xC8: // RET Z
    cpu->current_opcode = OP_RET_ZERO;
    cpu->cycles_left = 5;
    break;
  case 0xC9: // RET
    cpu->current_opcode = OP_RET;
    cpu->cycles_left = 4;
    break;
  case 0xCA: // JP Z, a16
    cpu->current_opcode = OP_JP_Z_IMM16;
    cpu->cycles_left = 4;
    break;
  case 0xCB: // CB prefix
    cpu->current_opcode = OP_CB_PREFIX;
    cpu->cycles_left = 2;
    break;
  case 0xCC: // CALL Z, a16
    cpu->current_opcode = OP_CALL_Z_IMM16;
    cpu->cycles_left = 6;
    break;
  case 0xCD: // CALL a16
    cpu->current_opcode = OP_CALL_IMM16;
    cpu->cycles_left = 6;
    break;
  case 0xCE: // ADC A, d8
    cpu->current_opcode = OP_ADC_A_IMM8;
    cpu->cycles_left = 2;
    break;
  case 0xD0: // RET NC
    cpu->current_opcode = OP_RET_NC;
    cpu->cycles_left = 5;
    break;
  case 0xD2: // JP NC, a16
    cpu->current_opcode = OP_JP_NC_IMM16;
    cpu->cycles_left = 4;
    break;
  case 0xD4: // CALL NC, a16
    cpu->current_opcode = OP_CALL_NC_IMM16;
    cpu->cycles_left = 6;
    break;
  case 0xD6: // SUB A, d8
    cpu->current_opcode = OP_SUB_A_IMM8;
    cpu->cycles_left = 2;
    break;
  case 0xD8: // RET C
    cpu->current_opcode = OP_RET_C;
    cpu->cycles_left = 5;
    break;
  case 0xD9: // RETI
    cpu->current_opcode = OP_RETI;
    cpu->cycles_left = 4;
    break;
  case 0xDA: // JP C, a16
    cpu->current_opcode = OP_JP_C_IMM16;
    cpu->cycles_left = 4;
    break;
  case 0xDC: // CALL C, a16
    cpu->current_opcode = OP_CALL_C_IMM16;
    cpu->cycles_left = 6;
    break;
  case 0xDE: // SBC A, d8
    cpu->current_opcode = OP_SBC_A_IMM8;
    cpu->cycles_left = 2;
    break;
  case 0xE0: // LDH (a8), A
    cpu->current_opcode = OP_LDH_REF8_A;
    cpu->cycles_left = 3;
    break;
  case 0xE2: // LDH (C), A
    cpu->current_opcode = OP_LDH_C_REF_A;
    cpu->cycles_left = 2;
    break;
  case 0xE6: // AND A, d8
    cpu->current_opcode = OP_AND_A_IMM8;
    cpu->cycles_left = 2;
    break;
  case 0xE8: // ADD SP, r8
    cpu->current_opcode = OP_ADD_SP_S8;
    cpu->cycles_left = 4;
    break;
  case 0xE9: // JP (HL)
    cpu->current_opcode = OP_JP_HL;
    cpu->cycles_left = 1;
    break;
  case 0xEA: // LD (a16), A
    cpu->current_opcode = OP_LD_REF16_A;
    cpu->cycles_left = 4;
    break;
  case 0xEE: // XOR A, d8
    cpu->current_opcode = OP_XOR_A_IMM8;
    cpu->cycles_left = 2;
    break;
  case 0xF0: // LDH A, (a8)
    cpu->current_opcode = OP_LDH_A_REF8;
    cpu->cycles_left = 3;
    break;
  case 0xF2: // LDH A, (C)
    cpu->current_opcode = OP_LDH_A_C_REF;
    cpu->cycles_left = 2;
    break;
  case 0xF3: // DI
    cpu->current_opcode = OP_DI;
    cpu->cycles_left = 1;
    break;
  case 0xF6: // OR A, d8
    cpu->current_opcode = OP_OR_A_IMM8;
    cpu->cycles_left = 2;
    break;
  case 0xF8: // LD HL, SP+r8
    cpu->current_opcode = OP_LD_HL_SP_PLUS_S8;
    cpu->cycles_left = 3;
    break;
  case 0xF9: // LD SP, HL
    cpu->current_opcode = OP_LD_SP_HL;
    cpu->cycles_left = 2;
    break;
  case 0xFA: // LD A, (a16)
    cpu->current_opcode = OP_LD_A_REF16;
    cpu->cycles_left = 4;
    break;
  case 0xFB: // EI
    cpu->current_opcode = OP_EI;
    cpu->cycles_left = 1;
    break;
  case 0xFE: // CP A, d8
    cpu->current_opcode = OP_CP_A_IMM8;
    cpu->cycles_left = 2;
    break;
  default:
    fprintf(stderr, "[Decoder] Unknown opcode: 0x%02X\n", opcode);
    exit(EXIT_FAILURE);
  }
  execute_instruction(cpu);
}

void decode_cb_instruction(cpu_t *const restrict cpu) {
  uint8_t cb_opcode = cpu->ram[cpu->pc];
  switch (cb_opcode) {
  case 0x00: // RLC B
  case 0x01: // RLC C
  case 0x02: // RLC D
  case 0x03: // RLC E
  case 0x04: // RLC H
  case 0x05: // RLC L
  case 0x07: // RLC A
    cpu->current_opcode = CB_RLC_R8;
    cpu->cycles_left = 1;
    break;
  case 0x06: // RLC (HL)
    cpu->current_opcode = CB_RLC_R8;
    cpu->cycles_left = 3;
    break;
  case 0x08: // RRC B
  case 0x09: // RRC C
  case 0x0A: // RRC D
  case 0x0B: // RRC E
  case 0x0C: // RRC H
  case 0x0D: // RRC L
  case 0x0F: // RRC A
    cpu->current_opcode = CB_RRC_R8;
    cpu->cycles_left = 1;
    break;
  case 0x0E: // RRC (HL)
    cpu->current_opcode = CB_RRC_R8;
    cpu->cycles_left = 3;
    break;
  default:
    fprintf(stderr, "[Decoder] Unknown CB opcode: 0x%02X\n", cb_opcode);
    exit(EXIT_FAILURE);
  }
  execute_instruction(cpu);
}