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

    BOOST_AUTO_TEST_SUITE(pseudo_instructions)

        BOOST_AUTO_TEST_SUITE(adr)

            BOOST_AUTO_TEST_CASE(constant_with_zero_origin)
            {
                divided_thumb_assembler a;

                // Because bit 1 of the PC is forced to zero when the address is calculated,
                // the following two instructions generate the same opcode, including the offset.
                a.adr(r0, 8);
                a.adr(r0, 8);

                // Minimum offset. Again due to bit 1 of the PC being forced to zero the same opcode is generated.
                a.adr(r0, 8);
                a.adr(r0, 8);

                // Maximum offset.
                a.adr(r0, 1032);
                a.adr(r0, 1032);

                CHECK_PROGRAM(a, 0, H(0xa001, 0xa001, 0xa000, 0xa000, 0xa0ff, 0xa0ff));
            }

            BOOST_AUTO_TEST_CASE(constant_with_nonzero_origin)
            {
                divided_thumb_assembler a;

                // Constant is treated as absolute value, so resulting offsets are
                //     (256 - 64 - 4) / 4 = 0x2f
                // and (256 - 64 - 8) / 4 = 0x2e
                a.adr(r0, 256);
                a.adr(r0, 256);
                a.adr(r0, 256);
                a.adr(r0, 256);

                CHECK_PROGRAM(a, 64, H(0xa02f, 0xa02f, 0xa02e, 0xa02e));
            }

            BOOST_AUTO_TEST_CASE(constant_out_of_range_too_small)
            {
                divided_thumb_assembler a;

                a.adr(r0, 3);

                CHECK_LINK_THROWS(a, 0, is_immediate_out_of_range);
            }

            BOOST_AUTO_TEST_CASE(constant_out_of_range_too_big)
            {
                divided_thumb_assembler a;

                a.adr(r0, 1025);

                CHECK_LINK_THROWS(a, 0, is_immediate_out_of_range);
            }

            BOOST_AUTO_TEST_CASE(constant_misaligned)
            {
                divided_thumb_assembler a;

                a.adr(r0, 6);

                CHECK_LINK_THROWS(a, 0, is_misaligned_immediate_value);
            }

            BOOST_AUTO_TEST_CASE(symbol_with_zero_origin)
            {
                divided_thumb_assembler a;

                a.adr(r0, "label"s);
                a.adr(r0, "label"s);
                a.adr(r0, "label"s);
                a.adr(r0, "label"s);
                a.align(2);
                a.label("label"s);

                CHECK_PROGRAM(a, 0, H(0xa001, 0xa001, 0xa000, 0xa000));
            }

            BOOST_AUTO_TEST_CASE(symbol_with_nonzero_origin)
            {
                divided_thumb_assembler a;

                a.adr(r0, "label"s);
                a.adr(r0, "label"s);
                a.adr(r0, "label"s);
                a.adr(r0, "label"s);
                a.align(2);
                a.label("label"s);

                CHECK_PROGRAM(a, 0x80, H(0xa001, 0xa001, 0xa000, 0xa000));
            }

        BOOST_AUTO_TEST_SUITE_END()

        BOOST_AUTO_TEST_SUITE(ldr)

            BOOST_AUTO_TEST_CASE(duplicated_constants)
            {
                divided_thumb_assembler a;

                a.label("label"s);
                a.ldr(r0, 0x01234567);
                a.ldr(r0, 0x01234567);
                a.ldr(r0, 0x89abcdef);
                a.ldr(r0, "label"s);
                a.ldr(r0, "label"s);
                a.ldr(r0, 0x89abcdef);

                CHECK_PROGRAM(a, 0x40302010, H(0x4802, 0x4802, 0x4802, 0x4803, 0x4802, 0x4801, 0x4567, 0x0123, 0xcdef, 0x89ab, 0x2010, 0x4030));
            }

            BOOST_AUTO_TEST_CASE(minimum_offset)
            {
                divided_thumb_assembler a;

                a.ldr(r0, 0x12345678);
                a.pool();

                CHECK_PROGRAM(a, 0, H(0x4800, 0x0000, 0x5678, 0x1234));
            }

            BOOST_AUTO_TEST_CASE(maximum_offset)
            {
                divided_thumb_assembler a;

                a.ldr(r0, 0x12345678);
                space(a, 255 * 4 + 2); // 2 bytes of alignment, and then we can reach the next 255 words.
                a.pool();

                bytevector expected_program = to_bytevector(H(0x48ff));
                expected_program.insert(expected_program.end(), 255 * 4 + 2, 0);
                expected_program.push_back(0x78);
                expected_program.push_back(0x56);
                expected_program.push_back(0x34);
                expected_program.push_back(0x12);

                CHECK_PROGRAM(a, 0, expected_program);
            }

            BOOST_AUTO_TEST_CASE(out_of_range_too_big)
            {
                // Note: we can only test the case where the offset is too big:
                // * Because the pool is always somewhere after the instruction that is using the constant,
                //   and because the pool directive always aligns the pool, the offset can never be too small.
                // * Moreover, because the pool directive always aligns the pool, the offset can never be misaligned.
                divided_thumb_assembler a;

                a.ldr(r0, 0x12345678);
                space(a, 255 * 4 + 2 + 1);

                BOOST_REQUIRE_EXCEPTION(a.pool(), std::runtime_error, is_immediate_out_of_range);
            }

        BOOST_AUTO_TEST_SUITE_END()

        BOOST_AUTO_TEST_CASE(nop)
        {
            CHECK(nop(), H(0x46c0));
        }

    BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()

}
