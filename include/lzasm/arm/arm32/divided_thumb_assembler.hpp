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

#ifndef LZASM_ARM_ARM32_DIVIDED_THUMB_ASSEMBLER_HPP_INCLUDED
#define LZASM_ARM_ARM32_DIVIDED_THUMB_ASSEMBLER_HPP_INCLUDED

#include <cassert>
#include <concepts>
#include <limits>
#include <string>
#include "lzasm/arm/arm32/detail/basic_types.hpp"
#include "lzasm/arm/arm32/detail/immediate.hpp"
#include "lzasm/arm/arm32/detail/object.hpp"
#include "lzasm/arm/arm32/detail/operations.hpp"
#include "lzasm/arm/arm32/detail/reference.hpp"
#include "lzasm/arm/arm32/detail/registers.hpp"
#include "lzasm/arm/arm32/detail/register_lists.hpp"
#include "lzasm/arm/arm32/detail/symbol.hpp"
#include "lzasm/arm/arm32/detail/utilities.hpp"

namespace lzasm::arm::arm32
{

template <typename TSymbolName>
class basic_divided_thumb_assembler
{
public:
    using immediate = detail::immediate<TSymbolName>;

    virtual ~basic_divided_thumb_assembler() = default;

    address_t current_lc() const
    {
        return obj.current_lc();
    }

    bytevector link(address_t origin)
    {
        obj.link(origin);
        return obj.to_bytevector();
    }

    ////////////////////////////////////////////////////////////////////////////
    // Miscellaneous directives
    ////////////////////////////////////////////////////////////////////////////

    basic_divided_thumb_assembler& adr(const low_reg rd, const immediate& imm10)
    {
        obj.add_reference(reference_type::adr, imm10);
        return add(rd, pc, 0);
    }

    basic_divided_thumb_assembler& align(address_t alignment)
    {
        obj.align(alignment);
        return *this;
    }

    basic_divided_thumb_assembler& label(const symbol<TSymbolName>& s)
    {
        obj.add_symbol(s);
        return *this;
    }

    basic_divided_thumb_assembler& pool()
    {
        obj.emit_literal_pool();
        return *this;
    }

    ////////////////////////////////////////////////////////////////////////////
    // Data definition directives
    ////////////////////////////////////////////////////////////////////////////

    basic_divided_thumb_assembler& byte(const immediate& imm8)
    {
        obj.emit8(to_abs(reference_type::abs8_byte, imm8) & 255);
        return *this;
    }

    template <typename... Bytes>
    basic_divided_thumb_assembler& byte(const immediate& imm8, const Bytes&... bytes)
    {
        byte(imm8);
        return byte(bytes...);
    }

    basic_divided_thumb_assembler& hword(const immediate& imm16)
    {
        obj.emit16(to_abs(reference_type::abs16, imm16));
        return *this;
    }

    template <typename... Hwords>
    basic_divided_thumb_assembler& hword(const immediate& imm16, const Hwords&... hwords)
    {
        hword(imm16);
        return hword(hwords...);
    }

    template <typename Iterator>
    basic_divided_thumb_assembler& incbin(Iterator iterator, Iterator end)
    {
        for (; iterator != end; ++iterator)
        {
            obj.emit8(*iterator);
        }
        return *this;
    }

    basic_divided_thumb_assembler& word(const immediate& imm32)
    {
        obj.emit32(to_abs(reference_type::abs32, imm32));
        return *this;
    }

    template <typename... Words>
    basic_divided_thumb_assembler& word(const immediate& imm32, const Words&... words)
    {
        word(imm32);
        return word(words...);
    }

    ////////////////////////////////////////////////////////////////////////////
    // ARM code generation pseudo instructions
    ////////////////////////////////////////////////////////////////////////////

    // Generates an unconditional ARM branch instruction, e.g. "b some_label".
    basic_divided_thumb_assembler& arm_branch(const immediate& imm)
    {
        obj.add_reference(reference_type::arm_branch, imm);
        obj.emit32(0);
        return *this;
    }

    // Generates the following two instructions to switch from ARM to Thumb state:
    //     add r, pc, #1
    //     bx r
    // thumb_start:
    //
    // r may be freely chosen.
    // After switching to Thumb state, r is equal to thumb_start + 1.
    basic_divided_thumb_assembler& arm_to_thumb(const reg r)
    {
        obj.emit32((0xe28fu << 16) | (r.n() << 12) | 0x001);
        obj.emit32((0xe12fff1u << 4) | r.n());
        return *this;
    }

    ////////////////////////////////////////////////////////////////////////////
    // Thumb instructions
    ////////////////////////////////////////////////////////////////////////////

    // ["adc", "Rx!=HI, Rx!=HI, Rm!=HI", "T16", "0100|000|101|Rm:3|Rx:3", "ARMv4T+ IT=IN"]
    basic_divided_thumb_assembler& adc(const low_reg rx, const low_reg rm) { return emit_alu_operation(alu_operation::adc, rx, rm); }

