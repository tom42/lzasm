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
#include <string>
#include "lzasm/arm/arm32/divided_thumb_assembler.hpp"
#include "assembler_test_utilities.hpp"
#include "test_utilities.hpp"

namespace lzasm_unittest
{

using namespace std::string_literals;
using namespace ::lzasm::arm::arm32;

BOOST_AUTO_TEST_SUITE(divided_thumb_assembler_test)

    BOOST_AUTO_TEST_SUITE(conditional_branch)

        BOOST_AUTO_TEST_CASE(condition_codes)
        {
            CHECK(beq(0), H(0xd0fe));
            CHECK(bne(0), H(0xd1fe));
            CHECK(bcs(0), H(0xd2fe));
            CHECK(bcc(0), H(0xd3fe));
            CHECK(bmi(0), H(0xd4fe));
            CHECK(bpl(0), H(0xd5fe));
            CHECK(bvs(0), H(0xd6fe));
            CHECK(bvc(0), H(0xd7fe));
            CHECK(bhi(0), H(0xd8fe));
            CHECK(bls(0), H(0xd9fe));
            CHECK(bge(0), H(0xdafe));
            CHECK(blt(0), H(0xdbfe));
            CHECK(bgt(0), H(0xdcfe));
            CHECK(ble(0), H(0xddfe));
        }

        BOOST_AUTO_TEST_CASE(constant_with_zero_origin)
        {
            CHECK(beq(-252),    H(0xd080)); // Smallest negative relative address
            CHECK(beq(2),       H(0xd0ff)); // Biggest negative relative address
            CHECK(beq(4),       H(0xd000)); // Relative address is zero
            CHECK(beq(6),       H(0xd001)); // Smallest positive relative address
            CHECK(beq(258),     H(0xd07f)); // Biggest positive relative address
        }

        BOOST_AUTO_TEST_CASE(constant_with_nonzero_origin)
        {
            divided_thumb_assembler a;

            a.beq(-124);    // Smallest negative relative address
            a.beq(132);     // Biggest negative relative address
            a.beq(136);     // Relative address is zero
            a.beq(140);     // Smallest positive relative address
            a.beq(394);     // Biggest positive relative address

            CHECK_PROGRAM(a, 0x80, H(0xd080, 0xd0ff, 0xd000, 0xd001, 0xd07f));
        }

        BOOST_AUTO_TEST_CASE(constant_out_of_range_too_small)
        {
            divided_thumb_assembler a;

            a.beq(-253);

            CHECK_LINK_THROWS(a, 0, is_immediate_out_of_range);
        }

        BOOST_AUTO_TEST_CASE(constant_out_of_range_too_big)
        {
            divided_thumb_assembler a;

            a.beq(259);

            CHECK_LINK_THROWS(a, 0, is_immediate_out_of_range);
        }

        BOOST_AUTO_TEST_CASE(constant_misaligned)
        {
            divided_thumb_assembler a;

            a.beq(1);

            CHECK_LINK_THROWS(a, 0, is_misaligned_immediate_value);
        }

        BOOST_AUTO_TEST_CASE(symbol_with_nonzero_origin)
        {
            divided_thumb_assembler a;

            a.label("label"s);
            a.beq("label"s);

            CHECK_PROGRAM(a, 0x80, H(0xd0fe));
        }

    BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()

}
