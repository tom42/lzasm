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

    BOOST_AUTO_TEST_SUITE(add_offset_to_sp)

        BOOST_AUTO_TEST_CASE(add_offset_to_sp)
        {
            CHECK(add(r13, 0x000), H(0xb000));
            CHECK(add(sp, 0x000), H(0xb000));
            CHECK(add(sp, 0x1fc), H(0xb07f));
            CHECK(sub(sp, 0x000), H(0xb080));
            CHECK(sub(sp, 0x1fc), H(0xb0ff));
        }

        BOOST_AUTO_TEST_CASE(immediate_out_of_range)
        {
            CHECK_THROWS(add(sp, -509), is_immediate_out_of_range);
            CHECK_THROWS(add(sp, 509), is_immediate_out_of_range);
            CHECK_THROWS(sub(sp, -509), is_immediate_out_of_range);
            CHECK_THROWS(sub(sp, 509), is_immediate_out_of_range);
        }

        BOOST_AUTO_TEST_CASE(immediate_is_not_a_multiple_of_4)
        {
            CHECK_THROWS(add(sp, 1), is_misaligned_immediate_value);
            CHECK_THROWS(add(sp, 2), is_misaligned_immediate_value);
            CHECK_THROWS(add(sp, 3), is_misaligned_immediate_value);
            CHECK_THROWS(add(sp, -1), is_misaligned_immediate_value);
        }

        BOOST_AUTO_TEST_CASE(negative_immediate_inverts_instruction)
        {
            CHECK(add(sp, -0x1fc), H(0xb0ff));
            CHECK(sub(sp, -0x1fc), H(0xb07f));
        }

        BOOST_AUTO_TEST_CASE(symbol_with_nonzero_origin)
        {
            divided_thumb_assembler a;

            a.label("label"s);
            a.add(sp, "label"s);
            a.sub(sp, "label"s);

            CHECK_PROGRAM(a, 508, H(0xb07f, 0xb0ff));
        }

    BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()

}