    // ["add", "Rx!=HI, Rx!=HI, #ImmZ", "T16", "0011|0|Rx:3|ImmZ:8", "ARMv4T+ IT=IN"]
    basic_divided_thumb_assembler& add(const low_reg rx, const immediate& imm8)
    {
        return emit_add_sub_imm8(imm8_operation::add, rx, imm8);
    }

    // ["add", "Rd!=HI, Rn!=HI, #ImmZ", "T16", "0001|110|ImmZ:3|Rn:3|Rd:3", "ARMv4T+ IT=IN"]
    basic_divided_thumb_assembler& add(const low_reg rd, const low_reg rn, const immediate& imm3)
    {
        return emit_add_sub_imm3(add_sub_operation::add, rd, rn, imm3);
    }

    // ["add", "Rx==SP, Rx==SP, #ImmZ*4", "T16", "1011|00000|ImmZ:7", "ARMv4T+ IT=ANY"]
    basic_divided_thumb_assembler& add(const reg_sp, const immediate& imm9)
    {
        return emit_add_sub_sp_imm9(add_sub_operation::add, imm9);
    }

    // ["add", "Rd!=SP, Rn==SP, #ImmZ*4", "T16", "1010|1|Rd:3|ImmZ:8", "ARMv4T+ IT=ANY"]
    basic_divided_thumb_assembler& add(const low_reg rd, const reg_sp, const immediate& imm10)
    {
        return emit_load_address(true, rd, imm10);
    }

    // ["add", "Rd!=HI, Rn!=HI, Rm!=HI", "T16", "0001|100|Rm:3|Rn:3|Rd:3", "ARMv4T+ IT=IN"]
    basic_divided_thumb_assembler& add(const low_reg rd, const low_reg rn, const low_reg rm)
    {
        return emit_add_sub_register(add_sub_operation::add, rd, rn, rm);
    }

    // ["add", "Rx!=XX, Rx!=XX, Rm!=XX", "T16", "0100|010|0|Rx:1|Rm:4|Rx:3", "ARMv4T+ IT=IN  ARMv6T2_IF_LOW"]
    // ARM Architecture Reference Manual (DDI 0100E): [both operands can] "be any of R0 to R15"
    basic_divided_thumb_assembler& add(const reg rx, const reg rm)
    {
        if (are_all_low(rx, rm))
        {
            // Up to but not including ARMv6T2, behavior is unpredictable if both operands are low registers.
            // Map to different instruction.
            return emit_add_sub_register(add_sub_operation::add, rx, rx, rm);
        }
        else
        {
            return emit_high_register_operation(high_register_operation::add, rx, rm);
        }
    }

    // ["adr", "Rd!=HI, #RelZ*4", "T16", "1010|0|Rd:3|RelZ:8", "ARMv4T+ IT=ANY ADD=1"]
    // Note: asmdb lists this instruction as adr, but adr is a pseudo instruction which generates this add instruction.
    basic_divided_thumb_assembler& add(const low_reg rd, const reg_pc, const immediate& imm10)
    {
        return emit_load_address(false, rd, imm10);
    }

    // ["and", "Rx!=HI, Rx!=HI, Rm!=HI", "T16", "0100|000|000|Rm:3|Rx:3", "ARMv4T+ IT=IN"]
    basic_divided_thumb_assembler& and_(const low_reg rx, const low_reg rm) { return emit_alu_operation(alu_operation::and_, rx, rm); }

    // ["asr", "Rd!=HI, Rn!=HI, #Shift", "T16", "0001|0|Shift:5|Rn:3|Rd:3", "ARMv4T+ IT=IN"]
    basic_divided_thumb_assembler& asr(const low_reg rd, const low_reg rn, const immediate& imm5)
    {
        return emit_asr_lsr_imm5(shift_operation::asr, rd, rn, imm5);
    }

    // ["asr", "Rx!=HI, Rx!=HI, Rm!=HI", "T16", "0100|000|100|Rm:3|Rx:3", "ARMv4T+ IT=IN"]
    basic_divided_thumb_assembler& asr(const low_reg rx, const low_reg rm) { return emit_alu_operation(alu_operation::asr, rx, rm); }

    // ["b", "#RelS*2", "T16", "1101|Cond|RelS:8", "ARMv4T+ IT=OUT"]
    basic_divided_thumb_assembler& beq(const immediate& imm9) { return emit_conditional_branch(condition_code::eq, imm9); }
    basic_divided_thumb_assembler& bne(const immediate& imm9) { return emit_conditional_branch(condition_code::ne, imm9); }
    basic_divided_thumb_assembler& bcs(const immediate& imm9) { return emit_conditional_branch(condition_code::cs, imm9); }
    basic_divided_thumb_assembler& bcc(const immediate& imm9) { return emit_conditional_branch(condition_code::cc, imm9); }
    basic_divided_thumb_assembler& bmi(const immediate& imm9) { return emit_conditional_branch(condition_code::mi, imm9); }
    basic_divided_thumb_assembler& bpl(const immediate& imm9) { return emit_conditional_branch(condition_code::pl, imm9); }
    basic_divided_thumb_assembler& bvs(const immediate& imm9) { return emit_conditional_branch(condition_code::vs, imm9); }
    basic_divided_thumb_assembler& bvc(const immediate& imm9) { return emit_conditional_branch(condition_code::vc, imm9); }
    basic_divided_thumb_assembler& bhi(const immediate& imm9) { return emit_conditional_branch(condition_code::hi, imm9); }
    basic_divided_thumb_assembler& bls(const immediate& imm9) { return emit_conditional_branch(condition_code::ls, imm9); }
    basic_divided_thumb_assembler& bge(const immediate& imm9) { return emit_conditional_branch(condition_code::ge, imm9); }
    basic_divided_thumb_assembler& blt(const immediate& imm9) { return emit_conditional_branch(condition_code::lt, imm9); }
    basic_divided_thumb_assembler& bgt(const immediate& imm9) { return emit_conditional_branch(condition_code::gt, imm9); }
    basic_divided_thumb_assembler& ble(const immediate& imm9) { return emit_conditional_branch(condition_code::le, imm9); }

