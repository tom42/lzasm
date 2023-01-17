// SPDX-FileCopyrightText: 2021 Thomas Mathys
// SPDX-License-Identifier: MIT
// lzasm: a runtime assembler

#ifndef LZASM_ARM_ARM32_DETAIL_REGISTERS_HPP_INCLUDED
#define LZASM_ARM_ARM32_DETAIL_REGISTERS_HPP_INCLUDED

#include <cassert>

namespace lzasm::arm::arm32
{

using register_number_t = int;

// Base class for registers.
// Registers are upcast by value, so derived classes should not have any fields.
// The different register classes are only there to restrict what kind of registers
// can be passed to mnemonics.
class reg
{
public:
    explicit constexpr reg(register_number_t n)
        : m_n(n)
    {
        assert((n >= 0) && (n <= 15));
    }

    constexpr register_number_t n() const { return m_n; }

    constexpr bool is_low() const { return m_n < 8; }

    constexpr register_number_t high_bit() const { return (m_n >> 3) & 1; }

    constexpr register_number_t low_bits() const { return m_n & 7; }

private:
    const register_number_t m_n;
};

class low_reg final : public reg
{
public:
    explicit constexpr low_reg(register_number_t n)
        : reg(n)
    {
        assert((n >= 0) && (n <= 7));
    }
};

class reg_sp final : public reg
{
public:
    explicit constexpr reg_sp() : reg(13) {}
};

class reg_lr final : public reg
{
public:
    explicit constexpr reg_lr() : reg(14) {}
};

class reg_pc final : public reg
{
public:
    explicit constexpr reg_pc() : reg(15) {}
};

class writeback_low_reg final
{
public:
    explicit constexpr writeback_low_reg(const low_reg r) : m_r(r) {}

    constexpr register_number_t n() const { return m_r.n(); }

private:
    const low_reg m_r;
};

static_assert(sizeof(reg) == sizeof(register_number_t));
static_assert(sizeof(low_reg) == sizeof(register_number_t));
static_assert(sizeof(reg_sp) == sizeof(register_number_t));
static_assert(sizeof(reg_lr) == sizeof(register_number_t));
static_assert(sizeof(reg_pc) == sizeof(register_number_t));
static_assert(sizeof(writeback_low_reg) == sizeof(register_number_t));

constexpr writeback_low_reg operator ! (const low_reg r)
{
    return writeback_low_reg(r);
}

constexpr low_reg r0(0);
constexpr low_reg r1(1);
constexpr low_reg r2(2);
constexpr low_reg r3(3);
constexpr low_reg r4(4);
constexpr low_reg r5(5);
constexpr low_reg r6(6);
constexpr low_reg r7(7);
constexpr reg     r8(8);
constexpr reg     r9(9);
constexpr reg     r10(10);
constexpr reg     r11(11);
constexpr reg     r12(12);
constexpr reg_sp  r13;
constexpr reg_lr  r14;
constexpr reg_pc  r15;

// Register aliases
constexpr reg_sp  sp;
constexpr reg_lr  lr;
constexpr reg_pc  pc;

}

#endif
