#include "cpu.h"
#include "decoder.h"
#include "executor.h"

void tick(cpu_t *const restrict cpu) {
  if (cpu->halted) {
    return;
  }
  if (cpu->current_opcode == OP_MUST_FETCH) {
    decode_instruction(cpu);
  } else {
    execute_instruction(cpu);
  }
}