    // ["b", "#RelS*2", "T16", "1110|0|RelS:11", "ARMv4T+ IT=OUT|LAST"]
    basic_divided_thumb_assembler& b(const immediate& imm12)
    {
        obj.add_reference(reference_type::unconditional_branch, imm12);
        obj.emit16(0);
        return *this;
    }

    // ["bic", "Rx!=HI, Rx!=HI, Rm!=HI", "T16", "0100|001|110|Rm:3|Rx:3", "ARMv4T+ IT=IN"]
    basic_divided_thumb_assembler& bic(const low_reg rx, const low_reg rm) { return emit_alu_operation(alu_operation::bic, rx, rm); }

    // ["bl", "#RelS*2", "T32", "1111|0|RelS[23]|RelS[20:11]|11|Ja|1|Jb|RelS[10:0]", "ARMv4T+ IT=OUT|LAST"]
    basic_divided_thumb_assembler& bl(const immediate& imm23)
    {
        obj.add_reference(reference_type::bl, imm23);
        obj.emit32(0);
        return *this;
    }

    // ["bx", "Rm", "T16", "0100|011|10|Rm:4|000", "ARMv4T+ IT=OUT|LAST"]
    basic_divided_thumb_assembler& bx(const reg rm)
    {
        obj.emit16((0b010001110 << 7) | (rm.n() << 3));
        return *this;
    }

    // ["cmn", "Rn!=HI, Rm!=HI", "T16", "0100|001|011|Rm:3|Rn:3", "ARMv4T+ IT=ANY APSR.NZCV=W"]
    basic_divided_thumb_assembler& cmn(const low_reg rx, const low_reg rm) { return emit_alu_operation(alu_operation::cmn, rx, rm); }

    // ["cmp", "Rn!=HI, #ImmZ", "T16", "0010|1|Rn:3|ImmZ:8", "ARMv4T+ IT=ANY APSR.NZCV=W"]
    basic_divided_thumb_assembler& cmp(const low_reg rd, const immediate& imm8)
    {
        return emit_cmp_mov_imm8(imm8_operation::cmp, rd, imm8);
    }

    // ["cmp", "Rn!=HI, Rm!=HI", "T16", "0100|001|010|Rm:3|Rn:3", "ARMv4T+ IT=ANY APSR.NZCV=W"]
    basic_divided_thumb_assembler& cmp(const low_reg rx, const low_reg rm) { return emit_alu_operation(alu_operation::cmp, rx, rm); }

    // ["cmp", "Rn!=PC, Rm!=PC", "T16", "0100|010|1|Rn:1|Rm:4|Rn:3", "ARMv4T+ IT=ANY APSR.NZCV=W UNPRED_IF_ALL_LOW"]
    // ARM Architecture Reference Manual (DDI 0100E): [both operands can] "be any of R0 to R15"
    basic_divided_thumb_assembler& cmp(const reg rx, const reg rm)
    {
        if (are_all_low(rx, rm))
        {
            // Up to but not including ARMv6T2, behavior is unpredictable if both operands are low registers.
            // Map to different instruction.
            return emit_alu_operation(alu_operation::cmp, rx, rm);
        }
        else
        {
            return emit_high_register_operation(high_register_operation::cmp, rx, rm);
        }
    }

    // ["eor", "Rx!=HI, Rx!=HI, Rm!=HI", "T16", "0100|000|001|Rm:3|Rx:3", "ARMv4T+ IT=IN"]
    basic_divided_thumb_assembler& eor(const low_reg rx, const low_reg rm) { return emit_alu_operation(alu_operation::eor, rx, rm); }

    // ["ldm", "[Rn!=HI]{!}, RdList", "T16", "1100|1|Rn:3|RdList:8", "ARMv4T+ IT=ANY T16_LDM"]
    template <typename R, typename... Rs>
    basic_divided_thumb_assembler& ldmia(const writeback_low_reg rn, const R r, const Rs... rs)
    {
        return ldmia(rn, make_low_reg_list(r, rs...));
    }

    basic_divided_thumb_assembler& ldmia(const writeback_low_reg rn, const low_reg_list list)
    {
        return emit_ldmia_stmia(ldmia_stmia_operation::ldmia, rn, list);
    }

