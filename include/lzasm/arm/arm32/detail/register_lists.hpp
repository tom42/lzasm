// SPDX-FileCopyrightText: 2021 Thomas Mathys
// SPDX-License-Identifier: MIT
// lzasm: a runtime assembler

#ifndef LZASM_ARM_ARM32_DETAIL_REGISTER_LISTS_HPP_INCLUDED
#define LZASM_ARM_ARM32_DETAIL_REGISTER_LISTS_HPP_INCLUDED

#include <bit>
#include <cassert>
#include "lzasm/arm/arm32/detail/registers.hpp"

namespace lzasm::arm::arm32
{

////////////////////////////////////////////////////////////////////////////////
// low_reg_list
////////////////////////////////////////////////////////////////////////////////

class low_reg_list final
{
public:
    explicit constexpr low_reg_list(const low_reg r) : low_reg_list(1 << r.n()) {}

    constexpr register_number_t n() const { return m_n; }

    constexpr bool contains(const low_reg r) const
    {
        return n() & low_reg_list(r).n();
    }

    constexpr bool is_lowest(const low_reg r) const
    {
        return std::countr_zero(static_cast<std::make_unsigned_t<register_number_t>>(n())) == r.n();
    }

private:
    explicit constexpr low_reg_list(register_number_t n) : m_n(n)
    {
        assert((n >= 1) && (n <= 255));
    }

    friend constexpr low_reg_list operator | (const low_reg_list lhs, const low_reg_list rhs);

    register_number_t m_n;
};

static_assert(sizeof(low_reg_list) == sizeof(register_number_t));

constexpr low_reg_list operator | (const low_reg_list lhs, const low_reg_list rhs)
{
    return low_reg_list(lhs.n() | rhs.n());
}

constexpr low_reg_list operator - (const low_reg lhs, const low_reg rhs)
{
    assert(lhs.n() < rhs.n());
    low_reg_list result(lhs);
    for (auto n = lhs.n() + 1; n <= rhs.n(); ++n)
    {
        result = result | low_reg_list(low_reg(n));
    }
    return result;
}

template <typename... Registers>
constexpr low_reg_list make_low_reg_list(const Registers... registers)
{
    return (low_reg_list(registers) | ...);
}


////////////////////////////////////////////////////////////////////////////////
// push_list
////////////////////////////////////////////////////////////////////////////////

class push_list final
{
public:
    explicit constexpr push_list(const low_reg r) : push_list(1 << r.n()) {}

    explicit constexpr push_list(const reg_lr) : push_list(1 << 8) {}

    explicit constexpr push_list(const low_reg_list list) : push_list(list.n()) {}

    constexpr register_number_t n() const { return m_n; }

    constexpr int size() const { return std::popcount(static_cast<std::make_unsigned_t<register_number_t>>(n())); }

private:
    explicit constexpr push_list(register_number_t n) : m_n(n)
    {
        assert((n >= 1) && (n <= 511));
    }

    friend constexpr push_list operator | (const push_list lhs, const push_list rhs);

    register_number_t m_n;
};

static_assert(sizeof(push_list) == sizeof(register_number_t));

constexpr push_list operator | (const push_list lhs, const push_list rhs)
{
    return push_list(lhs.n() | rhs.n());
}

template <typename... Registers>
constexpr push_list make_push_list(const Registers... registers)
{
    return (push_list(registers) | ...);
}


////////////////////////////////////////////////////////////////////////////////
// pop_list
////////////////////////////////////////////////////////////////////////////////

class pop_list final
{
public:
    explicit constexpr pop_list(const low_reg r) : pop_list(1 << r.n()) {}

    explicit constexpr pop_list(const reg_pc) : pop_list(1 << 8) {}

    explicit constexpr pop_list(const low_reg_list list) : pop_list(list.n()) {}

    constexpr register_number_t n() const { return m_n; }

private:
    explicit constexpr pop_list(register_number_t n) : m_n(n)
    {
        assert((n >= 1) && (n <= 511));
    }

    friend constexpr pop_list operator | (const pop_list lhs, const pop_list rhs);
    friend constexpr pop_list to_pop_list(const push_list list);

    register_number_t m_n;
};

static_assert(sizeof(pop_list) == sizeof(register_number_t));

constexpr pop_list operator | (const pop_list lhs, const pop_list rhs)
{
    return pop_list(lhs.n() | rhs.n());
}

constexpr pop_list to_pop_list(const push_list list)
{
    return pop_list(list.n());
}

template <typename... Registers>
constexpr pop_list make_pop_list(const Registers... registers)
{
    return (pop_list(registers) | ...);
}

}

#endif
