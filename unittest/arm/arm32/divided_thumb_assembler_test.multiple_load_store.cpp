// MIT License
//
// lzasm: a runtime assembler
// Copyright 2021 Thomas Mathys
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <boost/test/unit_test.hpp>
#include "lzasm/arm/arm32/divided_thumb_assembler.hpp"
#include "assembler_test_utilities.hpp"
#include "test_utilities.hpp"

namespace lzasm_unittest
{

using namespace ::lzasm::arm::arm32;

BOOST_AUTO_TEST_SUITE(divided_thumb_assembler_test)

    BOOST_AUTO_TEST_SUITE(multiple_load_store)

        BOOST_AUTO_TEST_SUITE(ldmia)

            BOOST_AUTO_TEST_CASE(one_register)
            {
                CHECK(ldmia(!r0, r7), H(0xc880));
                CHECK(ldmia(!r7, r0), H(0xcf01));
            }

            BOOST_AUTO_TEST_CASE(comma_separated_registers)
            {
                CHECK(ldmia(!r7, r0, r2, r4, r6), H(0xcf55));
                CHECK(ldmia(!r0, r1, r3, r5, r7), H(0xc8aa));
            }

            BOOST_AUTO_TEST_CASE(register_ranges)
            {
                CHECK(ldmia(!r7, r0 - r3), H(0xcf0f));
                CHECK(ldmia(!r0, r4 - r7), H(0xc8f0));
                CHECK(ldmia(!r7, r0 - r2, r4 - r6), H(0xcf77));
            }

            BOOST_AUTO_TEST_CASE(comma_separated_registers_and_ranges)
            {
                CHECK(ldmia(!r0, r1, r3, r5 - r7), H(0xc8ea));
                CHECK(ldmia(!r7, r0 - r2, r4, r6), H(0xcf57));
            }

        BOOST_AUTO_TEST_SUITE_END()

        BOOST_AUTO_TEST_SUITE(stmia)

            BOOST_AUTO_TEST_CASE(one_register)
            {
                CHECK(stmia(!r0, r7), H(0xc080));
                CHECK(stmia(!r7, r0), H(0xc701));
            }

            BOOST_AUTO_TEST_CASE(comma_separated_registers)
            {
                CHECK(stmia(!r7, r0, r2, r4, r6), H(0xc755));
                CHECK(stmia(!r0, r1, r3, r5, r7), H(0xc0aa));
            }

            BOOST_AUTO_TEST_CASE(register_ranges)
            {
                CHECK(stmia(!r7, r0 - r3), H(0xc70f));
                CHECK(stmia(!r0, r4 - r7), H(0xc0f0));
                CHECK(stmia(!r7, r0 - r2, r4 - r6), H(0xc777));
            }

            BOOST_AUTO_TEST_CASE(comma_separated_registers_and_ranges)
            {
                CHECK(stmia(!r0, r1, r3, r5 - r7), H(0xc0ea));
                CHECK(stmia(!r7, r0 - r2, r4, r6), H(0xc757));
            }

            BOOST_AUTO_TEST_CASE(unpredictable)
            {
                CHECK(stmia(!r0, r0 - r7), H(0xc0ff));
                CHECK_THROWS(stmia(!r1, r0 - r7), is_unpredictable_behavior);
            }

        BOOST_AUTO_TEST_SUITE_END()

    BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()

}