    // ["ldr", "Rd!=HI, [Rn!=HI, #ImmZ*4]", "T16", "0110|1|ImmZ:5|Rn:3|Rd:3", "ARMv4T+ IT=ANY"]
    basic_divided_thumb_assembler& ldr(const low_reg rd, const low_reg rn, const immediate& imm7)
    {
        return emit_load_store_word(true, rd, rn, imm7);
    }

    // ["ldr", "Rd!=HI, [Rn==PC, #ImmZ*4]", "T16", "0100|1|Rd:3|ImmZ:8", "ARMv6T2+ IT=ANY"]
    // Note: asmdb says this instruction is available in ARMv6T2+, but it should be ARMv4T+.
    basic_divided_thumb_assembler& ldr(const low_reg rd, const reg_pc, const immediate& imm10)
    {
        auto imm = to_abs(reference_type::abs10, imm10);
        obj.emit16((0b01001 << 11) | (rd.n() << 8) | (imm / 4));
        return *this;
    }

    // ["ldr", "Rd!=HI, [Rn==SP, #ImmZ*4]", "T16", "1001|1|Rd:3|ImmZ:8", "ARMv4T+ IT=ANY"]
    basic_divided_thumb_assembler& ldr(const low_reg rd, const reg_sp, const immediate& imm10)
    {
        return emit_sp_relative_load_store(true, rd, imm10);
    }

    // ["ldr", "Rd!=HI, [Rn!=HI, Rm!=HI]", "T16", "0101|100|Rm:3|Rn:3|Rd:3", "ARMv4T+ IT=ANY"]
    basic_divided_thumb_assembler& ldr(const low_reg rd, const low_reg rn, const low_reg rm)
    {
        return emit_load_store_with_register_offset(true, false, rd, rn, rm);
    }

    basic_divided_thumb_assembler& ldr(const low_reg rd, const immediate& imm)
    {
        // Always assemble an ldr instruction, even if the literal fits into a mov instruction.
        // Unlike in ARM state, in Thumb state mov sets the flags, so we cannot replace ldr by mov.
        obj.add_reference_to_literal(imm);
        return ldr(rd, pc, dummy_value);
    }

    // ["ldrb", "Rd!=HI, [Rn!=HI, #ImmZ*4]", "T16", "0111|1|ImmZ:5|Rn:3|Rd:3", "ARMv4T+ IT=ANY"]
    basic_divided_thumb_assembler& ldrb(const low_reg rd, const low_reg rn, const immediate& imm5)
    {
        return emit_load_store_byte(true, rd, rn, imm5);
    }

    // ["ldrb", "Rd!=HI, [Rn!=HI, Rm!=HI]", "T16", "0101|110|Rm:3|Rn:3|Rd:3", "ARMv4T+ IT=ANY"]
    basic_divided_thumb_assembler& ldrb(const low_reg rd, const low_reg rn, const low_reg rm)
    {
        return emit_load_store_with_register_offset(true, true, rd, rn, rm);
    }

    // ["ldrh", "Rd!=HI, [Rn!=HI, #ImmZ*4]", "T16", "1000|1|ImmZ:5|Rn:3|Rd:3", "ARMv4T+ IT=ANY"]
    basic_divided_thumb_assembler& ldrh(const low_reg rd, const low_reg rn, const immediate& imm6)
    {
        return emit_load_store_halfword(true, rd, rn, imm6);
    }

    // ["ldrh", "Rd!=HI, [Rn!=HI, Rm!=HI]", "T16", "0101|101|Rm:3|Rn:3|Rd:3", "ARMv4T+ IT=ANY"]
    basic_divided_thumb_assembler& ldrh(const low_reg rd, const low_reg rn, const low_reg rm)
    {
        return emit_load_store_sign_extended(true, false, rd, rn, rm);
    }

    // ["ldrsb", "Rd!=HI, [Rn!=HI, Rm!=HI]", "T16", "0101|011|Rm:3|Rn:3|Rd:3", "ARMv4T+ IT=ANY"]
    basic_divided_thumb_assembler& ldrsb(const low_reg rd, const low_reg rn, const low_reg rm)
    {
        return emit_load_store_sign_extended(false, true, rd, rn, rm);
    }

    // ["ldrsh", "Rd!=HI, [Rn!=HI, Rm!=HI]", "T16", "0101|111|Rm:3|Rn:3|Rd:3", "ARMv4T+ IT=ANY"]
    basic_divided_thumb_assembler& ldrsh(const low_reg rd, const low_reg rn, const low_reg rm)
    {
        return emit_load_store_sign_extended(true, true, rd, rn, rm);
    }

    // ["lsl", "Rd!=HI, Rn!=HI, #Shift", "T16", "0000|0|Shift:5|Rn:3|Rd:3", "ARMv4T+ IT=IN"]
    basic_divided_thumb_assembler& lsl(const low_reg rd, const low_reg rn, const immediate& imm5)
    {
        auto imm = to_abs(reference_type::abs5, imm5);
        obj.emit16((0b00000 << 11) | (imm << 6) | (rn.n() << 3) | rd.n());
        return *this;
    }

