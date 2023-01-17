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

    BOOST_AUTO_TEST_SUITE(move_shifted_register)

        BOOST_AUTO_TEST_SUITE(lsl)

            BOOST_AUTO_TEST_CASE(lsl)
            {
                CHECK(lsl(r0, r7, 0x00), H(0x0038));
                CHECK(lsl(r1, r6, 0x05), H(0x0171));
                CHECK(lsl(r2, r5, 0x0a), H(0x02aa));
                CHECK(lsl(r3, r4, 0x0f), H(0x03e3));
                CHECK(lsl(r4, r3, 0x14), H(0x051c));
                CHECK(lsl(r5, r2, 0x19), H(0x0655));
                CHECK(lsl(r6, r1, 0x1e), H(0x078e));
                CHECK(lsl(r7, r0, 0x1f), H(0x07c7));
            }

            BOOST_AUTO_TEST_CASE(immediate_out_of_range)
            {
                CHECK_THROWS(lsl(r0, r7, -1), is_immediate_out_of_range);
                CHECK_THROWS(lsl(r0, r7, 32), is_immediate_out_of_range);
            }

            BOOST_AUTO_TEST_CASE(symbol_with_nonzero_origin)
            {
                divided_thumb_assembler a;

                a.label("label"s);
                a.lsl(r0, r0, "label"s);

                CHECK_PROGRAM(a, 30, H(0x0780));
            }

        BOOST_AUTO_TEST_SUITE_END()

        BOOST_AUTO_TEST_SUITE(asr)

            BOOST_AUTO_TEST_CASE(asr)
            {
                CHECK(asr(r0, r7, 0x00), H(0x0038));
                CHECK(asr(r1, r6, 0x01), H(0x1071));
                CHECK(asr(r2, r5, 0x02), H(0x10aa));
                CHECK(asr(r3, r4, 0x03), H(0x10e3));
                CHECK(asr(r4, r3, 0x04), H(0x111c));
                CHECK(asr(r5, r2, 0x05), H(0x1155));
                CHECK(asr(r6, r1, 0x06), H(0x118e));
                CHECK(asr(r7, r0, 0x07), H(0x11c7));
                CHECK(asr(r0, r7, 0x08), H(0x1238));
                CHECK(asr(r1, r6, 0x09), H(0x1271));
                CHECK(asr(r2, r5, 0x0a), H(0x12aa));
                CHECK(asr(r3, r4, 0x0b), H(0x12e3));
                CHECK(asr(r4, r3, 0x0c), H(0x131c));
                CHECK(asr(r5, r2, 0x0d), H(0x1355));
                CHECK(asr(r6, r1, 0x0e), H(0x138e));
                CHECK(asr(r7, r0, 0x0f), H(0x13c7));
                CHECK(asr(r0, r7, 0x10), H(0x1438));
                CHECK(asr(r1, r6, 0x11), H(0x1471));
                CHECK(asr(r2, r5, 0x12), H(0x14aa));
                CHECK(asr(r3, r4, 0x13), H(0x14e3));
                CHECK(asr(r4, r3, 0x14), H(0x151c));
                CHECK(asr(r5, r2, 0x15), H(0x1555));
                CHECK(asr(r6, r1, 0x16), H(0x158e));
                CHECK(asr(r7, r0, 0x17), H(0x15c7));
                CHECK(asr(r0, r7, 0x18), H(0x1638));
                CHECK(asr(r1, r6, 0x19), H(0x1671));
                CHECK(asr(r2, r5, 0x1a), H(0x16aa));
                CHECK(asr(r3, r4, 0x1b), H(0x16e3));
                CHECK(asr(r4, r3, 0x1c), H(0x171c));
                CHECK(asr(r5, r2, 0x1d), H(0x1755));
                CHECK(asr(r6, r1, 0x1e), H(0x178e));
                CHECK(asr(r7, r0, 0x1f), H(0x17c7));
                CHECK(asr(r0, r7, 0x20), H(0x1038));
            }

            BOOST_AUTO_TEST_CASE(immediate_out_of_range)
            {
                CHECK_THROWS(asr(r0, r7, -1), is_immediate_out_of_range);
                CHECK_THROWS(asr(r0, r7, 33), is_immediate_out_of_range);
            }

            BOOST_AUTO_TEST_CASE(symbol_with_nonzero_origin)
            {
                divided_thumb_assembler a;

                a.label("label"s);
                a.asr(r0, r0, "label"s);

                CHECK_PROGRAM(a, 30, H(0x1780));
            }

        BOOST_AUTO_TEST_SUITE_END()

        BOOST_AUTO_TEST_SUITE(lsr)

            BOOST_AUTO_TEST_CASE(lsr)
            {
                CHECK(lsr(r0, r7, 0x00), H(0x0038));
                CHECK(lsr(r1, r6, 0x01), H(0x0871));
                CHECK(lsr(r2, r5, 0x02), H(0x08aa));
                CHECK(lsr(r3, r4, 0x03), H(0x08e3));
                CHECK(lsr(r4, r3, 0x04), H(0x091c));
                CHECK(lsr(r5, r2, 0x05), H(0x0955));
                CHECK(lsr(r6, r1, 0x06), H(0x098e));
                CHECK(lsr(r7, r0, 0x07), H(0x09c7));
                CHECK(lsr(r0, r7, 0x08), H(0x0a38));
                CHECK(lsr(r1, r6, 0x09), H(0x0a71));
                CHECK(lsr(r2, r5, 0x0a), H(0x0aaa));
                CHECK(lsr(r3, r4, 0x0b), H(0x0ae3));
                CHECK(lsr(r4, r3, 0x0c), H(0x0b1c));
                CHECK(lsr(r5, r2, 0x0d), H(0x0b55));
                CHECK(lsr(r6, r1, 0x0e), H(0x0b8e));
                CHECK(lsr(r7, r0, 0x0f), H(0x0bc7));
                CHECK(lsr(r0, r7, 0x10), H(0x0c38));
                CHECK(lsr(r1, r6, 0x11), H(0x0c71));
                CHECK(lsr(r2, r5, 0x12), H(0x0caa));
                CHECK(lsr(r3, r4, 0x13), H(0x0ce3));
                CHECK(lsr(r4, r3, 0x14), H(0x0d1c));
                CHECK(lsr(r5, r2, 0x15), H(0x0d55));
                CHECK(lsr(r6, r1, 0x16), H(0x0d8e));
                CHECK(lsr(r7, r0, 0x17), H(0x0dc7));
                CHECK(lsr(r0, r7, 0x18), H(0x0e38));
                CHECK(lsr(r1, r6, 0x19), H(0x0e71));
                CHECK(lsr(r2, r5, 0x1a), H(0x0eaa));
                CHECK(lsr(r3, r4, 0x1b), H(0x0ee3));
                CHECK(lsr(r4, r3, 0x1c), H(0x0f1c));
                CHECK(lsr(r5, r2, 0x1d), H(0x0f55));
                CHECK(lsr(r6, r1, 0x1e), H(0x0f8e));
                CHECK(lsr(r7, r0, 0x1f), H(0x0fc7));
                CHECK(lsr(r0, r7, 0x20), H(0x0838));
            }

            BOOST_AUTO_TEST_CASE(immediate_out_of_range)
            {
                CHECK_THROWS(lsr(r0, r7, -1), is_immediate_out_of_range);
                CHECK_THROWS(lsr(r0, r7, 33), is_immediate_out_of_range);
            }

            BOOST_AUTO_TEST_CASE(symbol_with_nonzero_origin)
            {
                divided_thumb_assembler a;

                a.label("label"s);
                a.lsr(r0, r0, "label"s);

                CHECK_PROGRAM(a, 30, H(0x0f80));
            }

        BOOST_AUTO_TEST_SUITE_END()

    BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()

}
