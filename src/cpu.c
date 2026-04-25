#include "cpu.h"
#include "decoder.h"
#include "executor.h"
#include "instructions.h"
#include "mmu.h"

void tick(cpu_t *const restrict cpu) {
  if (cpu->halted) {
    return;
  }
  if (cpu->_div_counter == 63) {
    cpu->_div_counter = 0;
    cpu->_ram[TIMER_DIVIDER] += 1;
  }
  else {
    cpu->_div_counter += 1;
  }
  if (cpu->current_opcode == OP_MUST_FETCH) {
    decode_instruction(cpu);
  } else if (cpu->current_opcode == OP_MUST_FETCH_CB) {
    decode_cb_instruction(cpu);
  } else {
    execute_instruction(cpu);
  }
}