    // ["lsl", "Rx!=HI, Rx!=HI, Rm!=HI", "T16", "0100|000|010|Rm:3|Rx:3", "ARMv4T+ IT=IN"]
    basic_divided_thumb_assembler& lsl(const low_reg rx, const low_reg rm) { return emit_alu_operation(alu_operation::lsl, rx, rm); }

    // ["lsr", "Rd!=HI, Rn!=HI, #Shift", "T16", "0000|1|Shift:5|Rn:3|Rd:3", "ARMv4T+ IT=IN"]
    basic_divided_thumb_assembler& lsr(const low_reg rd, const low_reg rn, const immediate& imm5)
    {
        return emit_asr_lsr_imm5(shift_operation::lsr, rd, rn, imm5);
    }

    // ["lsr", "Rx!=HI, Rx!=HI, Rm!=HI", "T16", "0100|000|011|Rm:3|Rx:3", "ARMv4T+ IT=IN"]
    basic_divided_thumb_assembler& lsr(const low_reg rx, const low_reg rm) { return emit_alu_operation(alu_operation::lsr, rx, rm); }

    // ["mov", "Rd!=HI, #ImmZ", "T16", "0010|0|Rd:3|ImmZ:8", "ARMv4T+ IT=IN"]
    basic_divided_thumb_assembler& mov(const low_reg rd, const immediate& imm8)
    {
        return emit_cmp_mov_imm8(imm8_operation::mov, rd, imm8);
    }

    // ["mov", "Rd    , Rn", "T16", "0100|0110|Rd:1|Rn:4|Rd:3", "ARMv4T+ IT=IN  ARMv6T2_IF_LOW"]
    basic_divided_thumb_assembler& mov(const reg rx, const reg rm)
    {
        if (are_all_low(rx, rm))
        {
            // Up to but not including ARMv6T2, behavior is unpredictable if both operands are low registers.
            // Map to different instruction.
            return emit_add_sub_imm3(add_sub_operation::add, rx, rm, 0);
        }
        else
        {
            return emit_high_register_operation(high_register_operation::mov, rx, rm);
        }
    }

    // ["mul", "Rx!=HI, Rx!=HI, Rm!=HI", "T16", "0100|001|101|Rm:3|Rx:3", "ARMv4T+ IT=IN"]
    basic_divided_thumb_assembler& mul(const low_reg rx, const low_reg rm) { return emit_alu_operation(alu_operation::mul, rx, rm); }

    // ["mvn", "Rd!=HI, Rn!=HI", "T16", "0100|001|111|Rn:3|Rd:3", "ARMv4T+ IT=IN"]
    basic_divided_thumb_assembler& mvn(const low_reg rx, const low_reg rm) { return emit_alu_operation(alu_operation::mvn, rx, rm); }

    // ["rsb", "Rd!=HI, Rn!=HI, #0", "T16", "0100|001001|Rn:3|Rd:3", "ARMv4T+ IT=IN"]
    basic_divided_thumb_assembler& neg(const low_reg rx, const low_reg rm) { return emit_alu_operation(alu_operation::neg, rx, rm); }

    basic_divided_thumb_assembler& nop() { return mov(r8, r8); }

    // ["orr", "Rx!=HI, Rx!=HI, Rm!=HI", "T16", "0100|001|100|Rm:3|Rx:3", "ARMv4T+ IT=IN"]
    basic_divided_thumb_assembler& orr(const low_reg rx, const low_reg rm) { return emit_alu_operation(alu_operation::orr, rx, rm); }

    // ["pop", "RdList", "T16", "1011|110|RdList[15]|RdList[7:0]", "ARMv4T+ IT=ANY"]
    template <typename R, typename... Rs>
    basic_divided_thumb_assembler& pop(const R r, const Rs... rs)
    {
        return pop(make_pop_list(r, rs...));
    }

    basic_divided_thumb_assembler& pop(const pop_list list)
    {
        return emit_push_pop(push_pop_operation::pop, list);
    }

    // ["push", "RsList", "T16", "1011|010|RsList[14]|RsList[7:0]", "ARMv4T+ IT=ANY"]
    template <typename R, typename... Rs>
    basic_divided_thumb_assembler& push(const R r, const Rs... rs)
    {
        return push(make_push_list(r, rs...));
    }

    basic_divided_thumb_assembler& push(const push_list list)
    {
        return emit_push_pop(push_pop_operation::push, list);
    }

    // ["ror", "Rx!=HI, Rx!=HI, Rm!=HI", "T16", "0100|000|111|Rm:3|Rx:3", "ARMv4T+ IT=IN"]
    basic_divided_thumb_assembler& ror(const low_reg rx, const low_reg rm) { return emit_alu_operation(alu_operation::ror, rx, rm); }

    // ["sbc", "Rx!=HI, Rx!=HI, Rm!=HI", "T16", "0100|000|110|Rm:3|Rx:3", "ARMv4T+ IT=IN"]
    basic_divided_thumb_assembler& sbc(const low_reg rx, const low_reg rm) { return emit_alu_operation(alu_operation::sbc, rx, rm); }

