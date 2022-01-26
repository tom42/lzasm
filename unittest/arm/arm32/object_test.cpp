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
#include <boost/test/data/test_case.hpp>
#include <stdexcept>
#include <string>
#include "lzasm/arm/arm32/detail/basic_types.hpp"
#include "lzasm/arm/arm32/detail/object.hpp"
#include "lzasm/arm/arm32/detail/reference.hpp"
#include "lzasm/arm/arm32/detail/symbol.hpp"
#include "test_utilities.hpp"

namespace lzasm_unittest
{

#define CHECK_NOLINK(expected_data)                                                                     \
{                                                                                                       \
    BOOST_TEST(obj.to_bytevector() == to_bytevector(expected_data), boost::test_tools::per_element());  \
}

#define CHECK_LINK(expected_data)                                                                       \
{                                                                                                       \
    obj.link(0);                                                                                        \
    BOOST_TEST(obj.to_bytevector() == to_bytevector(expected_data), boost::test_tools::per_element());  \
}

using namespace std::string_literals;
using object = ::lzasm::arm::arm32::detail::object<std::string>;
using reference_type = ::lzasm::arm::arm32::detail::reference_type;
using symbol = ::lzasm::arm::arm32::symbol<std::string>;

struct object_test_fixture
{
    object obj;
};

static const int out_of_range_alignments[] = { -1, ::lzasm::arm::arm32::detail::max_alignment + 1 };
static const int alignments[] = { 1, 2, 3 };
static const int valid_origins[] = { 0, 8, 16 };

BOOST_FIXTURE_TEST_SUITE(object_test, object_test_fixture)

    BOOST_AUTO_TEST_SUITE(construction)

        BOOST_AUTO_TEST_CASE(object_is_empty_after_construction)
        {
            BOOST_TEST(obj.to_bytevector().size() == 0u);
        }

    BOOST_AUTO_TEST_SUITE_END()

    BOOST_AUTO_TEST_SUITE(add_reference)

        BOOST_AUTO_TEST_CASE(numeric_value)
        {
            obj.add_reference(reference_type::arm_branch, 0);
            BOOST_TEST(obj.to_bytevector().size() == 0u);
        }

        BOOST_AUTO_TEST_CASE(symbolic_value)
        {
            obj.add_reference(reference_type::arm_branch, "some_label"s);
            BOOST_TEST(obj.to_bytevector().size() == 0u);
        }

    BOOST_AUTO_TEST_SUITE_END()

    BOOST_AUTO_TEST_SUITE(add_reference_to_literal)

        BOOST_AUTO_TEST_CASE(new_constant_literal)
        {
            obj.add_reference_to_literal(0x01234567);
            obj.emit16(0x3300);
            obj.add_reference_to_literal(0x89abcdef);
            obj.emit16(0x3300);
            obj.emit_literal_pool();

            CHECK_LINK(H(0x3300, 0x3301, 0x4567, 0x0123, 0xcdef, 0x89ab));
        }

        BOOST_AUTO_TEST_CASE(new_symbolic_literal)
        {
            obj.add_reference_to_literal("symbol1"s);
            obj.emit16(0x3300);
            obj.add_reference_to_literal("symbol2"s);
            obj.emit16(0x3300);
            obj.emit_literal_pool();

            // Since we do not link the symbols don't have to be defined,
            // but the literals in the pool are then all 0.
            CHECK_NOLINK(H(0x3300, 0x3301, 0x0000, 0x0000, 0x0000, 0x0000));
        }

        BOOST_AUTO_TEST_CASE(when_constant_literal_with_same_value_exists_then_no_new_literal_is_added)
        {
            obj.add_reference_to_literal(0x11223344);
            obj.emit16(0x3300);
            obj.add_reference_to_literal(0x11223344);
            obj.emit16(0x3300);
            obj.emit_literal_pool();

            CHECK_LINK(H(0x3300, 0x3300, 0x3344, 0x1122));
        }

        BOOST_AUTO_TEST_CASE(when_symbolic_literal_with_same_value_exists_then_no_new_literal_is_added)
        {
            obj.add_reference_to_literal("same_name"s);
            obj.emit16(0x3300);
            obj.add_reference_to_literal("same_name"s);
            obj.emit16(0x3300);
            obj.emit_literal_pool();

            // Since we do not link the symbols don't have to be defined,
            // but the literals in the pool are then all 0.
            CHECK_NOLINK(H(0x3300, 0x3300, 0x0000, 0x0000));
        }

    BOOST_AUTO_TEST_SUITE_END()

    BOOST_AUTO_TEST_SUITE(add_symbol)

        BOOST_AUTO_TEST_CASE(add_symbol)
        {
            obj.add_symbol(symbol("some_label"s));
            BOOST_TEST(obj.to_bytevector().size() == 0u);
        }

