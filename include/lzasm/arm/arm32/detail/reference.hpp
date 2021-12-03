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

#ifndef LZASM_ARM_ARM32_DETAIL_REFERENCE_HPP_INCLUDED
#define LZASM_ARM_ARM32_DETAIL_REFERENCE_HPP_INCLUDED

#include "lzasm/arm/arm32/detail/basic_types.hpp"
#include "lzasm/arm/arm32/detail/immediate.hpp"
#include "lzasm/arm/arm32/detail/utilities.hpp"

namespace lzasm::arm::arm32::detail
{

enum class reference_type
{
    // Enum members are used as array index.
    // Order of members must match order of elements in reference_type_descriptors::descriptors.
    abs3,
    abs5,
    abs5_asr_lsr,
    abs6,
    abs7,
    abs8_add_sub,
    abs8_byte,
    abs8_unsigned,
    abs9_add_sub_sp,
    abs10,
    abs16,
    abs32,
    adr,
    arm_branch,
    bl,
    conditional_branch,
    unconditional_branch,
    literal,
};

class reference_type_descriptor final
{
public:
    constexpr reference_type_descriptor(
        reference_type type,
        immediate_t min,
        immediate_t max,
        immediate_t alignment,
        immediate_t bit_width,
        immediate_t bit_pos)
        : type(type), min(min), max(max), alignment(alignment), bit_width(bit_width), bit_pos(bit_pos), bit_mask((1LL << bit_width) - 1) {}

    // Source address offset for relative addressing modes caused by prefetch operation.
    constexpr address_t pc_offset() const
    {
        return type == reference_type::arm_branch ? 8 : 4;
    }

    constexpr bool require_pc_bit1_cleared() const
    {
        return (type == reference_type::adr) || (type == reference_type::literal);
    }

    const reference_type type;
    const immediate_t min;
    const immediate_t max;

    // Alignment, as number of least significant bits that are required to be zero.
    const immediate_t alignment;

    // The number of bits needed to encode the value in the opcode.
    const immediate_t bit_width;

    // The position of the value in the opcode.
    const immediate_t bit_pos;

    const immediate_t bit_mask;
};

class reference_type_descriptors final
{
public:
    static const reference_type_descriptor& get(reference_type type)
    {
        auto index = to_underlying(type);

        assert((index >= 0) && (index < std::ssize(descriptors)));
        assert(descriptors[index].type == type);

        return descriptors[index];
    }

private:
    reference_type_descriptors() = delete;

    static inline constexpr reference_type_descriptor descriptors[]
    {
        // Note: reference_type members are used as index for this array.
        // Order of array elements must match order of reference_type members.
        reference_type_descriptor(reference_type::abs3,                 -7,             7,              0,  3,  6),
        reference_type_descriptor(reference_type::abs5,                 0,              0x1f,           0,  5,  6),
        reference_type_descriptor(reference_type::abs5_asr_lsr,         0,              0x20,           0,  5,  6),
        reference_type_descriptor(reference_type::abs6,                 0,              0x1f * 2,       1,  5,  6),
        reference_type_descriptor(reference_type::abs7,                 0,              0x1f * 4,       2,  5,  6),
        reference_type_descriptor(reference_type::abs8_add_sub,         -0xff,          0xff,           0,  8,  0),
        reference_type_descriptor(reference_type::abs8_byte,            -0x80,          0xff,           0,  8,  0),
        reference_type_descriptor(reference_type::abs8_unsigned,        0,              0xff,           0,  8,  0),
        reference_type_descriptor(reference_type::abs9_add_sub_sp,      -0x7f * 4,      0x7f * 4,       2,  7,  0),
        reference_type_descriptor(reference_type::abs10,                0,              0xff * 4,       2,  8,  0),
        reference_type_descriptor(reference_type::abs16,                -0x8000,        0xffff,         0,  16, 0),
        reference_type_descriptor(reference_type::abs32,                -2147483648,    0x7fffffff,     0,  32, 0),
        reference_type_descriptor(reference_type::adr,                  0,              0xff * 4,       2,  8,  0),
        reference_type_descriptor(reference_type::arm_branch,           -0x800000 * 4,  0x7fffff * 4,   2,  24, 0),
        reference_type_descriptor(reference_type::bl,                   -0x200000 * 2,  0x1fffff * 2,   1,  22, 0),
        reference_type_descriptor(reference_type::conditional_branch,   -0x80 * 2,      0x7f * 2,       1,  8,  0),
        reference_type_descriptor(reference_type::unconditional_branch, -0x400 * 2,     0x3ff * 2,      1,  11, 0),
        reference_type_descriptor(reference_type::literal,              0,              0xff * 4,       2,  8,  0)
    };
};

template <typename TSymbolName>
class reference final
{
public:
    reference(reference_type type, address_t fixup_location, const immediate<TSymbolName>& value)
        : type(type), fixup_location(fixup_location), value(value) {}

    const reference_type type;
    const address_t fixup_location;

    // For PC relative addressing schemes we need to resolve things at link time
    // even if the value is a numeric literal and not a symbol, so we keep a
    // value here and not just a symbol name.
    const immediate<TSymbolName> value;
};

class reference_to_literal final
{
public:
    reference_to_literal(address_t fixup_location, literal_name_t name)
        : fixup_location(fixup_location), name(name) {}

    const address_t fixup_location;
    const literal_name_t name;
};

template <typename T>
constexpr T check_immediate_range(T imm, T min, T max)
{
    if ((imm < min) || (imm > max))
    {
        imm = 0;
        report_error("Immediate value is out of range");
    }

    return imm;
}

template <typename T>
constexpr T check_immediate_is_aligned(T imm, T alignment)
{
    auto mask = get_byte_alignment(alignment) - 1;

    if (imm & mask)
    {
        imm = 0;
        report_error("Misaligned immediate value");
    }

    return imm;
}

}

#endif
