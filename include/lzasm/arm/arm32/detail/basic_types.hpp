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

#ifndef LZASM_ARM_ARM32_DETAIL_BASIC_TYPES_HPP_INCLUDED
#define LZASM_ARM_ARM32_DETAIL_BASIC_TYPES_HPP_INCLUDED

#include <cstdint>
#include <vector>

namespace lzasm::arm::arm32
{

using bytevector = std::vector<unsigned char>;

// Currently need to use fixed width integer types here.
// Using e.g. int_fast32_t causes address calculation errors when
// int_fast32_t is wider than 32 bits, e.g. with 64 bit gcc or clang.
using address_t = uint32_t;
using immediate_t = int32_t;
static_assert(sizeof(address_t) == sizeof(immediate_t), "address_t and immediate_t must have same size");

namespace detail
{

// max_address is what we need for the target CPU, so we don't derive its value from some type.
constexpr address_t max_address = 0xffffffff;

// Maximum alignment, e.g. for align directive.
// More than 31 is pointless, since 1 << 32 is 4GB on one hand, and on the other hand
// may result in undefined behavior when using 32 bit types.
constexpr auto max_alignment = 31;

}

}

#endif
