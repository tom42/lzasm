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

    BOOST_AUTO_TEST_SUITE(load_store_with_register_offset)

        BOOST_AUTO_TEST_CASE(ldr)
        {
            CHECK(ldr(r0, r1, r2), H(0x5888));
            CHECK(ldr(r1, r2, r3), H(0x58d1));
            CHECK(ldr(r2, r3, r4), H(0x591a));
            CHECK(ldr(r3, r4, r5), H(0x5963));
            CHECK(ldr(r4, r5, r6), H(0x59ac));
            CHECK(ldr(r5, r6, r7), H(0x59f5));
            CHECK(ldr(r6, r7, r0), H(0x583e));
            CHECK(ldr(r7, r0, r1), H(0x5847));
        }

        BOOST_AUTO_TEST_CASE(ldrb)
        {
            CHECK(ldrb(r0, r1, r2), H(0x5c88));
            CHECK(ldrb(r1, r2, r3), H(0x5cd1));
            CHECK(ldrb(r2, r3, r4), H(0x5d1a));
            CHECK(ldrb(r3, r4, r5), H(0x5d63));
            CHECK(ldrb(r4, r5, r6), H(0x5dac));
            CHECK(ldrb(r5, r6, r7), H(0x5df5));
            CHECK(ldrb(r6, r7, r0), H(0x5c3e));
            CHECK(ldrb(r7, r0, r1), H(0x5c47));
        }

        BOOST_AUTO_TEST_CASE(str)
        {
            CHECK(str(r0, r1, r2), H(0x5088));
            CHECK(str(r1, r2, r3), H(0x50d1));
            CHECK(str(r2, r3, r4), H(0x511a));
            CHECK(str(r3, r4, r5), H(0x5163));
            CHECK(str(r4, r5, r6), H(0x51ac));
            CHECK(str(r5, r6, r7), H(0x51f5));
            CHECK(str(r6, r7, r0), H(0x503e));
            CHECK(str(r7, r0, r1), H(0x5047));
        }

        BOOST_AUTO_TEST_CASE(strb)
        {
            CHECK(strb(r0, r1, r2), H(0x5488));
            CHECK(strb(r1, r2, r3), H(0x54d1));
            CHECK(strb(r2, r3, r4), H(0x551a));
            CHECK(strb(r3, r4, r5), H(0x5563));
            CHECK(strb(r4, r5, r6), H(0x55ac));
            CHECK(strb(r5, r6, r7), H(0x55f5));
            CHECK(strb(r6, r7, r0), H(0x543e));
            CHECK(strb(r7, r0, r1), H(0x5447));
        }

    BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()

}