    // ["stm", "[Rn!=HI]!, RsList", "T16", "1100|0|Rn:3|RsList:8", "ARMv4T+ IT=ANY"]
    template <typename R, typename... Rs>
    basic_divided_thumb_assembler& stmia(const writeback_low_reg rn, const R r, const Rs... rs)
    {
        return stmia(rn, make_low_reg_list(r, rs...));
    }

    basic_divided_thumb_assembler& stmia(const writeback_low_reg rn, const low_reg_list list)
    {
        auto tmp = low_reg(rn.n());
        if (list.contains(tmp) && !list.is_lowest(tmp))
        {
            detail::report_error("Unpredictable behavior");
        }

        return emit_ldmia_stmia(ldmia_stmia_operation::stmia, rn, list);
    }

    // ["str", "Rs!=HI, [Rn!=HI, #ImmZ*4]", "T16", "0110|0|ImmZ:5|Rn:3|Rs:3", "ARMv4T+ IT=ANY"]
    basic_divided_thumb_assembler& str(const low_reg rs, const low_reg rn, const immediate& imm7)
    {
        return emit_load_store_word(false, rs, rn, imm7);
    }

    // ["str", "Rs!=HI, [Rn==SP, #ImmZ*4]", "T16", "1001|0|Rs:3|ImmZ:8", "ARMv4T+ IT=ANY"]
    basic_divided_thumb_assembler& str(const low_reg rs, const reg_sp, const immediate& imm10)
    {
        return emit_sp_relative_load_store(false, rs, imm10);
    }

    // ["str", "Rs!=HI, [Rn!=HI, Rm!=HI]", "T16", "0101|000|Rm:3|Rn:3|Rs:3", "ARMv4T+ IT=ANY"]
    basic_divided_thumb_assembler& str(const low_reg rs, const low_reg rn, const low_reg rm)
    {
        return emit_load_store_with_register_offset(false, false, rs, rn, rm);
    }

    // ["strb", "Rs!=HI, [Rn!=HI, #ImmZ*4]", "T16", "0111|0|ImmZ:5|Rn:3|Rs:3", "ARMv4T+ IT=ANY"]
    // Note: the #ImmZ*4 from asmdb appears to be wrong and should probably just be #ImmZ.
    basic_divided_thumb_assembler& strb(const low_reg rs, const low_reg rn, const immediate& imm5)
    {
        return emit_load_store_byte(false, rs, rn, imm5);
    }

    // ["strb", "Rs!=HI, [Rn!=HI, Rm!=HI]", "T16", "0101|010|Rm:3|Rn:3|Rs:3", "ARMv4T+ IT=ANY"]
    basic_divided_thumb_assembler& strb(const low_reg rs, const low_reg rn, const low_reg rm)
    {
        return emit_load_store_with_register_offset(false, true, rs, rn, rm);
    }

    // ["strh", "Rs!=HI, [Rn!=HI, #ImmZ*4]", "T16", "1000|0|ImmZ:5|Rn:3|Rs:3", "ARMv4T+ IT=ANY"]
    basic_divided_thumb_assembler& strh(const low_reg rs, const low_reg rn, const immediate& imm6)
    {
        return emit_load_store_halfword(false, rs, rn, imm6);
    }

    // ["strh", "Rs!=HI, [Rn!=HI, Rm!=HI]", "T16", "0101|001|Rm:3|Rn:3|Rs:3", "ARMv4T+ IT=ANY"]
    basic_divided_thumb_assembler& strh(const low_reg rs, const low_reg rn, const low_reg rm)
    {
        return emit_load_store_sign_extended(false, false, rs, rn, rm);
    }

    // ["sub", "Rd!=HI, Rn!=HI, #ImmZ", "T16", "0001|111|ImmZ:3|Rn:3|Rd:3", "ARMv4T+ IT=IN"]
    basic_divided_thumb_assembler& sub(const low_reg rd, const low_reg rn, const immediate& imm3)
    {
        return emit_add_sub_imm3(add_sub_operation::sub, rd, rn, imm3);
    }

    // ["sub", "Rx!=HI, Rx!=HI, #ImmZ", "T16", "0011|1|Rx:3|ImmZ:8", "ARMv4T+ IT=IN"]
    basic_divided_thumb_assembler& sub(const low_reg rx, const immediate& imm8)
    {
        return emit_add_sub_imm8(imm8_operation::sub, rx, imm8);
    }

    // ["sub", "Rx==SP, Rx==SP, #ImmZ*4", "T16", "1011|00001|ImmZ:7", "ARMv4T+ IT=ANY"]
    basic_divided_thumb_assembler& sub(const reg_sp, const immediate& imm9)
    {
        return emit_add_sub_sp_imm9(add_sub_operation::sub, imm9);
    }

    // ["sub", "Rd!=HI, Rn!=HI, Rm!=HI", "T16", "0001|101|Rm:3|Rn:3|Rd:3", "ARMv4T+ IT=IN"]
    basic_divided_thumb_assembler& sub(const low_reg rd, const low_reg rn, const low_reg rm)
    {
        return emit_add_sub_register(add_sub_operation::sub, rd, rn, rm);
    }