        BOOST_AUTO_TEST_CASE(when_symbol_already_exists_then_fails)
        {
            // Define label with value 0
            obj.add_symbol(symbol("some_label"s));
            obj.emit32(0x11111111);

            // Attempt to redefine the label should fail
            BOOST_CHECK_EXCEPTION(obj.add_symbol(symbol("some_label"s)), std::runtime_error, is_symbol_already_defined);

            // Emit a reference to some_label. It should end up 0 after linking
            // Note that the value emitted here does not matter. It will be completely overwritten.
            obj.add_reference(reference_type::arm_branch, "some_label"s);
            obj.emit32(0x22222222);

            // Verify program
            CHECK_LINK(W(0x11111111, 0xeafffffd));
        }

    BOOST_AUTO_TEST_SUITE_END()

    BOOST_AUTO_TEST_SUITE(align)

        BOOST_DATA_TEST_CASE(when_alignment_is_out_of_range_then_throws, boost::unit_test::data::make(out_of_range_alignments), alignment)
        {
            BOOST_CHECK_EXCEPTION(obj.align(alignment), std::runtime_error, is_alignment_out_of_range);
            BOOST_TEST(obj.to_bytevector().size() == 0u);
        }

        BOOST_DATA_TEST_CASE(when_alignment_is_necessary_then_pc_is_moved, boost::unit_test::data::make(alignments), alignment)
        {
            obj.emit8(0);
            BOOST_TEST(obj.to_bytevector().size() == 1u);

            obj.align(alignment);
            BOOST_TEST(obj.to_bytevector().size() == (1u << alignment));
        }

        BOOST_DATA_TEST_CASE(when_alignment_is_not_necessary_then_pc_is_not_moved, boost::unit_test::data::make(alignments), alignment)
        {
            const auto byte_alignent = 1u << alignment;

            for (auto i = 0u; i < byte_alignent; ++i)
            {
                obj.emit8(0);
            }
            BOOST_TEST(obj.to_bytevector().size() == byte_alignent);

            obj.align(alignment);
            BOOST_TEST(obj.to_bytevector().size() == byte_alignent);
        }

    BOOST_AUTO_TEST_SUITE_END()

    BOOST_AUTO_TEST_SUITE(current_lc)

        BOOST_AUTO_TEST_CASE(current_lc_is_zero_after_construction)
        {
            BOOST_TEST(obj.current_lc() == 0u);
        }

        BOOST_AUTO_TEST_CASE(current_lc_increments_when_code_is_emitted)
        {
            obj.emit8(0);
            BOOST_TEST(obj.current_lc() == 1u);

            obj.emit32(0);
            BOOST_TEST(obj.current_lc() == 5u);
        }

    BOOST_AUTO_TEST_SUITE_END()

    BOOST_AUTO_TEST_SUITE(emit)

        BOOST_AUTO_TEST_CASE(emit8)
        {
            obj.emit8(0x11);
            obj.emit8(0x22);
            CHECK_LINK(B(0x11, 0x22));
        }

        BOOST_AUTO_TEST_CASE(emit16)
        {
            obj.emit16(0x1122);
            obj.emit16(0x3344);
            CHECK_LINK(H(0x1122, 0x3344));
        }

        BOOST_AUTO_TEST_CASE(emit32)
        {
            obj.emit32(0x01234567);
            obj.emit32(0x89abcdef);
            CHECK_LINK(W(0x01234567, 0x89abcdef));
        }

    BOOST_AUTO_TEST_SUITE_END()

    BOOST_AUTO_TEST_SUITE(peek)

        BOOST_AUTO_TEST_CASE(peek8)
        {
            obj.emit16(0x1234);

            BOOST_TEST(obj.peek8(0) == 0x34);
            BOOST_TEST(obj.peek8(1) == 0x12);
        }

        BOOST_AUTO_TEST_CASE(peek16)
        {
            obj.emit32(0x12345678);

            BOOST_TEST(obj.peek16(0) == 0x5678u);
            BOOST_TEST(obj.peek16(1) == 0x3456u);
        }

    BOOST_AUTO_TEST_SUITE_END()

    BOOST_AUTO_TEST_SUITE(poke)

        BOOST_AUTO_TEST_CASE(poke8)
        {
            obj.emit8(0);
            obj.emit8(0);
            obj.emit8(0);

            obj.poke8(1, 2);

            CHECK_LINK(B(0, 2, 0));
        }

        BOOST_AUTO_TEST_CASE(poke16)
        {
            obj.emit16(0);
            obj.emit16(0);
            obj.emit16(0);

            obj.poke16(1, 0x0102);
            obj.poke16(4, 0x0304);

            CHECK_LINK(B(0x00, 0x02, 0x01, 0x00, 0x04, 0x03));
        }

