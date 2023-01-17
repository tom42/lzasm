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

    BOOST_AUTO_TEST_SUITE(software_interrupt)

        BOOST_AUTO_TEST_CASE(software_interrupt)
        {
            CHECK(swi(0x00), H(0xdf00));
            CHECK(swi(0xff), H(0xdfff));
        }

        BOOST_AUTO_TEST_CASE(immediate_out_of_range)
        {
            CHECK_THROWS(swi(-1), is_immediate_out_of_range);
            CHECK_THROWS(swi(256), is_immediate_out_of_range);
        }

        BOOST_AUTO_TEST_CASE(symbol_with_nonzero_origin)
        {
            divided_thumb_assembler a;

            a.swi("label"s);
            a.label("label"s);

            CHECK_PROGRAM(a, 0x80, H(0xdf82));
        }

    BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()

}
