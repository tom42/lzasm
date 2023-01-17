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

    BOOST_AUTO_TEST_SUITE(arm_code_generation_pseudo_instructions)

        BOOST_AUTO_TEST_SUITE(arm_branch)

            BOOST_AUTO_TEST_CASE(constant_with_zero_origin)
            {
                CHECK(arm_branch(0xfe000008),   W(0xea800000)); // Smallest negative relative address
                CHECK(arm_branch(4),            W(0xeaffffff)); // Biggest negative relative address
                CHECK(arm_branch(8),            W(0xea000000)); // Relative address is zero
                CHECK(arm_branch(12),           W(0xea000001)); // Smallest positive relative address
                CHECK(arm_branch(0x2000004),    W(0xea7fffff)); // Biggest positive relative address
            }

            BOOST_AUTO_TEST_CASE(constant_with_nonzero_origin)
            {
                divided_thumb_assembler a;

                a.arm_branch(0x1000008);    // Smallest negative relative address
                a.arm_branch(0x3000008);    // Biggest negative relative address
                a.arm_branch(0x3000010);    // Relative address is zero
                a.arm_branch(0x3000018);    // Smallest positive relative address
                a.arm_branch(0x5000014);    // Biggest positive relative address

                CHECK_PROGRAM(a, 0x3000000, W(0xea800000, 0xeaffffff, 0xea000000, 0xea000001, 0xea7fffff));
            }

            BOOST_AUTO_TEST_CASE(constant_out_of_range_too_small)
            {
                divided_thumb_assembler a;

                a.arm_branch(0xfe000007);

                CHECK_LINK_THROWS(a, 0, is_immediate_out_of_range);
            }

            BOOST_AUTO_TEST_CASE(constant_out_of_range_too_big)
            {
                divided_thumb_assembler a;

                a.arm_branch(0x2000005);

                CHECK_LINK_THROWS(a, 0, is_immediate_out_of_range);
            }

            BOOST_AUTO_TEST_CASE(constant_misaligned)
            {
                divided_thumb_assembler a;

                a.arm_branch(2);

                CHECK_LINK_THROWS(a, 0, is_misaligned_immediate_value);
            }

            BOOST_AUTO_TEST_CASE(nonzero_fixup_location)
            {
                divided_thumb_assembler a;
                a.arm_branch(0);
                a.arm_branch(0); // Fixup at address 4.
                CHECK_PROGRAM(a, 0, W(0xeafffffe, 0xeafffffd));
            }

            BOOST_AUTO_TEST_CASE(symbol_with_nonzero_origin)
            {
                divided_thumb_assembler a;

                a.label("label"s);
                a.arm_branch("label"s);

                CHECK_PROGRAM(a, 0x2000000, W(0xeafffffe));
            }

        BOOST_AUTO_TEST_SUITE_END()

        BOOST_AUTO_TEST_CASE(arm_to_thumb)
        {
            CHECK(arm_to_thumb(r0),  W(0xe28f0001, 0xe12fff10));
            CHECK(arm_to_thumb(r1),  W(0xe28f1001, 0xe12fff11));
            CHECK(arm_to_thumb(r2),  W(0xe28f2001, 0xe12fff12));
            CHECK(arm_to_thumb(r3),  W(0xe28f3001, 0xe12fff13));
            CHECK(arm_to_thumb(r4),  W(0xe28f4001, 0xe12fff14));
            CHECK(arm_to_thumb(r5),  W(0xe28f5001, 0xe12fff15));
            CHECK(arm_to_thumb(r6),  W(0xe28f6001, 0xe12fff16));
            CHECK(arm_to_thumb(r7),  W(0xe28f7001, 0xe12fff17));
            CHECK(arm_to_thumb(r8),  W(0xe28f8001, 0xe12fff18));
            CHECK(arm_to_thumb(r9),  W(0xe28f9001, 0xe12fff19));
            CHECK(arm_to_thumb(r10), W(0xe28fa001, 0xe12fff1a));
            CHECK(arm_to_thumb(r11), W(0xe28fb001, 0xe12fff1b));
            CHECK(arm_to_thumb(r12), W(0xe28fc001, 0xe12fff1c));
            CHECK(arm_to_thumb(r13), W(0xe28fd001, 0xe12fff1d));
            CHECK(arm_to_thumb(r14), W(0xe28fe001, 0xe12fff1e));
            CHECK(arm_to_thumb(r15), W(0xe28ff001, 0xe12fff1f));
        }

    BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()

}