    // ["svc", "#ImmZ", "T16", "1101|1111|ImmZ:8", "ARMv4T+ IT=ANY"]
    basic_divided_thumb_assembler& swi(const immediate& imm8)
    {
        auto imm = to_abs(reference_type::abs8_unsigned, imm8);
        obj.emit16((0b11011111 << 8) | imm);
        return *this;
    }

    // ["tst", "Rn!=HI, Rn!=HI", "T16", "0100|00|1000|Rn:3|Rn:3", "ARMv4T+ IT=ANY APSR.NZC=W"]
    basic_divided_thumb_assembler& tst(const low_reg rx, const low_reg rm) { return emit_alu_operation(alu_operation::tst, rx, rm); }

private:
    using object = ::lzasm::arm::arm32::detail::object<TSymbolName>;
    using reference_type = ::lzasm::arm::arm32::detail::reference_type;
    using condition_code = ::lzasm::arm::arm32::detail::condition_code;
    using add_sub_operation = ::lzasm::arm::arm32::detail::add_sub_operation;
    using alu_operation = ::lzasm::arm::arm32::detail::alu_operation;
    using high_register_operation = ::lzasm::arm::arm32::detail::high_register_operation;
    using imm8_operation = ::lzasm::arm::arm32::detail::imm8_operation;
    using ldmia_stmia_operation = ::lzasm::arm::arm32::detail::ldmia_stmia_operation;
    using push_pop_operation = ::lzasm::arm::arm32::detail::push_pop_operation;
    using shift_operation = ::lzasm::arm::arm32::detail::shift_operation;

    basic_divided_thumb_assembler& emit_asr_lsr_imm5(shift_operation operation, const low_reg rd, const low_reg rn, const immediate& imm5)
    {
        // ASR and LSR allow shift counts in the range [0, 32] in assembly source.
        //
        // However, the opcodes have only 5 bits to encode the shift count:
        // * A shift count of 0 must be encoded as an LSL opcode with a shift count of 0.
        //   ASR and LSR cannot shift by 0.
        // * Shift counts 1..31 can be encoded as is.
        // * A shift count of 32 is encoded as a shift count of 0.
        assert((operation == shift_operation::asr) || (operation == shift_operation::lsr));
        auto imm = to_abs(reference_type::abs5_asr_lsr, imm5);
        if ((imm == 0) && !imm5.is_symbol_reference())
        {
            // Map a shift count of 0 to an LSR instruction, but only if the shift count is known.
            // If it is a symbol reference we don't know what instruction we're going to generate until link time.
            return lsl(rd, rn, 0);
        }
        else
        {
            // Bitwise and with 31 maps shift counts of 32 to 0.
            obj.emit16((to_underlying(operation) << 11) | ((imm & 31) << 6) | (rn.n() << 3) | rd.n());
            return *this;
        }
    }

    // Caution: although operands are of type reg, only low registers are allowed.
    basic_divided_thumb_assembler& emit_add_sub_register(add_sub_operation operation, const reg rd, const reg rn, const reg rm)
    {
        assert(are_all_low(rd, rn, rm));
        obj.emit16((0b000110 << 10) | (to_underlying(operation) << 9) | (rm.n() << 6) | (rn.n() << 3) | rd.n());
        return *this;
    }

    // Caution: although operands are of type reg, only low registers are allowed.
    basic_divided_thumb_assembler& emit_add_sub_imm3(add_sub_operation operation, const reg rd, const reg rn, const immediate& imm3)
    {
        assert(are_all_low(rd, rn));
        auto imm = to_abs(reference_type::abs3, imm3);
        invert_if_negative(operation, imm);
        obj.emit16((0b000111 << 10) | (to_underlying(operation) << 9) | (imm << 6) | (rn.n() << 3) | rd.n());
        return *this;
    }

    basic_divided_thumb_assembler& emit_add_sub_imm8(imm8_operation operation, const low_reg rx, const immediate& imm8)
    {
        assert((operation == imm8_operation::add) || (operation == imm8_operation::sub));
        auto imm = to_abs(reference_type::abs8_add_sub, imm8);
        invert_if_negative(operation, imm);
        obj.emit16((0b001 << 13) | (to_underlying(operation) << 11) | (rx.n() << 8) | imm);
        return *this;
    }

    basic_divided_thumb_assembler& emit_cmp_mov_imm8(imm8_operation operation, const low_reg rd, const immediate& imm8)
    {
        assert((operation == imm8_operation::cmp) || (operation == imm8_operation::mov));
        auto imm = to_abs(reference_type::abs8_unsigned, imm8);
        obj.emit16((0b001 << 13) | (to_underlying(operation) << 11) | (rd.n() << 8) | imm);
        return *this;
    }

    // Caution: although operands are of type reg, only low registers are allowed.
    basic_divided_thumb_assembler& emit_alu_operation(alu_operation operation, const reg rx, const reg rm)
    {
        assert(are_all_low(rx, rm));
        obj.emit16((0b010000 << 10) | (to_underlying(operation) << 6) | (rm.n() << 3) | rx.n());
        return *this;
    }

    basic_divided_thumb_assembler& emit_high_register_operation(high_register_operation operation, const reg rx, const reg rm)
    {
        assert(!are_all_low(rx, rm));
        obj.emit16((0b010001 << 10) | (to_underlying(operation) << 8) | (rx.high_bit() << 7) | (rm.n() << 3) | (rx.low_bits()));
        return *this;
    }

