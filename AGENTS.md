# AGENTS

Quick instructions for AI coding agents working in this repository.

## Project at a glance

- Language/tooling: C and C++ tests with CMake.
- Main library target: `GameCoy`.
- Main test target: `json_tests` (doctest + nlohmann/json).
- Core CPU state is currently exposed via `cpu_t` in `include/cpu.h`.

## Build and test

- Preferred build/test flow (also in `Taskfile.yml`):
  - `cmake --build build`
  - `./build/json_tests`
- If `build/` is missing or stale, configure first:
  - `cmake -S . -B build`

## Code layout and boundaries

- Public headers: `include/`
- Core implementation: `src/`
- Tests: `tests/json_tests.cpp`
- Vendored dependencies and test vectors: `external/`

Do not modify vendored code under `external/` unless explicitly requested.

## Conventions used in this codebase

- Prefer small C functions operating on `cpu_t *`.
- Common signature style: `cpu_t *const restrict cpu` in C implementation files.
- Use fixed-width integer types from `<stdint.h>`.
- Keep existing formatting style (2-space indentation, brace placement as in surrounding file).
- Keep changes surgical; avoid broad refactors unless requested.

## Testing guidance

- After behavior changes in `src/` or `include/`, run:
  - `cmake --build build`
  - `./build/json_tests`
- The JSON-driven instruction tests in `tests/json_tests.cpp` are the primary regression signal.

## Useful references

- Build config: [CMakeLists.txt](CMakeLists.txt)
- Convenience tasks: [Taskfile.yml](Taskfile.yml)
- CPU API/state shape: [include/cpu.h](include/cpu.h)
- Execution loop: [src/cpu.c](src/cpu.c)
- Main tests: [tests/json_tests.cpp](tests/json_tests.cpp)
