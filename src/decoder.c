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
    cpu->current_opcode = OP_INC_R8;
    cpu->cycles_left = 1;
    break;
  case 0x05: // DEC B
  case 0x0D: // DEC C
  case 0x15: // DEC D
  case 0x1D: // DEC E
  case 0x25: // DEC H
  case 0x2D: // DEC L
    cpu->current_opcode = OP_DEC_R8;
    cpu->cycles_left = 1;
    break;
  case 0x06: // LD B, d8
  case 0x0E: // LD C, d8
  case 0x16: // LD D, d8
  case 0x1E: // LD E, d8
  case 0x26: // LD H, d8
  case 0x2E: // LD L, d8
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
  default:
    printf("[Decoder] Unknown opcode: 0x%02X\n", opcode);
    break;
  }
  execute_instruction(cpu);
}