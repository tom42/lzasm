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

    BOOST_AUTO_TEST_SUITE(pc_relative_load)

        BOOST_AUTO_TEST_CASE(ldr)
        {
            CHECK(ldr(r0, pc, 0x3fc), H(0x48ff));
            CHECK(ldr(r1, pc, 0x018), H(0x4906));
            CHECK(ldr(r2, pc, 0x014), H(0x4a05));
            CHECK(ldr(r3, pc, 0x010), H(0x4b04));
            CHECK(ldr(r4, pc, 0x00c), H(0x4c03));
            CHECK(ldr(r5, pc, 0x008), H(0x4d02));
            CHECK(ldr(r6, pc, 0x004), H(0x4e01));
            CHECK(ldr(r7, pc, 0x000), H(0x4f00));
        }

        BOOST_AUTO_TEST_CASE(immediate_out_of_range)
        {
            CHECK_THROWS(ldr(r0, pc, -1), is_immediate_out_of_range);
            CHECK_THROWS(ldr(r0, pc, 255 * 4 + 1), is_immediate_out_of_range);
        }

        BOOST_AUTO_TEST_CASE(immediate_is_not_a_multiple_of_4)
        {
            CHECK_THROWS(ldr(r0, pc, 1), is_misaligned_immediate_value);
            CHECK_THROWS(ldr(r0, pc, 2), is_misaligned_immediate_value);
            CHECK_THROWS(ldr(r0, pc, 3), is_misaligned_immediate_value);
            CHECK_THROWS(ldr(r0, pc, 255 * 4 - 1), is_misaligned_immediate_value);
        }

        BOOST_AUTO_TEST_CASE(symbol_with_nonzero_origin)
        {
            divided_thumb_assembler a;

            a.label("label"s);
            a.ldr(r0, pc, "label"s);

            CHECK_PROGRAM(a, 255 * 4, H(0x48ff));
        }

    BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()

}
