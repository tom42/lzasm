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
