// SPDX-FileCopyrightText: 2021 Thomas Mathys
// SPDX-License-Identifier: MIT
// lzasm: a runtime assembler

#include <boost/test/unit_test.hpp>
#include "lzasm/arm/arm32/divided_thumb_assembler.hpp"
#include "assembler_test_utilities.hpp"
#include "test_utilities.hpp"

namespace lzasm_unittest
{

using namespace ::lzasm::arm::arm32;

BOOST_AUTO_TEST_SUITE(divided_thumb_assembler_test)

    BOOST_AUTO_TEST_SUITE(add_and_subtract_register)

        BOOST_AUTO_TEST_CASE(add)
        {
            CHECK(add(r0, r1, r2), H(0x1888));
            CHECK(add(r1, r2, r3), H(0x18d1));
            CHECK(add(r2, r3, r4), H(0x191a));
            CHECK(add(r3, r4, r5), H(0x1963));
            CHECK(add(r4, r5, r6), H(0x19ac));
            CHECK(add(r5, r6, r7), H(0x19f5));
            CHECK(add(r6, r7, r0), H(0x183e));
            CHECK(add(r7, r0, r1), H(0x1847));
        }

        BOOST_AUTO_TEST_CASE(sub)
        {
            CHECK(sub(r0, r1, r2), H(0x1a88));
        }

    BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()

}
