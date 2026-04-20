cmake_minimum_required(VERSION 4.3)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

project(
  GameCoy
  VERSION 1.0
  LANGUAGES C)

add_library(GameCoy
  src/decoder.c
  src/executor.c
  src/cpu.c
  src/alu.c
  src/bit_shift.c
  src/mmu.c
  src/jumps.c
  src/util.c
  src/bitwise.c
  src/misc.c)

target_include_directories(GameCoy
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include)

set_property(TARGET GameCoy PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)

add_custom_command(
  TARGET GameCoy
  POST_BUILD
  COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/cffi_wrapper.py) 