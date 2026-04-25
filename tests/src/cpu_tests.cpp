#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "cpu.h"
#include "doctest.h"
#include "nlohmann/json.hpp"
#include "test_utils.hpp"
#include <cstdio>
#include <fstream>
#include <string>
#include <vector>

using json = nlohmann::json;


TEST_CASE("SM83 Instruction JSON Tests") {
  std::vector<std::string> testable;
  testable.reserve(0xFFFF);
  for (int op = 0x00; op <= 0xFF; ++op) {
    if (op == 0xCB)
      continue; // CB-prefixed opcodes are tested separately
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

      if (name.find("10") != 0 && name.find("76") != 0) {
        size_t expected_cycles = test_case["cycles"].size();
        CHECK_MESSAGE(cycles == expected_cycles,
                      "Cycle count mismatch in " << name);
      }
    }
  }
}

TEST_CASE("SM83 Instruction JSON Tests - CB Prefix") {
  std::vector<std::string> testable;
  testable.reserve(0xFFFF);
  for (int op = 0x00; op <= 0x0F; ++op) {
    char hex[3];
    std::snprintf(hex, sizeof(hex), "%02x", op);
    testable.emplace_back(hex);
  }

  for (const auto &op : testable) {
    std::string path = "external/sm83/v1/cb " + op + ".json";
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

      if (name.find("10") != 0 && name.find("76") != 0) {
        size_t expected_cycles = test_case["cycles"].size();
        CHECK_MESSAGE(cycles == expected_cycles,
                      "Cycle count mismatch in " << name);
      }
    }
  }
}