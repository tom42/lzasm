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

    BOOST_AUTO_TEST_SUITE(high_register_operation)

        BOOST_AUTO_TEST_CASE(low_to_low)
        {
            CHECK(add(r0, r7), H(0x19c0));
            CHECK(add(r1, r6), H(0x1989));
            CHECK(add(r2, r5), H(0x1952));
            CHECK(add(r3, r4), H(0x191b));
            CHECK(add(r4, r3), H(0x18e4));
            CHECK(add(r5, r2), H(0x18ad));
            CHECK(add(r6, r1), H(0x1876));
            CHECK(add(r7, r0), H(0x183f));
        }

        BOOST_AUTO_TEST_CASE(low_to_high)
        {
            CHECK(add(r8, r7), H(0x44b8));
            CHECK(add(r9, r6), H(0x44b1));
            CHECK(add(r10, r5), H(0x44aa));
            CHECK(add(r11, r4), H(0x44a3));
            CHECK(add(r12, r3), H(0x449c));
            CHECK(add(r13, r2), H(0x4495));
            CHECK(add(r14, r1), H(0x448e));
            CHECK(add(r15, r0), H(0x4487));
        }

        BOOST_AUTO_TEST_CASE(high_to_low)
        {
            CHECK(add(r0, r15), H(0x4478));
            CHECK(add(r1, r14), H(0x4471));
            CHECK(add(r2, r13), H(0x446a));
            CHECK(add(r3, r12), H(0x4463));
            CHECK(add(r4, r11), H(0x445c));
            CHECK(add(r5, r10), H(0x4455));
            CHECK(add(r6, r9), H(0x444e));
            CHECK(add(r7, r8), H(0x4447));
        }

        BOOST_AUTO_TEST_CASE(high_to_high)
        {
            CHECK(add(r8, r15), H(0x44f8));
            CHECK(add(r9, r14), H(0x44f1));
            CHECK(add(r10, r13), H(0x44ea));
            CHECK(add(r11, r12), H(0x44e3));
            CHECK(add(r12, r11), H(0x44dc));
            CHECK(add(r13, r10), H(0x44d5));
            CHECK(add(r14, r9), H(0x44ce));
            CHECK(add(r15, r8), H(0x44c7));
        }

        BOOST_AUTO_TEST_CASE(cmp)
        {
            CHECK(cmp(r1, r2), H(0x4291));
            CHECK(cmp(reg(1), reg(2)), H(0x4291));
            CHECK(cmp(r11, r4), H(0x45a3));
            CHECK(cmp(r3, r12), H(0x4563));
            CHECK(cmp(r9, r14), H(0x45f1));
        }

        BOOST_AUTO_TEST_CASE(mov)
        {
            CHECK(mov(r1, r2), H(0x1c11));
            CHECK(mov(reg(1), reg(2)), H(0x1c11));
            CHECK(mov(r11, r4), H(0x46a3));
            CHECK(mov(r3, r12), H(0x4663));
            CHECK(mov(r9, r14), H(0x46f1));
        }

        BOOST_AUTO_TEST_CASE(bx)
        {
            CHECK(bx(r0), H(0x4700));
            CHECK(bx(r1), H(0x4708));
            CHECK(bx(r2), H(0x4710));
            CHECK(bx(r3), H(0x4718));
            CHECK(bx(r4), H(0x4720));
            CHECK(bx(r5), H(0x4728));
            CHECK(bx(r6), H(0x4730));
            CHECK(bx(r7), H(0x4738));
            CHECK(bx(r8), H(0x4740));
            CHECK(bx(r9), H(0x4748));
            CHECK(bx(r10), H(0x4750));
            CHECK(bx(r11), H(0x4758));
            CHECK(bx(r12), H(0x4760));
            CHECK(bx(r13), H(0x4768));
            CHECK(bx(r14), H(0x4770));
            CHECK(bx(r15), H(0x4778));
        }

    BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()

}
