// SPDX-FileCopyrightText: 2021 Thomas Mathys
// SPDX-License-Identifier: MIT
// lzasm: a runtime assembler

#ifndef LZASM_ARM_ARM32_DETAIL_OPERATIONS_HPP_INCLUDED
#define LZASM_ARM_ARM32_DETAIL_OPERATIONS_HPP_INCLUDED

#include <cassert>

namespace lzasm::arm::arm32::detail
{

enum class condition_code
{
    eq = 0b0000,
    ne = 0b0001,
    cs = 0b0010,
    cc = 0b0011,
    mi = 0b0100,
    pl = 0b0101,
    vs = 0b0110,
    vc = 0b0111,
    hi = 0b1000,
    ls = 0b1001,
    ge = 0b1010,
    lt = 0b1011,
    gt = 0b1100,
    le = 0b1101
};

enum class add_sub_operation
{
    add = 0,
    sub = 1
};

enum class alu_operation
{
    and_ = 0b0000,
    eor = 0b0001,
    lsl = 0b0010,
    lsr = 0b0011,
    asr = 0b0100,
    adc = 0b0101,
    sbc = 0b0110,
    ror = 0b0111,
    tst = 0b1000,
    neg = 0b1001,
    cmp = 0b1010,
    cmn = 0b1011,
    orr = 0b1100,
    mul = 0b1101,
    bic = 0b1110,
    mvn = 0b1111
};

enum class high_register_operation
{
    add = 0b00,
    cmp = 0b01,
    mov = 0b10
};

enum class imm8_operation
{
    mov = 0b00,
    cmp = 0b01,
    add = 0b10,
    sub = 0b11
};

enum class ldmia_stmia_operation
{
    ldmia = 0b11001,
    stmia = 0b11000
};

enum class push_pop_operation
{
    pop = 0b1011110,
    push = 0b1011010
};

enum class shift_operation
{
    lsr = 0b01,
    asr = 0b10
};

constexpr add_sub_operation invert_operation(add_sub_operation operation)
{
    assert((operation == add_sub_operation::add) || (operation == add_sub_operation::sub));
    return operation == add_sub_operation::add ? add_sub_operation::sub : add_sub_operation::add;
}

constexpr imm8_operation invert_operation(imm8_operation operation)
{
    assert((operation == imm8_operation::add) || (operation == imm8_operation::sub));
    return operation == imm8_operation::add ? imm8_operation::sub : imm8_operation::add;
}

}

#endif
