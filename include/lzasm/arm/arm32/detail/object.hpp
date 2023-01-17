// SPDX-FileCopyrightText: 2021 Thomas Mathys
// SPDX-License-Identifier: MIT
// lzasm: a runtime assembler

#ifndef LZASM_ARM_ARM32_DETAIL_OBJECT_HPP_INCLUDED
#define LZASM_ARM_ARM32_DETAIL_OBJECT_HPP_INCLUDED

#include <algorithm>
#include <cstdint>
#include <map>
#include <vector>
#include "lzasm/arm/arm32/detail/basic_types.hpp"
#include "lzasm/arm/arm32/detail/immediate.hpp"
#include "lzasm/arm/arm32/detail/literal.hpp"
#include "lzasm/arm/arm32/detail/reference.hpp"
#include "lzasm/arm/arm32/detail/symbol.hpp"
#include "lzasm/arm/arm32/detail/utilities.hpp"

namespace lzasm::arm::arm32::detail
{

// Object for little endian ARM CPUs
template <typename TSymbolName>
class object final
{
public:
    void add_reference(reference_type type, const immediate<TSymbolName>& value)
    {
        references.emplace_back(type, current_lc(), value);
    }

    void add_reference_to_literal(const immediate<TSymbolName>& imm)
    {
        auto literal_name = get_name_of_new_or_existing_literal(imm);
        literal_references.emplace_back(current_lc(), literal_name);
    }

    void add_symbol(const symbol<TSymbolName>& symbol)
    {
        auto insertion_result = symbols.insert(std::make_pair(symbol, current_lc()));
        if (!insertion_result.second)
        {
            report_error("Symbol is already defined");
        }
    }

    void align(address_t alignment)
    {
        check_alignment_is_in_range(alignment);

        auto byte_alignment = get_byte_alignment(alignment);
        while (current_lc() % byte_alignment != 0)
        {
            emit8(0);
        }
    }

    address_t current_lc() const
    {
        return static_cast<address_t>(data.size());
    }

    void emit8(uint_fast8_t u8)
    {
        data.push_back(u8 & 0xff);
    }

    void emit16(uint_fast16_t u16)
    {
        emit8(u16 & 0xff);
        emit8((u16 >> 8) & 0xff);
    }

    void emit32(uint_fast32_t u32)
    {
        emit16(u32 & 0xffff);
        emit16((u32 >> 16) & 0xffff);
    }

    uint_fast8_t peek8(address_t address)
    {
        return data[address];
    }

    uint_fast16_t peek16(address_t address)
    {
        return data[address + 0] + (data[address + 1] << 8);
    }

    void poke8(address_t address, uint_fast8_t u8)
    {
        data[address] = u8;
    }

    void poke16(address_t address, uint_fast16_t u16)
    {
        data[address + 0] = u16 & 255;
        data[address + 1] = (u16 >> 8) & 255;
    }

    void poke32(address_t address, uint_fast32_t u32)
    {
        data[address + 0] = u32 & 255;
        data[address + 1] = (u32 >> 8) & 255;
        data[address + 2] = (u32 >> 16) & 255;
        data[address + 3] = (u32 >> 24) & 255;
    }

    void emit_literal_pool()
    {
        if (literals.empty())
        {
            return;
        }

        align(2);

        // Dump the literals into the pool, and record their addresses.
        for (auto& literal : literals)
        {
            literal.address = current_lc();

            if (literal.value.is_symbol_reference())
            {
                // The literal is a symbol, e.g. ldr r0,=somesymbol
                // Add a reference, so that it gets resolved.
                add_reference(reference_type::abs32, literal.value);
                emit32(dummy_value);
            }
            else
            {
                // The literal is a constant, e.g. ldr r0,=123
                // Write its value directly into the pool.
                // This is an optimization, we could as well generate a reference.
                emit32(literal.value.value());
            }
        }

        // Fix all the references into the literal pool.
        // That is, make all the "ldr,=" pseudo instructions encountered
        // since the last pool point into the pool we've just emitted.
        for (const auto& reference : literal_references)
        {
            fix_reference_to_literal(reference);
        }

        literals.clear();
        literal_references.clear();
    }

    void link(address_t origin)
    {
        check_origin(origin);
        emit_literal_pool();
        for (const auto& ref : references)
        {
            fix_address(ref, origin);
        }
    }

