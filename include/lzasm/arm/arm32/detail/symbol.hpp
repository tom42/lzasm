// SPDX-FileCopyrightText: 2021 Thomas Mathys
// SPDX-License-Identifier: MIT
// lzasm: a runtime assembler

#ifndef LZASM_ARM_ARM32_DETAIL_SYMBOL_HPP_INCLUDED
#define LZASM_ARM_ARM32_DETAIL_SYMBOL_HPP_INCLUDED

#include <string>
#include <utility>

namespace lzasm::arm::arm32
{

// General implementation of symbol.
template <typename TSymbolName>
class symbol final
{
public:
    // Do not allow implicit conversion from TSymbolName.
    // We do not want to allow the following, for interface symmetry:
    //
    //   basic_divided_thumb_assembler<int> a;
    //   a.label(42);           // UNWANTED: Implicit conversion from int, 42 is a symbol name
    //   a.add(r0, 42);         // This adds the value 42, not the value of symbol 42
    //   a.add(r0, symbol(42)); // This adds the value of symbol 42
    //
    // We want that users have to write
    //
    //   a.label(symbol(42));   // WANTED
    //
    explicit symbol(TSymbolName name) : name(std::move(name)) {}
    const TSymbolName name;
};

// Specialization of symbol for std::string.
template <>
class symbol<std::string> final
{
public:
    // Implicit conversion from std::string.
    symbol(std::string name) : name(std::move(name)) {}

    const std::string name;
};

template <typename TSymbolName>
bool operator < (const symbol<TSymbolName>& l, const symbol<TSymbolName>& r)
{
    return l.name < r.name;
}

template <typename TSymbolName>
bool operator == (const symbol<TSymbolName>& l, const symbol<TSymbolName>& r)
{
    return l.name == r.name;
}

static_assert(sizeof(symbol<int>) == sizeof(int));
static_assert(sizeof(symbol<std::string>) == sizeof(std::string));

}

#endif
