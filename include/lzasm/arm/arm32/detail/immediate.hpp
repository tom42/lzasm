// SPDX-FileCopyrightText: 2021 Thomas Mathys
// SPDX-License-Identifier: MIT
// lzasm: a runtime assembler

#ifndef LZASM_ARM_ARM32_DETAIL_IMMEDIATE_HPP_INCLUDED
#define LZASM_ARM_ARM32_DETAIL_IMMEDIATE_HPP_INCLUDED

#include <variant>
#include "lzasm/arm/arm32/detail/basic_types.hpp"
#include "lzasm/arm/arm32/detail/symbol.hpp"

namespace lzasm::arm::arm32::detail
{

template <typename TSymbolName>
class immediate_base
{
public:
    // Implicit conversion from constant value
    constexpr immediate_base(immediate_t value) : m_value(std::in_place_index<value_index>, value) {}

    // Implicit conversion from symbolic value
    constexpr immediate_base(symbol<TSymbolName> symbol) : m_value(std::in_place_index<symbol_index>, std::move(symbol)) {}

    constexpr bool is_symbol_reference() const { return m_value.index() == symbol_index; }

    constexpr immediate_t value() const { return std::get<value_index>(m_value); }

    constexpr const symbol<TSymbolName>& sym() const { return std::get<symbol_index>(m_value); }

    constexpr bool operator == (const immediate_base& rhs) const
    {
        return m_value == rhs.m_value;
    }

private:
    static constexpr int value_index = 0;
    static constexpr int symbol_index = 1;
    const std::variant<immediate_t, symbol<TSymbolName>> m_value;
};

// General implementation of immediate.
template <typename TSymbolName>
class immediate final : public immediate_base<TSymbolName>
{
public:
    using immediate_base<TSymbolName>::immediate_base;
};

// Specialization of immediate for std::string.
template <>
class immediate<std::string> final : public immediate_base<std::string>
{
public:
    using immediate_base<std::string>::immediate_base;

    // Implicit conversion from symbolic value of type std::string.
    // We intentionally do not have an implicit conversion from const char*.
    // Together with the implicit conversion from immediate_t, such a conversion
    // would be ambiguous when the integer literal 0 should be converted,
    // because 0 could be an integer or a null pointer.
    immediate(std::string symbol_name) : immediate_base(symbol<std::string>(symbol_name)) {}
};

}

#endif