    basic_divided_thumb_assembler& emit_load_store_with_register_offset(bool is_load, bool is_byte, const low_reg rd_rs, const low_reg rn, const low_reg rm)
    {
        obj.emit16((0b0101000 << 9) | (is_load << 11) | (is_byte << 10) | (rm.n() << 6) | (rn.n() << 3) | (rd_rs.n() << 0));
        return *this;
    }

    basic_divided_thumb_assembler& emit_load_store_sign_extended(bool is_halfword, bool is_sign_extended, const low_reg rd_rs, const low_reg rn, const low_reg rm)
    {
        obj.emit16((0b0101001 << 9) | (is_halfword << 11) | (is_sign_extended << 10) | (rm.n() << 6) | (rn.n() << 3) | (rd_rs.n() << 0));
        return *this;
    }

    basic_divided_thumb_assembler& emit_add_sub_sp_imm9(add_sub_operation operation, const immediate& imm9)
    {
        auto imm = to_abs(reference_type::abs9_add_sub_sp, imm9);
        invert_if_negative(operation, imm);
        obj.emit16((0b10110000 << 8) | (to_underlying(operation) << 7) | (imm / 4));
        return *this;
    }

    template <typename T>
    basic_divided_thumb_assembler& emit_push_pop(push_pop_operation operation, const T list)
    {
        assert((list.n() >= 1) && (list.n() <= 511));
        obj.emit16((to_underlying(operation) << 9) | list.n());
        return *this;
    }

    basic_divided_thumb_assembler& emit_ldmia_stmia(ldmia_stmia_operation operation, const writeback_low_reg rn, const low_reg_list list)
    {
        assert((list.n() >= 1) && (list.n() <= 255));
        obj.emit16((to_underlying(operation) << 11) | (rn.n() << 8) | list.n());
        return *this;
    }

    basic_divided_thumb_assembler& emit_sp_relative_load_store(bool is_load, const low_reg rd_rs, const immediate& imm10)
    {
        auto imm = to_abs(reference_type::abs10, imm10);
        obj.emit16((0b1001 << 12) | (is_load << 11) | (rd_rs.n() << 8) | (imm / 4));
        return *this;
    }

    basic_divided_thumb_assembler& emit_load_store_byte(bool is_load, const low_reg rd_rs, const low_reg rn, const immediate& imm5)
    {
        auto imm = to_abs(reference_type::abs5, imm5);
        obj.emit16((0b0111 << 12) | (is_load << 11) | (imm << 6) | (rn.n() << 3) | rd_rs.n());
        return *this;
    }

    basic_divided_thumb_assembler& emit_load_store_halfword(bool is_load, const low_reg rd_rs, const low_reg rn, const immediate& imm6)
    {
        auto imm = to_abs(reference_type::abs6, imm6);
        obj.emit16((0b1000 << 12) | (is_load << 11) | ((imm / 2) << 6) | (rn.n() << 3) | rd_rs.n());
        return *this;
    }

    basic_divided_thumb_assembler& emit_load_store_word(bool is_load, const low_reg rd_rs, const low_reg rn, const immediate& imm7)
    {
        auto imm = to_abs(reference_type::abs7, imm7);
        obj.emit16((0b0110 << 12) | (is_load << 11) | ((imm / 4) << 6) | (rn.n() << 3) | rd_rs.n());
        return *this;
    }

    basic_divided_thumb_assembler& emit_load_address(bool is_sp, const low_reg rd, const immediate& imm10)
    {
        auto imm = to_abs(reference_type::abs10, imm10);
        obj.emit16((0b1010 << 12) | (is_sp << 11) | (rd.n() << 8) | (imm / 4));
        return *this;
    }

    basic_divided_thumb_assembler& emit_conditional_branch(condition_code cc, const immediate& imm9)
    {
        obj.add_reference(reference_type::conditional_branch, imm9);
        obj.emit16((0b1101 << 12) | (to_underlying(cc) << 8) | dummy_value);
        return *this;
    }

    constexpr immediate_t to_abs(reference_type type, const immediate& imm)
    {
        const auto& d = detail::reference_type_descriptors::get(type);

        if (imm.is_symbol_reference())
        {
            obj.add_reference(d.type, imm);
            return 0;
        }

        auto i = imm.value();
        i = detail::check_immediate_range(i, d.min, d.max);
        i = detail::check_immediate_is_aligned(i, d.alignment);
        return i;
    }

    template <typename TOp, typename TImm>
    static constexpr void invert_if_negative(TOp& operation, TImm& imm)
    {
        if (imm < 0)
        {
            operation = ::lzasm::arm::arm32::detail::invert_operation(operation);
            imm = abs(imm);
        }
    }

    static constexpr bool are_all_low(std::same_as<reg> auto const ... registers)
    {
        return (registers.is_low() && ...);
    }

    static constexpr auto dummy_value = 0;
    object obj;
};

using divided_thumb_assembler = basic_divided_thumb_assembler<std::string>;

}

#endif
