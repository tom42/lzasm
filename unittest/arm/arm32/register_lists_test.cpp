// SPDX-FileCopyrightText: 2021 Thomas Mathys
// SPDX-License-Identifier: MIT
// lzasm: a runtime assembler

#include <boost/test/unit_test.hpp>
#include "lzasm/arm/arm32/detail/register_lists.hpp"

namespace lzasm_unittest
{

using namespace ::lzasm::arm::arm32;

BOOST_AUTO_TEST_SUITE(register_lists_test)

    BOOST_AUTO_TEST_SUITE(low_reg_list_test)

        BOOST_AUTO_TEST_CASE(contains)
        {
            auto list = make_low_reg_list(r4, r6);
            BOOST_CHECK_EQUAL(false, list.contains(r0));
            BOOST_CHECK_EQUAL(false, list.contains(r1));
            BOOST_CHECK_EQUAL(false, list.contains(r2));
            BOOST_CHECK_EQUAL(false, list.contains(r3));
            BOOST_CHECK_EQUAL(true, list.contains(r4));
            BOOST_CHECK_EQUAL(false, list.contains(r5));
            BOOST_CHECK_EQUAL(true, list.contains(r6));
            BOOST_CHECK_EQUAL(false, list.contains(r7));
        }

        BOOST_AUTO_TEST_CASE(lowest_register)
        {
            auto list = make_low_reg_list(r4, r6);
            BOOST_CHECK_EQUAL(false, list.is_lowest(r0));
            BOOST_CHECK_EQUAL(false, list.is_lowest(r1));
            BOOST_CHECK_EQUAL(false, list.is_lowest(r2));
            BOOST_CHECK_EQUAL(false, list.is_lowest(r3));
            BOOST_CHECK_EQUAL(true, list.is_lowest(r4));
            BOOST_CHECK_EQUAL(false, list.is_lowest(r5));
            BOOST_CHECK_EQUAL(false, list.is_lowest(r6));
            BOOST_CHECK_EQUAL(false, list.is_lowest(r7));
        }

    BOOST_AUTO_TEST_SUITE_END()

    BOOST_AUTO_TEST_SUITE(push_list_test)

        BOOST_AUTO_TEST_CASE(size)
        {
            BOOST_TEST(1 == make_push_list(r1).size());
            BOOST_TEST(5 == make_push_list(r0, r3 - r5, lr).size());
        }

    BOOST_AUTO_TEST_SUITE_END()

    BOOST_AUTO_TEST_SUITE(pop_list_test)

        BOOST_AUTO_TEST_CASE(to_pop_list)
        {
            BOOST_TEST(make_pop_list(r0, r7).n() == ::lzasm::arm::arm32::to_pop_list(make_push_list(r0, r7)).n());
            BOOST_TEST(make_pop_list(r0, r7, pc).n() == ::lzasm::arm::arm32::to_pop_list(make_push_list(r0, r7, r14)).n());
        }

    BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()

}
