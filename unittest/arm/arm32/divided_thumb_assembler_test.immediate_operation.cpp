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

    BOOST_AUTO_TEST_SUITE(immediate_operation)

        BOOST_AUTO_TEST_CASE(immediate_operation)
        {
            CHECK(mov(r0, 0xff), H(0x20ff));
            CHECK(mov(r7, 0x00), H(0x2700));
            CHECK(cmp(r0, 0xff), H(0x28ff));
            CHECK(cmp(r7, 0x00), H(0x2f00));
            CHECK(add(r0, 0xff), H(0x30ff));
            CHECK(add(r7, 0x00), H(0x3700));
            CHECK(sub(r0, 0xff), H(0x38ff));
            CHECK(sub(r7, 0x00), H(0x3f00));
        }

        BOOST_AUTO_TEST_CASE(immediate_out_of_range)
        {
            CHECK_THROWS(cmp(r0, -1), is_immediate_out_of_range);
            CHECK_THROWS(mov(r0, 256), is_immediate_out_of_range);
            CHECK_THROWS(add(r0, -256), is_immediate_out_of_range);
            CHECK_THROWS(sub(r0, 256), is_immediate_out_of_range);
        }

        BOOST_AUTO_TEST_CASE(add_sub_negative_immediate_inverts_instruction)
        {
            CHECK(add(r0, -0xff), H(0x38ff));
            CHECK(sub(r0, -0xff), H(0x30ff));
        }

        BOOST_AUTO_TEST_CASE(symbol_with_nonzero_origin)
        {
            divided_thumb_assembler a;

            a.label("label"s);
            a.add(r0, "label"s);
            a.cmp(r7, "label"s);
            a.mov(r0, "label"s);
            a.sub(r7, "label"s);

            CHECK_PROGRAM(a, 0x80, H(0x3080, 0x2f80, 0x2080, 0x3f80));
        }

    BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()

}