    bytevector to_bytevector() const { return data; }

private:
    auto get_name_of_new_or_existing_literal(const immediate<TSymbolName>& imm)
    {
        auto iter = std::find_if(literals.begin(), literals.end(), [&](const auto& literal) { return literal.value == imm; });
        if (iter == literals.end())
        {
            literals.emplace_back(imm);
            iter = literals.end() - 1;
        }

        // The index of the literal in the literals buffer is the literal's name by
        // which it can be referenced. The literal itself does not store its name.
        return iter - literals.begin();
    }

    void fix_address(const reference<TSymbolName>& ref, address_t origin)
    {
        switch (ref.type)
        {
            case reference_type::abs5_asr_lsr:
                return fix_abs5_asr_lsr(ref, origin);
            case reference_type::abs3:
            case reference_type::abs5:
            case reference_type::abs6:
            case reference_type::abs7:
            case reference_type::abs8_add_sub:
            case reference_type::abs8_unsigned:
            case reference_type::abs9_add_sub_sp:
            case reference_type::abs10:
            case reference_type::abs16:
                return fix_abs_generic(ref, origin);
            case reference_type::abs8_byte:
                return fix_abs8_byte(ref, origin);
            case reference_type::abs32:
                return fix_abs32(ref, origin);
            case reference_type::adr:
                return fix_adr(ref, origin);
            case reference_type::arm_branch:
                return fix_arm_branch(ref, origin);
            case reference_type::bl:
                return fix_bl(ref, origin);
            case reference_type::conditional_branch:
                return fix_conditional_branch(ref, origin);
            case reference_type::unconditional_branch:
                return fix_unconditional_branch(ref, origin);
            default:
                return report_error("Internal error: invalid reference type");
        }
    }

    void fix_abs5_asr_lsr(const reference<TSymbolName>& ref, address_t origin)
    {
        if (get_value(ref.value, origin) == 0)
        {
            // If the shift count is 0, map ASR/LSR to LSL.
            // Do so by clearing the most significant five bits of the opcode.
            auto opcode = peek16(ref.fixup_location);
            opcode &= 0b0000011111111111;
            poke16(ref.fixup_location, opcode);
        }

        return fix_abs_generic(ref, origin);
    }

    void fix_abs_generic(const reference<TSymbolName>& ref, address_t origin)
    {
        const auto& d = reference_type_descriptors::get(ref.type);

        auto immediate_bits = get_absolute_immediate_bits(ref, origin);

        auto opcode = peek16(ref.fixup_location);

        // The assembler should have left the bits for the immediate value all zero.
        assert((opcode & (d.bit_mask << d.bit_pos)) == 0);

        opcode |= immediate_bits << d.bit_pos;
        poke16(ref.fixup_location, opcode);
    }

    void fix_abs8_byte(const reference<TSymbolName>& ref, address_t origin)
    {
        // Note:
        // * The byte directive cannot be handled using fix_abs_generic, because fix_abs_generic uses peek16 and poke16.
        //   This will read/write past the end of the program if a byte directive is at the very end of the program,
        //   that's why fix_abs8_byte exists. It uses poke8 and thus never reads/writes past the end of the program.
        // * fix_abs8_byte could be used to process other reference types where the bit width is <= 8 and where
        //   the immediate does not span more than one byte.
        //   To do so it would have to be changed to use peek8 *and* poke8, though.
        auto immediate_bits = get_absolute_immediate_bits(ref, origin);
        poke8(ref.fixup_location, immediate_bits & 255);
    }

    void fix_abs32(const reference<TSymbolName>& ref, address_t origin)
    {
        auto immediate_bits = get_absolute_immediate_bits(ref, origin);
        poke32(ref.fixup_location, immediate_bits);
    }

    void fix_adr(const reference<TSymbolName>& ref, address_t origin)
    {
        auto immediate_bits = get_relative_immediate_bits(ref, origin);
        poke8(ref.fixup_location, immediate_bits & 255);
    }

    void fix_arm_branch(const reference<TSymbolName>& ref, address_t origin)
    {
        auto immediate_bits = get_relative_immediate_bits(ref, origin);
        poke32(ref.fixup_location, 0xea000000 | (immediate_bits & 0x00ffffff));
    }

