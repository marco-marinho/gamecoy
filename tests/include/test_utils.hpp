#include "cpu.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

void load_test(cpu_t *cpu, const json &state);
void verify_test(const cpu_t *cpu, const json &expected);