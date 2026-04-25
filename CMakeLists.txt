cmake_minimum_required(VERSION 4.3)
include(CheckIPOSupported)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

project(
  GameCoy
  VERSION 1.0
  LANGUAGES C CXX)

check_ipo_supported(RESULT ipo_supported OUTPUT error)

add_library(GameCoy
  src/decoder.c
  src/executor.c
  src/cpu.c
  src/alu.c
  src/bit_shift.c
  src/bitflag.c
  src/mmu.c
  src/jumps.c
  src/util.c
  src/bitwise.c
  src/misc.c)

target_include_directories(GameCoy
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(cpu_tests
  tests/src/cpu_tests.cpp
  tests/src/test_utils.cpp)

target_include_directories(cpu_tests
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/external/json/single_include
  ${CMAKE_CURRENT_SOURCE_DIR}/external/doctest/doctest
  ${CMAKE_CURRENT_SOURCE_DIR}/tests/include)

target_link_libraries(cpu_tests
  PRIVATE
  GameCoy)

if(ipo_supported)
    message(STATUS "LTO/IPO is supported and enabled.")
    set_target_properties(GameCoy PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
    set_target_properties(cpu_tests PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
else()
    message(WARNING "LTO/IPO not supported: ${error}")
endif()

enable_testing()
add_test(NAME cpu_tests COMMAND cpu_tests)
set_tests_properties(cpu_tests PROPERTIES
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