    void fix_bl(const reference<TSymbolName>& ref, address_t origin)
    {
        auto immediate_bits = get_relative_immediate_bits(ref, origin);

        // bl is encoded as an instruction pair, where each pair contains 11 bits of the address.
        // The address itself is 23 bits wide, but the LSB is implicitly zero, so only 22 bits are encoded.
        auto upper_address_bits = (immediate_bits >> 11) & 2047;
        auto lower_address_bits = immediate_bits & 2047;
        poke16(ref.fixup_location + 0, 0xf000 | upper_address_bits);
        poke16(ref.fixup_location + 2, 0xf800 | lower_address_bits);
    }

    void fix_conditional_branch(const reference<TSymbolName>& ref, address_t origin)
    {
        auto immediate_bits = get_relative_immediate_bits(ref, origin);
        poke8(ref.fixup_location, immediate_bits & 255);
    }

    void fix_unconditional_branch(const reference<TSymbolName>& ref, address_t origin)
    {
        auto immediate_bits = get_relative_immediate_bits(ref, origin);
        poke16(ref.fixup_location, (0b11100 << 11) | (immediate_bits & 2047));
    }

    void fix_reference_to_literal(const reference_to_literal& ref)
    {
        const auto& d = reference_type_descriptors::get(reference_type::literal);

        auto target = literals[ref.name].address;
        auto relative_address = get_relative_address(target, ref.fixup_location, 0, d);
        auto immediate_bits = get_immediate_bits(relative_address, d);

        poke8(ref.fixup_location, immediate_bits & 255);
    }

    immediate_t get_absolute_immediate_bits(const reference<TSymbolName>& ref, address_t origin)
    {
        auto immediate_value = get_value(ref.value, origin);

        // During linking we only support symbol references.
        // The only type of symbols are labels, which are memory locations and thus
        // cannot be negative. Therefore, unlike the assembler, we do not perform
        // special handling of negative values for add/sub instructions and require
        // that the resolved immediate value is >= 0.
        assert(ref.value.is_symbol_reference());
        assert(immediate_value >= 0);

        const auto& d = reference_type_descriptors::get(ref.type);
        return get_immediate_bits(immediate_value, d);
    }

    auto get_relative_immediate_bits(const reference<TSymbolName>& ref, address_t origin)
    {
        const auto& d = reference_type_descriptors::get(ref.type);
        auto target = get_value(ref.value, origin);
        auto relative_address = get_relative_address(target, ref.fixup_location, origin, d);
        return get_immediate_bits(relative_address, d);
    }

    template <typename T>
    auto get_relative_address(T target, address_t fixup_location, address_t origin, const reference_type_descriptor& d)
    {
        auto source = origin + fixup_location + d.pc_offset();

        if (d.require_pc_bit1_cleared())
        {
            source = clear_bit1(source);
        }

        return target - source;
    }

    immediate_t get_immediate_bits(immediate_t imm, const reference_type_descriptor& d)
    {
        imm = check_immediate_range(imm, d.min, d.max);
        imm = check_immediate_is_aligned(imm, d.alignment);
        return discard_implicitly_zero_bits(imm, d);
    }

    immediate_t discard_implicitly_zero_bits(immediate_t imm, const reference_type_descriptor& d)
    {
        // The bitwise and is needed to special handle asr/lsr. It maps shift count 32 to 0.
        return (imm >> d.alignment) & d.bit_mask;
    }

    immediate_t get_value(const immediate<TSymbolName>& imm, address_t origin)
    {
        if (imm.is_symbol_reference())
        {
            auto symbol_table_entry = symbols.find(imm.sym());
            if (symbol_table_entry == symbols.end())
            {
                report_error("Undefined symbol");
            }

            return symbol_table_entry->second + origin;
        }

        return imm.value();
    }

    void check_origin(address_t origin)
    {
        if (max_address - current_lc() < origin)
        {
            report_error("Origin too large");
        }
    }

    static void check_alignment_is_in_range(address_t alignment)
    {
        if (alignment > max_alignment)
        {
            report_error("Alignment out of range");
        }
    }

    static constexpr auto dummy_value = 0;
    bytevector data;
    std::map<symbol<TSymbolName>, address_t> symbols;
    std::vector<reference<TSymbolName>> references;
    std::vector<detail::literal<TSymbolName>> literals;
    std::vector<reference_to_literal> literal_references;
};

}

#endif
