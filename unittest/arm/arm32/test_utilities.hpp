// MIT License
//
// lzasm: a runtime assembler
// Copyright 2021 Thomas Mathys
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

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
