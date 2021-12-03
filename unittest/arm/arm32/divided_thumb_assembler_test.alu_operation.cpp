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

#include <boost/test/unit_test.hpp>
#include "lzasm/arm/arm32/divided_thumb_assembler.hpp"
#include "assembler_test_utilities.hpp"
#include "test_utilities.hpp"

namespace lzasm_unittest
{

using namespace ::lzasm::arm::arm32;

BOOST_AUTO_TEST_SUITE(divided_thumb_assembler_test)

    BOOST_AUTO_TEST_SUITE(alu_operation)

        BOOST_AUTO_TEST_CASE(adc)
        {
            CHECK(adc(r0, r7), H(0x4178));
            CHECK(adc(r1, r6), H(0x4171));
            CHECK(adc(r2, r5), H(0x416a));
            CHECK(adc(r3, r4), H(0x4163));
            CHECK(adc(r4, r3), H(0x415c));
            CHECK(adc(r5, r2), H(0x4155));
            CHECK(adc(r6, r1), H(0x414e));
            CHECK(adc(r7, r0), H(0x4147));
        }

        BOOST_AUTO_TEST_CASE(and_)
        {
            CHECK(and_(r0, r7), H(0x4038));
            CHECK(and_(r1, r6), H(0x4031));
            CHECK(and_(r2, r5), H(0x402a));
            CHECK(and_(r3, r4), H(0x4023));
            CHECK(and_(r4, r3), H(0x401c));
            CHECK(and_(r5, r2), H(0x4015));
            CHECK(and_(r6, r1), H(0x400e));
            CHECK(and_(r7, r0), H(0x4007));
        }

        BOOST_AUTO_TEST_CASE(remaining_operations)
        {
            CHECK(asr(r0, r7), H(0x4138));
            CHECK(bic(r0, r7), H(0x43b8));
            CHECK(cmn(r0, r7), H(0x42f8));
            CHECK(cmp(r0, r7), H(0x42b8));
            CHECK(eor(r0, r7), H(0x4078));
            CHECK(lsl(r0, r7), H(0x40b8));
            CHECK(lsr(r0, r7), H(0x40f8));
            CHECK(mul(r0, r7), H(0x4378));
            CHECK(mvn(r0, r7), H(0x43f8));
            CHECK(neg(r0, r7), H(0x4278));
            CHECK(orr(r0, r7), H(0x4338));
            CHECK(ror(r0, r7), H(0x41f8));
            CHECK(sbc(r0, r7), H(0x41b8));
            CHECK(tst(r0, r7), H(0x4238));
        }

    BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()

}
