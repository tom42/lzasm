// SPDX-FileCopyrightText: 2021 Thomas Mathys
// SPDX-License-Identifier: MIT
// lzasm: a runtime assembler

#ifndef LZASM_ARM_ARM32_DETAIL_LITERAL_HPP_INCLUDED
#define LZASM_ARM_ARM32_DETAIL_LITERAL_HPP_INCLUDED

#include <cstddef>
#include "lzasm/arm/arm32/detail/basic_types.hpp"
#include "lzasm/arm/arm32/detail/immediate.hpp"

namespace lzasm::arm::arm32::detail
{

using literal_name_t = size_t;

template <typename TSymbolName>
class literal final
{
public:
    explicit literal(const immediate<TSymbolName>& value)
        : value(value), address(0) {}

    const immediate<TSymbolName> value;
    address_t address;
};

}

#endif
