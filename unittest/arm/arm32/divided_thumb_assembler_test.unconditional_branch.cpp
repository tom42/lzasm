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

    BOOST_AUTO_TEST_SUITE(unconditional_branch)

        BOOST_AUTO_TEST_CASE(constant_with_zero_origin)
        {
            CHECK(b(0xfffff804), H(0xe400));    // Smallest negative relative address
            CHECK(b(2),          H(0xe7ff));    // Biggest negative relative address
            CHECK(b(4),          H(0xe000));    // Relative address is zero
            CHECK(b(6),          H(0xe001));    // Smallest positive relative address
            CHECK(b(0x802),      H(0xe3ff));    // Biggest positive relative address
        }

        BOOST_AUTO_TEST_CASE(constant_with_nonzero_origin)
        {
            divided_thumb_assembler a;

            a.b(0x4);       // Smallest negative relative address
            a.b(0x804);     // Biggest negative relative address
            a.b(0x808);     // Relative address is zero
            a.b(0x80c);     // Smallest positive relative address
            a.b(0x100a);    // Biggest positive relative address

            CHECK_PROGRAM(a, 0x800, H(0xe400, 0xe7ff, 0xe000, 0xe001, 0xe3ff));
        }

        BOOST_AUTO_TEST_CASE(constant_out_of_range_too_small)
        {
            divided_thumb_assembler a;

            a.b(0xfffff803);

            CHECK_LINK_THROWS(a, 0, is_immediate_out_of_range);
        }

        BOOST_AUTO_TEST_CASE(constant_out_of_range_too_big)
        {
            divided_thumb_assembler a;

            a.b(0x803);

            CHECK_LINK_THROWS(a, 0, is_immediate_out_of_range);
        }

        BOOST_AUTO_TEST_CASE(constant_misaligned)
        {
            divided_thumb_assembler a;

            a.b(1);

            CHECK_LINK_THROWS(a, 0, is_misaligned_immediate_value);
        }

        BOOST_AUTO_TEST_CASE(symbol_with_nonzero_origin)
        {
            divided_thumb_assembler a;

            a.label("label"s);
            a.b("label"s);

            CHECK_PROGRAM(a, 0x800, H(0xe7fe));
        }

    BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()

}
