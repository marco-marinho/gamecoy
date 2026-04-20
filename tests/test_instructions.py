from gamecoy import ffi, lib # type: ignore
import json
import pytest

testable = ["00", "01", "02", "03", "04", "05", "06", "07", "08", "09", "0a", "0b", "0c", "0d", "0e", "0f", 
            "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "1a", "1b", "1c", "1d", "1e", "1f",
            "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "2a", "2b", "2c", "2d", "2e", "2f",][::-1]
TEST_CASES = []
for test in testable:
    with open(f"external/sm83/v1/{test}.json") as f:
        TEST_CASES.extend(json.load(f))
    
def verify_equal(label, actual, expected):
    assert actual == expected, (
        f"{label} mismatch: (expected={expected!r}, actual={actual!r})"
    )

def load_test(cpu, test_dict):
    cpu.pc = test_dict["pc"]
    cpu.sp = test_dict["sp"]
    cpu.registers[0] = test_dict["a"]
    cpu.registers[1] = test_dict["b"]
    cpu.registers[2] = test_dict["c"]
    cpu.registers[3] = test_dict["d"]
    cpu.registers[4] = test_dict["e"]
    cpu.registers[5] = test_dict["f"]
    cpu.registers[7] = test_dict["h"]
    cpu.registers[8] = test_dict["l"]
    for (addr, value) in test_dict["ram"]:
        cpu.ram[addr] = value

def verify_test(cpu, expected_dict):
    verify_equal("pc", cpu.pc, expected_dict["pc"])
    verify_equal("sp", cpu.sp, expected_dict["sp"])
    verify_equal("register a", cpu.registers[0], expected_dict["a"])
    verify_equal("register b", cpu.registers[1], expected_dict["b"])
    verify_equal("register c", cpu.registers[2], expected_dict["c"])
    verify_equal("register d", cpu.registers[3], expected_dict["d"])
    verify_equal("register e", cpu.registers[4], expected_dict["e"])
    verify_equal("register f", cpu.registers[5], expected_dict["f"])
    verify_equal("register h", cpu.registers[7], expected_dict["h"])
    verify_equal("register l", cpu.registers[8], expected_dict["l"])
    for addr, value in expected_dict["ram"]:
        verify_equal(f"ram[0x{addr:04x}]", cpu.ram[addr], value)

def case_id(test_case):
    name = test_case.get("name")
    if name:
        return name
    return f"pc_{test_case['initial']['pc']:04x}"

@pytest.mark.parametrize("test_case", TEST_CASES, ids=case_id)
def test_instructions_json(test_case):
    initial_state = test_case["initial"]
    cpu = ffi.new("cpu_t *")
    load_test(cpu, initial_state)
    lib.tick(cpu)
    cycles = 1
    while(cpu.cycles_left > 0):
        lib.tick(cpu)
        cycles +=1
    expected_state = test_case["final"]
    verify_test(cpu, expected_state)
    if not test_case["name"].startswith("10"):
        assert cycles == len(test_case["cycles"]), f"[Case {test_case['name']}] cycle count mismatch: (expected={len(test_case['cycles'])}, actual={cycles})"
