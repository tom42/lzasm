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

    BOOST_AUTO_TEST_SUITE(sp_relative_load_store)

        BOOST_AUTO_TEST_SUITE(ldr)

            BOOST_AUTO_TEST_CASE(ldr)
            {
                CHECK(ldr(r0, sp, 0x3fc), H(0x98ff));
                CHECK(ldr(r1, sp, 0x018), H(0x9906));
                CHECK(ldr(r2, sp, 0x014), H(0x9a05));
                CHECK(ldr(r3, sp, 0x010), H(0x9b04));
                CHECK(ldr(r4, sp, 0x00c), H(0x9c03));
                CHECK(ldr(r5, sp, 0x008), H(0x9d02));
                CHECK(ldr(r6, sp, 0x004), H(0x9e01));
                CHECK(ldr(r7, sp, 0x000), H(0x9f00));
            }

            BOOST_AUTO_TEST_CASE(immediate_out_of_range)
            {
                CHECK_THROWS(ldr(r0, sp, -1), is_immediate_out_of_range);
                CHECK_THROWS(ldr(r0, sp, 255 * 4 + 1), is_immediate_out_of_range);
            }

            BOOST_AUTO_TEST_CASE(immediate_is_not_a_multiple_of_4)
            {
                CHECK_THROWS(ldr(r0, sp, 1), is_misaligned_immediate_value);
                CHECK_THROWS(ldr(r0, sp, 2), is_misaligned_immediate_value);
                CHECK_THROWS(ldr(r0, sp, 3), is_misaligned_immediate_value);
                CHECK_THROWS(ldr(r0, sp, 255 * 4 - 1), is_misaligned_immediate_value);
            }

            BOOST_AUTO_TEST_CASE(symbol_with_nonzero_origin)
            {
                divided_thumb_assembler a;

                a.label("label"s);
                a.ldr(r0, sp, "label"s);

                CHECK_PROGRAM(a, 255 * 4, H(0x98ff));
            }

        BOOST_AUTO_TEST_SUITE_END()

        BOOST_AUTO_TEST_SUITE(str)

            BOOST_AUTO_TEST_CASE(str)
            {
                CHECK(str(r0, sp, 0x3fc), H(0x90ff));
                CHECK(str(r1, sp, 0x018), H(0x9106));
                CHECK(str(r2, sp, 0x014), H(0x9205));
                CHECK(str(r3, sp, 0x010), H(0x9304));
                CHECK(str(r4, sp, 0x00c), H(0x9403));
                CHECK(str(r5, sp, 0x008), H(0x9502));
                CHECK(str(r6, sp, 0x004), H(0x9601));
                CHECK(str(r7, sp, 0x000), H(0x9700));
            }

            BOOST_AUTO_TEST_CASE(immediate_out_of_range)
            {
                CHECK_THROWS(str(r0, sp, -1), is_immediate_out_of_range);
                CHECK_THROWS(str(r0, sp, 255 * 4 + 1), is_immediate_out_of_range);
            }

            BOOST_AUTO_TEST_CASE(immediate_is_not_a_multiple_of_4)
            {
                CHECK_THROWS(str(r0, sp, 1), is_misaligned_immediate_value);
                CHECK_THROWS(str(r0, sp, 2), is_misaligned_immediate_value);
                CHECK_THROWS(str(r0, sp, 3), is_misaligned_immediate_value);
                CHECK_THROWS(str(r0, sp, 255 * 4 - 1), is_misaligned_immediate_value);
            }

            BOOST_AUTO_TEST_CASE(symbol_with_nonzero_origin)
            {
                divided_thumb_assembler a;

                a.label("label"s);
                a.str(r0, sp, "label"s);

                CHECK_PROGRAM(a, 255 * 4, H(0x90ff));
            }

        BOOST_AUTO_TEST_SUITE_END()

    BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()

}
