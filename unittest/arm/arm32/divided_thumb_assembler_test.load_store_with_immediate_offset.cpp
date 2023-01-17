// SPDX-FileCopyrightText: 2021 Thomas Mathys
// SPDX-License-Identifier: MIT
// lzasm: a runtime assembler

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

    BOOST_AUTO_TEST_SUITE(load_store_with_immediate_offset)

        BOOST_AUTO_TEST_SUITE(ldr)

            BOOST_AUTO_TEST_CASE(ldr)
            {
                CHECK(ldr(r0, r7, 0x00), H(0x6838));
                CHECK(ldr(r1, r6, 0x2c), H(0x6af1));
                CHECK(ldr(r2, r5, 0x58), H(0x6daa));
                CHECK(ldr(r3, r4, 0x7c), H(0x6fe3));
            }

            BOOST_AUTO_TEST_CASE(immediate_out_of_range)
            {
                CHECK_THROWS(ldr(r0, r7, -1), is_immediate_out_of_range);
                CHECK_THROWS(ldr(r0, r7, 125), is_immediate_out_of_range);
            }

            BOOST_AUTO_TEST_CASE(immediate_is_not_a_multiple_of_4)
            {
                CHECK_THROWS(ldr(r0, r7, 0x01), is_misaligned_immediate_value);
                CHECK_THROWS(ldr(r0, r7, 0x02), is_misaligned_immediate_value);
                CHECK_THROWS(ldr(r0, r7, 0x03), is_misaligned_immediate_value);
                CHECK_THROWS(ldr(r0, r7, 0x7b), is_misaligned_immediate_value);
            }

            BOOST_AUTO_TEST_CASE(symbol_with_nonzero_origin)
            {
                divided_thumb_assembler a;

                a.label("label"s);
                a.ldr(r0, r7, "label"s);
                a.ldr(r1, r6, "label"s);
                a.ldr(r2, r5, "label"s);
                a.ldr(r3, r4, "label"s);
                a.ldr(r4, r3, "label"s);
                a.ldr(r5, r2, "label"s);
                a.ldr(r6, r1, "label"s);
                a.ldr(r7, r0, "label"s);

                CHECK_PROGRAM(a, 0x7c, H(0x6ff8, 0x6ff1, 0x6fea, 0x6fe3, 0x6fdc, 0x6fd5, 0x6fce, 0x6fc7));
            }

        BOOST_AUTO_TEST_SUITE_END()

        BOOST_AUTO_TEST_SUITE(ldrb)

            BOOST_AUTO_TEST_CASE(ldrb)
            {
                CHECK(ldrb(r0, r7, 0x00), H(0x7838));
                CHECK(ldrb(r1, r6, 0x15), H(0x7d71));
                CHECK(ldrb(r2, r5, 0x0a), H(0x7aaa));
                CHECK(ldrb(r3, r4, 0x1f), H(0x7fe3));
            }

            BOOST_AUTO_TEST_CASE(immediate_out_of_range)
            {
                CHECK_THROWS(ldrb(r0, r0, -1), is_immediate_out_of_range);
                CHECK_THROWS(ldrb(r0, r0, 32), is_immediate_out_of_range);
            }

            BOOST_AUTO_TEST_CASE(symbol_with_nonzero_origin)
            {
                divided_thumb_assembler a;

                a.label("label"s);
                a.ldrb(r0, r7, "label"s);
                a.ldrb(r1, r6, "label"s);
                a.ldrb(r2, r5, "label"s);
                a.ldrb(r3, r4, "label"s);
                a.ldrb(r4, r3, "label"s);
                a.ldrb(r5, r2, "label"s);
                a.ldrb(r6, r1, "label"s);
                a.ldrb(r7, r0, "label"s);

                CHECK_PROGRAM(a, 30, H(0x7fb8, 0x7fb1, 0x7faa, 0x7fa3, 0x7f9c, 0x7f95, 0x7f8e, 0x7f87));
            }

        BOOST_AUTO_TEST_SUITE_END()

        BOOST_AUTO_TEST_SUITE(str)

            BOOST_AUTO_TEST_CASE(str)
            {
                CHECK(str(r0, r7, 0x00), H(0x6038));
                CHECK(str(r1, r6, 0x2c), H(0x62f1));
                CHECK(str(r2, r5, 0x58), H(0x65aa));
                CHECK(str(r3, r4, 0x7c), H(0x67e3));
            }

            BOOST_AUTO_TEST_CASE(immediate_out_of_range)
            {
                CHECK_THROWS(str(r0, r7, -1), is_immediate_out_of_range);
                CHECK_THROWS(str(r0, r7, 125), is_immediate_out_of_range);
            }

            BOOST_AUTO_TEST_CASE(immediate_is_not_a_multiple_of_4)
            {
                CHECK_THROWS(str(r0, r7, 0x01), is_misaligned_immediate_value);
                CHECK_THROWS(str(r0, r7, 0x02), is_misaligned_immediate_value);
                CHECK_THROWS(str(r0, r7, 0x03), is_misaligned_immediate_value);
                CHECK_THROWS(str(r0, r7, 0x7b), is_misaligned_immediate_value);
            }

            BOOST_AUTO_TEST_CASE(symbol_with_nonzero_origin)
            {
                divided_thumb_assembler a;

                a.label("label"s);
                a.str(r0, r7, "label"s);
                a.str(r1, r6, "label"s);
                a.str(r2, r5, "label"s);
                a.str(r3, r4, "label"s);
                a.str(r4, r3, "label"s);
                a.str(r5, r2, "label"s);
                a.str(r6, r1, "label"s);
                a.str(r7, r0, "label"s);

                CHECK_PROGRAM(a, 0x7c, H(0x67f8, 0x67f1, 0x67ea, 0x67e3, 0x67dc, 0x67d5, 0x67ce, 0x67c7));
            }

        BOOST_AUTO_TEST_SUITE_END()

        BOOST_AUTO_TEST_SUITE(strb)

            BOOST_AUTO_TEST_CASE(strb)
            {
                CHECK(strb(r0, r7, 0x00), H(0x7038));
                CHECK(strb(r1, r6, 0x15), H(0x7571));
                CHECK(strb(r2, r5, 0x0a), H(0x72aa));
                CHECK(strb(r3, r4, 0x1f), H(0x77e3));
            }

            BOOST_AUTO_TEST_CASE(immediate_out_of_range)
            {
                CHECK_THROWS(strb(r0, r0, -1), is_immediate_out_of_range);
                CHECK_THROWS(strb(r0, r0, 32), is_immediate_out_of_range);
            }

            BOOST_AUTO_TEST_CASE(symbol_with_nonzero_origin)
            {
                divided_thumb_assembler a;

                a.label("label"s);
                a.strb(r0, r7, "label"s);
                a.strb(r1, r6, "label"s);
                a.strb(r2, r5, "label"s);
                a.strb(r3, r4, "label"s);
                a.strb(r4, r3, "label"s);
                a.strb(r5, r2, "label"s);
                a.strb(r6, r1, "label"s);
                a.strb(r7, r0, "label"s);

                CHECK_PROGRAM(a, 30, H(0x77b8, 0x77b1, 0x77aa, 0x77a3, 0x779c, 0x7795, 0x778e, 0x7787));
            }

        BOOST_AUTO_TEST_SUITE_END()

    BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()

}
