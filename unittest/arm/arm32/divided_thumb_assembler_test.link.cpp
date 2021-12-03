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

    BOOST_AUTO_TEST_SUITE(link)

        BOOST_AUTO_TEST_SUITE(fix_abs5_asr_lsr)

            BOOST_AUTO_TEST_CASE(shift_count_of_zero_is_mapped_to_lsl)
            {
                divided_thumb_assembler a;

                a.label("label"s);
                a.asr(r3, r5, "label"s);
                a.lsr(r5, r3, "label"s);

                CHECK_PROGRAM(a, 0, H(0x002b, 0x001d));
            }

            BOOST_AUTO_TEST_CASE(shift_count_of_32_is_mapped_to_zero)
            {
                divided_thumb_assembler a;

                a.label("label"s);
                a.asr(r3, r5, "label"s);
                a.lsr(r5, r3, "label"s);

                CHECK_PROGRAM(a, 32, H(0x102b, 0x081d));
            }

        BOOST_AUTO_TEST_SUITE_END()

        BOOST_AUTO_TEST_SUITE(fix_abs_generic)

            BOOST_AUTO_TEST_SUITE(abs6)

                BOOST_AUTO_TEST_CASE(immediate_is_out_of_range)
                {
                    divided_thumb_assembler a;

                    a.label("label"s);
                    a.ldrh(r0, r0, "label"s);

                    CHECK_LINK_THROWS(a, 63, is_immediate_out_of_range);
                }

                BOOST_AUTO_TEST_CASE(immediate_is_misaligned)
                {
                    divided_thumb_assembler a;

                    a.ldrh(r0, r0, "label"s);
                    a.byte(0);
                    a.label("label"s);

                    CHECK_LINK_THROWS(a, 0, is_misaligned_immediate_value);
                }

            BOOST_AUTO_TEST_SUITE_END()

            BOOST_AUTO_TEST_SUITE(abs7)

                BOOST_AUTO_TEST_CASE(immediate_is_out_of_range)
                {
                    divided_thumb_assembler a;

                    a.label("label"s);
                    a.ldr(r0, r0, "label"s);

                    CHECK_LINK_THROWS(a, 125, is_immediate_out_of_range);
                }

                BOOST_AUTO_TEST_CASE(immediate_is_misaligned)
                {
                    divided_thumb_assembler a;

                    a.label("label"s);
                    a.ldr(r0, r0, "label"s);

                    CHECK_LINK_THROWS(a, 2, is_misaligned_immediate_value);
                }

            BOOST_AUTO_TEST_SUITE_END()

        BOOST_AUTO_TEST_SUITE_END()

        BOOST_AUTO_TEST_SUITE(fix_abs8_byte)

            BOOST_AUTO_TEST_CASE(immediate_is_out_of_range)
            {
                divided_thumb_assembler a;

                a.label("label"s);
                a.byte("label"s);

                CHECK_LINK_THROWS(a, 256, is_immediate_out_of_range);
            }

        BOOST_AUTO_TEST_SUITE_END()

        BOOST_AUTO_TEST_SUITE(get_relative_immediate_bits)

            BOOST_AUTO_TEST_CASE(relative_address_out_of_range_too_big)
            {
                divided_thumb_assembler a;

                a.beq("label"s);
                space(a, 127 * 2 + 2 + 1);
                a.label("label"s);

                CHECK_LINK_THROWS(a, 0, is_immediate_out_of_range);
            }

            BOOST_AUTO_TEST_CASE(relative_address_misaligned)
            {
                divided_thumb_assembler a;

                a.beq("label"s);
                a.byte(0);
                a.label("label"s);

                CHECK_LINK_THROWS(a, 0, is_misaligned_immediate_value);
            }

        BOOST_AUTO_TEST_SUITE_END()

    BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()

}
