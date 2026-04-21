#include "executor.h"
#include "alu.h"
#include "bit_shift.h"
#include "bitwise.h"
#include "cpu.h"
#include "instructions.h"
#include "jumps.h"
#include "misc.h"
#include "mmu.h"
#include <stdio.h>

void execute_instruction(cpu_t *const restrict cpu) {
  switch (cpu->current_opcode) {
  case OP_NOP:
    nop(cpu);
    break;
  case OP_LD_R16_IMM16:
    ld_r16_imm16(cpu);
    break;
  case OP_LD_R16REF_A:
    ld_r16ref_a(cpu);
    break;
  case OP_INC_R16:
    inc_r16(cpu);
    break;
  case OP_INC_R8:
    inc_r8(cpu);
    break;
  case OP_DEC_R8:
    dec_r8(cpu);
    break;
  case OP_LD_R8_IMM8:
    ld_r8_imm8(cpu);
    break;
  case OP_RLCA:
    rlca(cpu);
    break;
  case OP_LD_IMM16REF_SP:
    ld_imm16ref_sp(cpu);
    break;
  case OP_ADD_HL_R16:
    add_hl_r16(cpu);
    break;
  case OP_LD_A_R16REF:
    ld_a_r16ref(cpu);
    break;
  case OP_DEC_R16:
    dec_r16(cpu);
    break;
  case OP_RRCA:
    rrca(cpu);
    break;
  case OP_STOP:
    stop(cpu);
    break;
  case OP_RLA:
    rla(cpu);
    break;
  case OP_JR_S8:
    jr_s8(cpu);
    break;
  case OP_RRA:
    rra(cpu);
    break;
  case OP_JR_NZ_S8:
    jr_nz_s8(cpu);
    break;
  case OP_LD_HL_PLUS_REF_A:
    ld_hl_plus_ref_a(cpu);
    break;
  case OP_DAA:
    daa(cpu);
    break;
  case OP_JR_Z_S8:
    jr_z_s8(cpu);
    break;
  case OP_LD_A_HL_PLUS_REF:
    ld_a_hl_plus_ref(cpu);
    break;
  case OP_CPL:
    cpl(cpu);
    break;
  case OP_JR_NC_S8:
    jr_nc_s8(cpu);
    break;
  case OP_LD_SP_IMM16:
    ld_sp_imm16(cpu);
    break;
  case OP_LD_HL_MINUS_REF_A:
    ld_hl_minus_ref_a(cpu);
    break;
  case OP_INC_SP:
    inc_sp(cpu);
    break;
  case OP_INC_HL_REF:
    inc_hl_ref(cpu);
    break;
  case OP_DEC_HL_REF:
    dec_hl_ref(cpu);
    break;
  case OP_LD_HL_REF_IMM8:
    ld_hl_ref_imm8(cpu);
    break;
  case OP_SCF:
    scf(cpu);
    break;
  case OP_JR_C_S8:
    jr_c_s8(cpu);
    break;
  case OP_ADD_HL_SP:
    add_hl_sp(cpu);
    break;
  case OP_LD_A_HL_MINUS_REF:
    ld_a_hl_minus_ref(cpu);
    break;
  case OP_DEC_SP:
    dec_sp(cpu);
    break;
  case OP_CCF:
    ccf(cpu);
    break;
  case OP_LD_R8_R8:
    ld_r8_r8(cpu);
    break;
  case OP_LD_R8_HL_REF:
    ld_r8_hl_ref(cpu);
    break;
  default:
    printf("[Executor] Unknown opcode: 0x%02X\n", cpu->current_opcode);
    break;
  }
  if (cpu->cycles_left == 0) {
    cpu->current_opcode = OP_MUST_FETCH;
  }
}