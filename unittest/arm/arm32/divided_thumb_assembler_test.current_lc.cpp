// SPDX-FileCopyrightText: 2021 Thomas Mathys
// SPDX-License-Identifier: MIT
// lzasm: a runtime assembler

#include <boost/test/unit_test.hpp>
#include "lzasm/arm/arm32/divided_thumb_assembler.hpp"

namespace lzasm_unittest
{

using namespace ::lzasm::arm::arm32;

BOOST_AUTO_TEST_SUITE(divided_thumb_assembler_test)

    BOOST_AUTO_TEST_SUITE(current_lc)

        BOOST_AUTO_TEST_CASE(current_lc_is_zero_after_construction)
        {
            divided_thumb_assembler a;
            BOOST_TEST(a.current_lc() == 0u);
        }

        BOOST_AUTO_TEST_CASE(current_lc_increments_when_code_is_emitted)
        {
            divided_thumb_assembler a;

            a.nop();
            BOOST_TEST(a.current_lc() == 2u);

            a.arm_branch(0);
            BOOST_TEST(a.current_lc() == 6u);
        }

    BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()

}
