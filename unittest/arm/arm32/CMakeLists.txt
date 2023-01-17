# SPDX-FileCopyrightText: 2021 Thomas Mathys
# SPDX-License-Identifier: MIT
# lzasm: a runtime assembler

set(
  SOURCES
  assembler_test_utilities.cpp
  assembler_test_utilities.hpp
  divided_thumb_assembler_test.add_and_subtract_immediate.cpp
  divided_thumb_assembler_test.add_and_subtract_register.cpp
  divided_thumb_assembler_test.add_offset_to_sp.cpp
  divided_thumb_assembler_test.alu_operation.cpp
  divided_thumb_assembler_test.arm_code_generation_pseudo_instructions.cpp
  divided_thumb_assembler_test.conditional_branch.cpp
  divided_thumb_assembler_test.current_lc.cpp
  divided_thumb_assembler_test.data_definition_directives.cpp
  divided_thumb_assembler_test.high_register_operation.cpp
  divided_thumb_assembler_test.immediate_operation.cpp
  divided_thumb_assembler_test.label_definitions_and_references.cpp
  divided_thumb_assembler_test.link.cpp
  divided_thumb_assembler_test.load_address.cpp
  divided_thumb_assembler_test.load_store_halfword.cpp
  divided_thumb_assembler_test.load_store_sign_extended.cpp
  divided_thumb_assembler_test.load_store_with_immediate_offset.cpp
  divided_thumb_assembler_test.load_store_with_register_offset.cpp
  divided_thumb_assembler_test.long_branch_with_link.cpp
  divided_thumb_assembler_test.miscellaneous_directives.cpp
  divided_thumb_assembler_test.move_shifted_register.cpp
  divided_thumb_assembler_test.multiple_load_store.cpp
  divided_thumb_assembler_test.pc_relative_load.cpp
  divided_thumb_assembler_test.pseudo_instructions.cpp
  divided_thumb_assembler_test.push_pop.cpp
  divided_thumb_assembler_test.software_interrupt.cpp
  divided_thumb_assembler_test.sp_relative_load_store.cpp
  divided_thumb_assembler_test.unconditional_branch.cpp
  immediate_test.cpp
  main.cpp
  object_test.cpp
  reference_type_descriptor_test.cpp
  register_lists_test.cpp
  shrinkler_depacker_test.cpp
  symbol_test.cpp
  test_utilities.cpp
  test_utilities.hpp)

add_executable(divided_thumb_assembler-unittest ${SOURCES})

target_include_directories(divided_thumb_assembler-unittest PRIVATE "${Boost_INCLUDE_DIRS}")
target_link_libraries(divided_thumb_assembler-unittest PRIVATE lzasm)

target_precompile_headers(
  divided_thumb_assembler-unittest
  PRIVATE
  <boost/test/unit_test.hpp>
  <string>
  [["lzasm/arm/arm32/divided_thumb_assembler.hpp"]]
  "assembler_test_utilities.hpp"
  "test_utilities.hpp")
set_source_files_properties(main.cpp PROPERTIES SKIP_PRECOMPILE_HEADERS ON)

add_test(NAME divided_thumb_assembler-unittest COMMAND divided_thumb_assembler-unittest)
