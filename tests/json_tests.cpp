#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "cpu.h"
#include "doctest.h"
#include "nlohmann/json.hpp"
#include <cstdio>
#include <fstream>
#include <string>
#include <vector>

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

TEST_CASE("SM83 Instruction JSON Tests") {
  std::vector<std::string> testable;
  testable.reserve(0xFFFF);
  for (int op = 0x00; op <= 0x5F; ++op) {
    char hex[3];
    std::snprintf(hex, sizeof(hex), "%02x", op);
    testable.emplace_back(hex);
  }

  for (const auto &op : testable) {
    std::string path = "external/sm83/v1/" + op + ".json";
    std::ifstream f(path);

    if (!f.is_open())
      continue;

    json test_cases;
    f >> test_cases;

    for (const auto &test_case : test_cases) {
      std::string name = test_case.contains("name")
                             ? test_case["name"].get<std::string>()
                             : "unnamed";

      INFO("Testing case: ", name);

      cpu_t cpu = {};
      load_test(&cpu, test_case["initial"]);

      tick(&cpu);
      int cycles = 1;
      while (cpu.cycles_left > 0) {
        tick(&cpu);
        cycles++;
      }

      verify_test(&cpu, test_case["final"]);

      if (name.find("10") != 0) {
        size_t expected_cycles = test_case["cycles"].size();
        CHECK_MESSAGE(cycles == expected_cycles,
                      "Cycle count mismatch in " << name);
      }
    }
  }
}