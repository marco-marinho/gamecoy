#include "cpu.h"
#include "doctest.h"
#include "nlohmann/json.hpp"
#include <cstdio>

using json = nlohmann::json;

void load_test(cpu_t *cpu, const json &state) {
  cpu->pc = state["pc"];
  cpu->sp = state["sp"];
  cpu->registers[0] = state["a"];
  cpu->registers[1] = state["b"];
  cpu->registers[2] = state["c"];
  cpu->registers[3] = state["d"];
  cpu->registers[4] = state["e"];
  cpu->registers[5] = state["f"];
  cpu->registers[7] = state["h"];
  cpu->registers[8] = state["l"];

  for (auto &entry : state["ram"]) {
    uint16_t addr = entry[0];
    uint8_t val = entry[1];
    cpu->ram[addr] = val;
  }
}

void verify_test(const cpu_t *cpu, const json &expected) {
  CHECK_MESSAGE(cpu->pc == (uint16_t)expected["pc"], "PC mismatch");
  CHECK_MESSAGE(cpu->sp == (uint16_t)expected["sp"], "SP mismatch");
  CHECK_MESSAGE(cpu->registers[0] == (uint8_t)expected["a"], "Reg A mismatch");
  CHECK_MESSAGE(cpu->registers[1] == (uint8_t)expected["b"], "Reg B mismatch");
  CHECK_MESSAGE(cpu->registers[2] == (uint8_t)expected["c"], "Reg C mismatch");
  CHECK_MESSAGE(cpu->registers[3] == (uint8_t)expected["d"], "Reg D mismatch");
  CHECK_MESSAGE(cpu->registers[4] == (uint8_t)expected["e"], "Reg E mismatch");
  CHECK_MESSAGE(cpu->registers[5] == (uint8_t)expected["f"], "Reg F mismatch");
  CHECK_MESSAGE(cpu->registers[7] == (uint8_t)expected["h"], "Reg H mismatch");
  CHECK_MESSAGE(cpu->registers[8] == (uint8_t)expected["l"], "Reg L mismatch");

  for (auto &entry : expected["ram"]) {
    uint16_t addr = entry[0];
    uint8_t val = entry[1];
    CAPTURE(addr);
    CHECK(cpu->ram[addr] == val);
  }
}