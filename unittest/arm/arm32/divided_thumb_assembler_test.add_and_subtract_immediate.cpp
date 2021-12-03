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
#include <string>
#include "lzasm/arm/arm32/divided_thumb_assembler.hpp"
#include "assembler_test_utilities.hpp"
#include "test_utilities.hpp"

namespace lzasm_unittest
{

using namespace std::string_literals;
using namespace ::lzasm::arm::arm32;

BOOST_AUTO_TEST_SUITE(divided_thumb_assembler_test)

    BOOST_AUTO_TEST_SUITE(add_and_subtract_immediate)

        BOOST_AUTO_TEST_CASE(add_and_subtract_immediate)
        {
            CHECK(add(r0, r1, 2), H(0x1c88));
            CHECK(add(r1, r2, 3), H(0x1cd1));
            CHECK(add(r2, r3, 4), H(0x1d1a));
            CHECK(add(r3, r4, 5), H(0x1d63));
            CHECK(add(r4, r5, 6), H(0x1dac));
            CHECK(add(r5, r6, 7), H(0x1df5));
            CHECK(add(r6, r7, 0), H(0x1c3e));
            CHECK(add(r7, r0, 1), H(0x1c47));

            CHECK(sub(r0, r1, 7), H(0x1fc8));
        }

        BOOST_AUTO_TEST_CASE(immediate_out_of_range)
        {
            CHECK_THROWS(add(r0, r0, -8), is_immediate_out_of_range);
            CHECK_THROWS(add(r0, r0, 8), is_immediate_out_of_range);
        }

        BOOST_AUTO_TEST_CASE(negative_immediate_inverts_instruction)
        {
            CHECK(add(r0, r1, -7), H(0x1fc8));
            CHECK(sub(r0, r1, -7), H(0x1dc8));
        }

        BOOST_AUTO_TEST_CASE(symbol_with_nonzero_origin)
        {
            divided_thumb_assembler a;

            a.label("label"s);
            a.add(r0, r7, "label"s);
            a.add(r1, r6, "label"s);
            a.add(r2, r5, "label"s);
            a.add(r3, r4, "label"s);
            a.add(r4, r3, "label"s);
            a.add(r5, r2, "label"s);
            a.add(r6, r1, "label"s);
            a.add(r7, r0, "label"s);
            a.sub(r7, r0, "label"s);

            CHECK_PROGRAM(a, 4, H(0x1d38, 0x1d31, 0x1d2a, 0x1d23, 0x1d1c, 0x1d15, 0x1d0e, 0x1d07, 0x1f07));
        }

    BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()

}
