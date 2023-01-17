// SPDX-FileCopyrightText: 2021 Thomas Mathys
// SPDX-License-Identifier: MIT
// lzasm: a runtime assembler

#ifndef LZASM_TEST_UTILITIES_HPP_INCLUDED
#define LZASM_TEST_UTILITIES_HPP_INCLUDED

#include <vector>
#include "lzasm/arm/arm32/detail/basic_types.hpp"

namespace lzasm_unittest
{

using bytevector = ::lzasm::arm::arm32::bytevector;
using hwordvector = std::vector<uint16_t>;
using wordvector = std::vector<uint32_t>;

bytevector to_bytevector();
bytevector to_bytevector(const bytevector& bytes);
bytevector to_bytevector(const hwordvector& halfwords);
bytevector to_bytevector(const wordvector& words);

#define B(...) bytevector { __VA_ARGS__ }
#define H(...) hwordvector { __VA_ARGS__ }
#define W(...) wordvector { __VA_ARGS__ }

bool is_alignment_out_of_range(const std::exception& e);
bool is_immediate_out_of_range(const std::exception& e);
bool is_misaligned_immediate_value(const std::exception& e);
bool is_origin_too_large(const std::exception& e);
bool is_symbol_already_defined(const std::exception& e);
bool is_undefined_symbol(const std::exception& e);
bool is_unpredictable_behavior(const std::exception& e);

}

#endif
