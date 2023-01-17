// SPDX-FileCopyrightText: 2021 Thomas Mathys
// SPDX-License-Identifier: MIT
// lzasm: a runtime assembler

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