        BOOST_AUTO_TEST_CASE(poke32)
        {
            obj.emit32(0);
            obj.emit32(0);
            obj.emit32(0);

            obj.poke32(1, 0x01020304);
            obj.poke32(8, 0x05060708);

            CHECK_LINK(B(0x00, 0x04, 0x03, 0x02, 0x01, 0, 0, 0, 0x08, 0x07, 0x06, 0x05))
        }

    BOOST_AUTO_TEST_SUITE_END()

    BOOST_AUTO_TEST_SUITE(create_literal_pool)

        BOOST_AUTO_TEST_CASE(when_empty_literal_pool_is_created_at_odd_address_then_nothing_happens)
        {
            obj.emit8(0x42);
            obj.emit_literal_pool();
            CHECK_LINK(B(0x42));
        }

        BOOST_AUTO_TEST_CASE(when_pool_is_created_at_odd_address_then_it_is_aligned)
        {
            obj.add_reference_to_literal(0x01234567);
            obj.emit16(0x3300);
            obj.emit8(0x42);
            obj.emit_literal_pool();
            CHECK_LINK(H(0x3300, 0x0042, 0x4567, 0x0123));
        }

        BOOST_AUTO_TEST_CASE(when_pool_contains_multiple_literals_then_they_are_all_emitted)
        {
            obj.add_reference_to_literal(0x01234567);
            obj.emit16(0x3300);
            obj.add_reference_to_literal(0x89abcdef);
            obj.emit16(0x3300);
            obj.add_reference_to_literal(0x11223344);
            obj.emit16(0x3300);
            obj.add_reference_to_literal(0x55667788);
            obj.emit16(0x3300);
            obj.emit_literal_pool();

            CHECK_LINK(H(0x3301, 0x3302, 0x3302, 0x3303, 0x4567, 0x0123, 0xcdef, 0x89ab, 0x3344, 0x1122, 0x7788, 0x5566));
        }

        BOOST_AUTO_TEST_CASE(when_pool_is_created_then_a_new_pool_is_started)
        {
            obj.add_reference_to_literal(0x11223344);
            obj.emit16(0x3300);
            obj.add_reference_to_literal(0x55667788);
            obj.emit16(0x3300);
            obj.emit_literal_pool();

            // A new pool containing the literal 0x11223344 will now be created.
            // The literals buffer should have been flushed, so the literal
            // 0x55667788 should not show up again.
            obj.add_reference_to_literal(0x11223344);
            obj.emit16(0x3300);
            obj.emit_literal_pool();

            CHECK_LINK(H(0x3300, 0x3301, 0x3344, 0x1122, 0x7788, 0x5566, 0x3300, 0x0000, 0x3344, 0x1122));
        }

        BOOST_AUTO_TEST_CASE(when_symbols_are_used_as_literals_then_references_are_created_and_resolved)
        {
            obj.emit32(0x11111111);
            obj.add_symbol("symbol1"s); // symbol1 = 4
            obj.emit32(0x22222222);
            obj.add_symbol("symbol2"s); // symbol2 = 8
            obj.emit32(0x33333333);
            obj.add_reference_to_literal("symbol2"s); // Create a literal with value 8
            obj.emit16(0x3300);
            obj.add_reference_to_literal("symbol1"s); // Create a literal with value 4
            obj.emit16(0x3300);
            obj.emit_literal_pool();
            CHECK_LINK(H(0x1111, 0x1111, 0x2222, 0x2222, 0x3333, 0x3333, 0x3300, 0x3301, 0x0008, 0x0000, 0x0004, 0x0000));
        }

    BOOST_AUTO_TEST_SUITE_END()

    BOOST_AUTO_TEST_SUITE(link)

        BOOST_DATA_TEST_CASE(valid_origin, boost::unit_test::data::make(valid_origins), origin)
        {
            obj.link(origin);
            BOOST_TEST(obj.to_bytevector().size() == 0u);
        }

        BOOST_AUTO_TEST_CASE(when_origin_is_too_large_then_throws)
        {
            obj.link(::lzasm::arm::arm32::detail::max_address);
            obj.emit8(1);
            BOOST_CHECK_EXCEPTION(obj.link(::lzasm::arm::arm32::detail::max_address), std::runtime_error, is_origin_too_large);
        }

        BOOST_AUTO_TEST_CASE(when_there_are_undefined_symbols_then_throws)
        {
            obj.add_reference(reference_type::arm_branch, "undefined_symbol"s);
            obj.emit32(0);
            BOOST_CHECK_EXCEPTION(obj.link(0), std::runtime_error, is_undefined_symbol);
        }

        BOOST_AUTO_TEST_CASE(when_there_are_unemitted_literals_then_literal_pool_is_created_before_linking)
        {
            obj.add_reference_to_literal(0x11223344);
            obj.emit16(0x3300);
            // Link, without creating the pool first.
            CHECK_LINK(H(0x3300, 0x0000, 0x3344, 0x1122));
        }

    BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()

}
