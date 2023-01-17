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

    BOOST_AUTO_TEST_SUITE(long_branch_with_link)

        BOOST_AUTO_TEST_CASE(constant_with_zero_origin)
        {
            CHECK(bl(0xffc00004), H(0xf400, 0xf800));   // Smallest negative relative address
            CHECK(bl(2),          H(0xf7ff, 0xffff));   // Biggest negative relative address
            CHECK(bl(4),          H(0xf000, 0xf800));   // Relative address is zero
            CHECK(bl(6),          H(0xf000, 0xf801));   // Smallest positive relative address
            CHECK(bl(0x400002),   H(0xf3ff, 0xffff));   // Biggest positive relative address
        }

        BOOST_AUTO_TEST_CASE(constant_with_nonzero_origin)
        {
            divided_thumb_assembler a;

            a.bl(0x200004); // Smallest negative relative address
            a.bl(0x600006); // Biggest negative relative address
            a.bl(0x60000c); // Relative address is zero
            a.bl(0x600012); // Smallest positive relative address
            a.bl(0xa00012); // Biggest positive relative address

            CHECK_PROGRAM(a, 0x600000, H(0xf400, 0xf800, 0xf7ff, 0xffff, 0xf000, 0xf800, 0xf000, 0xf801, 0xf3ff, 0xffff));
        }

        BOOST_AUTO_TEST_CASE(constant_out_of_range_too_small)
        {
            divided_thumb_assembler a;

            a.bl(0xffc00003);

            CHECK_LINK_THROWS(a, 0, is_immediate_out_of_range);
        }

        BOOST_AUTO_TEST_CASE(constant_out_of_range_too_big)
        {
            divided_thumb_assembler a;

            a.bl(0x400003);

            CHECK_LINK_THROWS(a, 0, is_immediate_out_of_range);
        }

        BOOST_AUTO_TEST_CASE(constant_misaligned)
        {
            divided_thumb_assembler a;

            a.bl(1);

            CHECK_LINK_THROWS(a, 0, is_misaligned_immediate_value);
        }

        BOOST_AUTO_TEST_CASE(symbol_with_nonzero_origin)
        {
            divided_thumb_assembler a;

            a.label("label"s);
            a.bl("label"s);

            CHECK_PROGRAM(a, 0x400000, H(0xf7ff, 0xfffe));
        }

    BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()

}
