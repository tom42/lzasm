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

    BOOST_AUTO_TEST_SUITE(push_pop)

        BOOST_AUTO_TEST_SUITE(pop)

            BOOST_AUTO_TEST_CASE(one_register)
            {
                CHECK(pop(r0), H(0xbc01));
                CHECK(pop(pc), H(0xbd00));
            }

            BOOST_AUTO_TEST_CASE(comma_separated_registers)
            {
                CHECK(pop(r0, r2, r4, r6, pc), H(0xbd55));
                CHECK(pop(r1, r3, r5, r7), H(0xbcaa));
            }

            BOOST_AUTO_TEST_CASE(register_ranges)
            {
                CHECK(pop(r0 - r3), H(0xbc0f));
                CHECK(pop(r4 - r7), H(0xbcf0));
                CHECK(pop(r0 - r2, r4 - r6), H(0xbc77));
            }

            BOOST_AUTO_TEST_CASE(comma_separated_registers_and_ranges)
            {
                CHECK(pop(r1, r3, r5 - r7, pc), H(0xbdea));
                CHECK(pop(r0 - r7, pc), H(0xbdff));
            }

        BOOST_AUTO_TEST_SUITE_END()

        BOOST_AUTO_TEST_SUITE(push)

            BOOST_AUTO_TEST_CASE(one_register)
            {
                CHECK(push(r0), H(0xb401));
                CHECK(push(lr), H(0xb500));
            }

            BOOST_AUTO_TEST_CASE(comma_separated_registers)
            {
                CHECK(push(r0, r2, r4, r6, lr), H(0xb555));
                CHECK(push(r1, r3, r5, r7), H(0xb4aa));
            }

            BOOST_AUTO_TEST_CASE(register_ranges)
            {
                CHECK(push(r0 - r3), H(0xb40f));
                CHECK(push(r4 - r7), H(0xb4f0));
                CHECK(push(r0 - r2, r4 - r6), H(0xb477));
            }

            BOOST_AUTO_TEST_CASE(comma_separated_registers_and_ranges)
            {
                CHECK(push(r1, r3, r5 - r7, lr), H(0xb5ea));
                CHECK(push(r0 - r7, lr), H(0xb5ff));
            }

        BOOST_AUTO_TEST_SUITE_END()

    BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()

}
