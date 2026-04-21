#include "cpu.h"
#include "executor.h"
#include "instructions.h"
#include <stdint.h>
#include <stdio.h>

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
    cpu->current_opcode = OP_LD_IMM16REF_SP;
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
    cpu->current_opcode = OP_LD_R8_R8;
    cpu->cycles_left = 1;
    break;
  case 0x46: // LD B, (HL)
  case 0x4E: // LD C, (HL)
  case 0x56: // LD D, (HL)
  case 0x5E: // LD E, (HL)
  case 0x66: // LD H, (HL)
  case 0x6E: // LD L, (HL)
    cpu->current_opcode = OP_LD_R8_HL_REF;
    cpu->cycles_left = 2;
    break;
  default:
    printf("[Decoder] Unknown opcode: 0x%02X\n", opcode);
    break;
  }
  execute_instruction(cpu);
}