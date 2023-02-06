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

    BOOST_AUTO_TEST_SUITE(data_definition_directives)

        BOOST_AUTO_TEST_SUITE(asciz)

            BOOST_AUTO_TEST_CASE(from_string)
            {
                // The overload taking std::string delegates to the overload taking iterators,
                // so we can do most of the testing over there.
                CHECK(asciz(std::string("")), B(0));
                CHECK(asciz(std::string("abc")), B('a', 'b', 'c', 0));
            }

            BOOST_AUTO_TEST_CASE(from_char_pointer)
            {
                CHECK(asciz(""), B(0));
                CHECK(asciz("abc"), B('a', 'b', 'c', 0));
            }

            BOOST_AUTO_TEST_CASE(from_iterator)
            {
                const auto empty_range = B();
                const auto single_nul_character(B(0));
                const auto non_empty_range = B('a', 'b', 'c');
                const auto non_empty_range_containing_nul_character = B('d', 'e', 0, 'f');

                CHECK(asciz(empty_range.begin(), empty_range.end()), B(0));
                CHECK(asciz(single_nul_character.begin(), single_nul_character.end()), B(0, 0));
                CHECK(asciz(non_empty_range.begin(), non_empty_range.end()), B('a', 'b', 'c', 0));
                CHECK(asciz(non_empty_range_containing_nul_character.begin(), non_empty_range_containing_nul_character.end()), B('d', 'e', 0, 'f', 0));
            }

        BOOST_AUTO_TEST_SUITE_END()

        BOOST_AUTO_TEST_SUITE(byte)

            BOOST_AUTO_TEST_CASE(single_byte)
            {
                CHECK(byte(-128), B(static_cast<unsigned char>(-128)));
                CHECK(byte(0), B(0));
                CHECK(byte(1), B(1));
                CHECK(byte(255), B(255));
            }

            BOOST_AUTO_TEST_CASE(multiple_bytes)
            {
                CHECK(byte(0, 1, 2, 254, 255), B(0, 1, 2, 254, 255));
            }

            BOOST_AUTO_TEST_CASE(single_byte_immediate_out_of_range)
            {
                CHECK_THROWS(byte(-129), is_immediate_out_of_range);
                CHECK_THROWS(byte(256), is_immediate_out_of_range);
            }

            BOOST_AUTO_TEST_CASE(multiple_bytes_immediate_out_of_range)
            {
                CHECK_THROWS(byte(0, 1, 256, 2), is_immediate_out_of_range, B(0, 1));
            }

            BOOST_AUTO_TEST_CASE(symbol_with_nonzero_origin)
            {
                divided_thumb_assembler a;

                a.byte("label"s);
                a.label("label"s);

                CHECK_PROGRAM(a, 0x80, B(0x81));
            }

        BOOST_AUTO_TEST_SUITE_END()

        BOOST_AUTO_TEST_SUITE(hword)

            BOOST_AUTO_TEST_CASE(single_hword)
            {
                CHECK(hword(-0x8000), H(0x8000));
                CHECK(hword(0), H(0));
                CHECK(hword(1), H(1));
                CHECK(hword(0x1234), H(0x1234));
                CHECK(hword(0xffff), H(0xffff));
            }

            BOOST_AUTO_TEST_CASE(multiple_hwords)
            {
                CHECK(hword(0, 1, 0x0123, 0x4567), H(0x0000, 0x0001, 0x0123, 0x4567));
            }

            BOOST_AUTO_TEST_CASE(single_hword_immediate_out_of_range)
            {
                CHECK_THROWS(hword(-0x8001), is_immediate_out_of_range);
                CHECK_THROWS(hword(0x10000), is_immediate_out_of_range);
            }

            BOOST_AUTO_TEST_CASE(multiple_hwords_immediate_out_of_range)
            {
                CHECK_THROWS(hword(0, 1, 0x10000, 2), is_immediate_out_of_range, H(0, 1));
            }

            BOOST_AUTO_TEST_CASE(symbol_with_nonzero_origin)
            {
                divided_thumb_assembler a;

                a.hword("label"s, 0x1234);
                a.label("label"s);

                CHECK_PROGRAM(a, 0x8000, H(0x8004, 0x1234));
            }

        BOOST_AUTO_TEST_SUITE_END()

        BOOST_AUTO_TEST_SUITE(incbin)

            BOOST_AUTO_TEST_CASE(empty_range)
            {
                bytevector v;
                CHECK(incbin(v.begin(), v.end()), B());
            }

            BOOST_AUTO_TEST_CASE(non_empty_range)
            {
                bytevector v{ 11, 22, 33 };
                CHECK(incbin(v.begin(), v.end()), B(11, 22, 33));
            }

        BOOST_AUTO_TEST_SUITE_END()

        BOOST_AUTO_TEST_SUITE(word)

            BOOST_AUTO_TEST_CASE(single_word)
            {
                CHECK(word(-2147483648), W(0x80000000));
                CHECK(word(0), W(0));
                CHECK(word(1), W(1));
                CHECK(word(0x12345678), W(0x12345678));
                CHECK(word(0xffffffff), W(0xffffffff));
            }

            BOOST_AUTO_TEST_CASE(multiple_words)
            {
                CHECK(word(0, 1, 0x01234567, 0x89abcdef), W(0, 1, 0x01234567, 0x89abcdef));
            }

            BOOST_AUTO_TEST_CASE(symbol_with_nonzero_origin)
            {
                divided_thumb_assembler a;

                a.word("label"s, 0x12345678);
                a.label("label"s);

                CHECK_PROGRAM(a, 0x40000000, W(0x40000008, 0x12345678));
            }

        BOOST_AUTO_TEST_SUITE_END()

    BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()

}
