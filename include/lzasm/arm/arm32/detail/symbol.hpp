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
