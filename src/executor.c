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
  case OP_LD_HL_REF_R8:
    ld_hl_ref_r8(cpu);
    break;
  case OP_HALT:
    halt(cpu);
    break;
  case OP_ADD_A_R8:
    add_a_r8(cpu);
    break;
  case OP_ADD_A_HL_REF:
    add_a_hl_ref(cpu);
    break;
  case OP_ADC_A_R8:
    adc_a_r8(cpu);
    break;
  case OP_ADC_A_HL_REF:
    adc_a_hl_ref(cpu);
    break;
  case OP_SUB_A_R8:
    sub_a_r8(cpu);
    break;
  case OP_SUB_A_HL_REF:
    sub_a_hl_ref(cpu);
    break;
  case OP_SBC_A_R8:
    sbc_a_r8(cpu);
    break;
  case OP_SBC_A_HL_REF:
    sbc_a_hl_ref(cpu);
    break;
  case OP_AND_A_R8:
    and_a_r8(cpu);
    break;
  case OP_AND_A_HL_REF:
    and_a_hl_ref(cpu);
    break;
  case OP_XOR_A_R8:
    xor_a_r8(cpu);
    break;
  case OP_XOR_A_HL_REF:
    xor_a_hl_ref(cpu);
    break;
  case OP_OR_A_R8:
    or_a_r8(cpu);
    break;
  case OP_OR_A_HL_REF:
    or_a_hl_ref(cpu);
    break;
  case OP_CP_A_R8:
    cp_a_r8(cpu);
    break;
  case OP_CP_A_HL_REF:
    cp_a_hl_ref(cpu);
    break;
  case OP_RET_NZ:
    ret_nz(cpu);
    break;
  case OP_POP_R16:
    pop_r16(cpu);
    break;
  case OP_JP_NZ_IMM16:
    jp_nz_imm16(cpu);
    break;
  case OP_JP_IMM16:
    jp_imm16(cpu);
    break;
  case OP_CALL_NZ_IMM16:
    call_nz_imm16(cpu);
    break;
  case OP_PUSH_R16:
    push_r16(cpu);
    break;
  case OP_ADD_A_IMM8:
    add_a_imm8(cpu);
    break;
  case OP_RST:
    rst(cpu);
    break;
  case OP_RET_ZERO:
    ret_z(cpu);
    break;
  case OP_RET:
    ret(cpu);
    break;
  case OP_JP_Z_IMM16:
    jp_z_imm16(cpu);
    break;
  case OP_CALL_Z_IMM16:
    call_z_imm16(cpu);
    break;
  case OP_CALL_IMM16:
    call_imm16(cpu);
    break;
  case OP_ADC_A_IMM8:
    adc_a_imm8(cpu);
    break;
  case OP_RET_NC:
    ret_nc(cpu);
    break;
  case OP_JP_NC_IMM16:
    jp_nc_imm16(cpu);
    break;
  case OP_CALL_NC_IMM16:
    call_nc_imm16(cpu);
    break;
  case OP_SUB_A_IMM8:
    sub_a_imm8(cpu);
    break;
  case OP_RET_C:
    ret_c(cpu);
    break;
  case OP_RETI:
    reti(cpu);
    break;
  case OP_JP_C_IMM16:
    jp_c_imm16(cpu);
    break;
  case OP_CALL_C_IMM16:
    call_c_imm16(cpu);
    break;
  case OP_SBC_A_IMM8:
    sbc_a_imm8(cpu);
    break;
  case OP_LDH_IMM8_REF_A:
    ldh_imm8_ref_a(cpu);
    break;
  case OP_LDH_C_REF_A:
    ldh_c_ref_a(cpu);
    break;
  default:
    printf("[Executor] Unknown opcode: 0x%02X\n", cpu->current_opcode);
    break;
  }
  if (cpu->ie_delay > 0) {
    cpu->ie_delay -= 1;
    if (cpu->ie_delay == 0) {
      cpu->ime = 1;
    }
  }
  if (cpu->cycles_left == 0) {
    cpu->current_opcode = OP_MUST_FETCH;
  }
}