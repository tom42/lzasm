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

    BOOST_AUTO_TEST_SUITE(load_store_halfword)

        BOOST_AUTO_TEST_SUITE(ldrh)

            BOOST_AUTO_TEST_CASE(ldrh)
            {
                CHECK(ldrh(r0, r0, 0b000000), H(0x8800));
                CHECK(ldrh(r0, r7, 0b101010), H(0x8d78));
                CHECK(ldrh(r7, r0, 0b010100), H(0x8a87));
                CHECK(ldrh(r7, r7, 0b111110), H(0x8fff));
            }

            BOOST_AUTO_TEST_CASE(immediate_out_of_range)
            {
                CHECK_THROWS(ldrh(r0, r0, -1), is_immediate_out_of_range);
                CHECK_THROWS(ldrh(r0, r0, 63), is_immediate_out_of_range);
            }

            BOOST_AUTO_TEST_CASE(immediate_is_not_a_multiple_of_2)
            {
                CHECK_THROWS(ldrh(r0, r0, 1), is_misaligned_immediate_value);
            }

            BOOST_AUTO_TEST_CASE(symbol_with_nonzero_origin)
            {
                divided_thumb_assembler a;

                a.label("label"s);
                a.ldrh(r0, r7, "label"s);
                a.ldrh(r1, r6, "label"s);
                a.ldrh(r2, r5, "label"s);
                a.ldrh(r3, r4, "label"s);
                a.ldrh(r4, r3, "label"s);
                a.ldrh(r5, r2, "label"s);
                a.ldrh(r6, r1, "label"s);
                a.ldrh(r7, r0, "label"s);

                CHECK_PROGRAM(a, 62, H(0x8ff8, 0x8ff1, 0x8fea, 0x8fe3, 0x8fdc, 0x8fd5, 0x8fce, 0x8fc7));
            }

        BOOST_AUTO_TEST_SUITE_END()

        BOOST_AUTO_TEST_SUITE(strh)

            BOOST_AUTO_TEST_CASE(strh)
            {
                CHECK(strh(r0, r0, 0b000000), H(0x8000));
                CHECK(strh(r0, r7, 0b101010), H(0x8578));
                CHECK(strh(r7, r0, 0b010100), H(0x8287));
                CHECK(strh(r7, r7, 0b111110), H(0x87ff));
            }

            BOOST_AUTO_TEST_CASE(immediate_out_of_range)
            {
                CHECK_THROWS(strh(r0, r0, -1), is_immediate_out_of_range);
                CHECK_THROWS(strh(r0, r0, 63), is_immediate_out_of_range);
            }

            BOOST_AUTO_TEST_CASE(immediate_is_not_a_multiple_of_2)
            {
                CHECK_THROWS(strh(r0, r0, 1), is_misaligned_immediate_value);
            }

            BOOST_AUTO_TEST_CASE(symbol_with_nonzero_origin)
            {
                divided_thumb_assembler a;

                a.label("label"s);
                a.strh(r0, r7, "label"s);
                a.strh(r1, r6, "label"s);
                a.strh(r2, r5, "label"s);
                a.strh(r3, r4, "label"s);
                a.strh(r4, r3, "label"s);
                a.strh(r5, r2, "label"s);
                a.strh(r6, r1, "label"s);
                a.strh(r7, r0, "label"s);

                CHECK_PROGRAM(a, 62, H(0x87f8, 0x87f1, 0x87ea, 0x87e3, 0x87dc, 0x87d5, 0x87ce, 0x87c7));
            }

        BOOST_AUTO_TEST_SUITE_END()

    BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()

}
