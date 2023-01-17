// SPDX-FileCopyrightText: 2021 Thomas Mathys
// SPDX-License-Identifier: MIT
// lzasm: a runtime assembler

#ifndef LZASM_ARM_ARM32_DETAIL_UTILITIES_HPP_INCLUDED
#define LZASM_ARM_ARM32_DETAIL_UTILITIES_HPP_INCLUDED

#include <stdexcept>
#include <type_traits>
#include "lzasm/arm/arm32/detail/basic_types.hpp"

namespace lzasm::arm::arm32::detail
{

template <typename T>
constexpr auto clear_bit1(T value)
{
    return value & ~2;
}

template <typename T>
constexpr auto get_byte_alignment(T alignment) noexcept
{
    assert((alignment >= 0) && (alignment <= max_alignment));
    return 1 << alignment;
}

inline void report_error(const char* message)
{
    throw std::runtime_error(message);
}

template <typename E>
constexpr auto to_underlying(E enumerator) noexcept
{
    return static_cast<std::underlying_type_t<E>>(enumerator);
}

}

#endif
