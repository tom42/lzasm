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

    BOOST_AUTO_TEST_SUITE(load_address)

        BOOST_AUTO_TEST_SUITE(from_sp)

            BOOST_AUTO_TEST_CASE(from_sp)
            {
                CHECK(add(r0, sp, 255 * 4), H(0xa8ff));
                CHECK(add(r7, sp, 0), H(0xaf00));
            }

            BOOST_AUTO_TEST_CASE(immediate_out_of_range)
            {
                CHECK_THROWS(add(r0, sp, -1), is_immediate_out_of_range);
                CHECK_THROWS(add(r0, sp, 255 * 4 + 1), is_immediate_out_of_range);
            }

            BOOST_AUTO_TEST_CASE(immediate_is_not_a_multiple_of_4)
            {
                CHECK_THROWS(add(r0, sp, 1), is_misaligned_immediate_value);
                CHECK_THROWS(add(r0, sp, 2), is_misaligned_immediate_value);
                CHECK_THROWS(add(r0, sp, 3), is_misaligned_immediate_value);
                CHECK_THROWS(add(r0, sp, 255 * 4 - 1), is_misaligned_immediate_value);
            }

            BOOST_AUTO_TEST_CASE(symbol_with_nonzero_origin)
            {
                divided_thumb_assembler a;

                a.label("label"s);
                a.add(r0, sp, "label"s);

                CHECK_PROGRAM(a, 255 * 4, H(0xa8ff));
            }

        BOOST_AUTO_TEST_SUITE_END()

        BOOST_AUTO_TEST_SUITE(from_pc)

            BOOST_AUTO_TEST_CASE(from_pc)
            {
                CHECK(add(r0, pc, 255 * 4), H(0xa0ff));
                CHECK(add(r7, pc, 0), H(0xa700));
            }

            BOOST_AUTO_TEST_CASE(immediate_out_of_range)
            {
                CHECK_THROWS(add(r0, pc, -1), is_immediate_out_of_range);
                CHECK_THROWS(add(r0, pc, 255 * 4 + 1), is_immediate_out_of_range);
            }

            BOOST_AUTO_TEST_CASE(immediate_is_not_a_multiple_of_4)
            {
                CHECK_THROWS(add(r0, pc, 1), is_misaligned_immediate_value);
                CHECK_THROWS(add(r0, pc, 2), is_misaligned_immediate_value);
                CHECK_THROWS(add(r0, pc, 3), is_misaligned_immediate_value);
                CHECK_THROWS(add(r0, pc, 255 * 4 - 1), is_misaligned_immediate_value);
            }

            BOOST_AUTO_TEST_CASE(symbol_with_nonzero_origin)
            {
                divided_thumb_assembler a;

                a.label("label"s);
                a.add(r0, pc, "label"s);

                CHECK_PROGRAM(a, 255 * 4, H(0xa0ff));
            }

        BOOST_AUTO_TEST_SUITE_END()

    BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()

}
