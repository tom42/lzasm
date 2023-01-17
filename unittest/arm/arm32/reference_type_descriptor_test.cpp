// SPDX-FileCopyrightText: 2021 Thomas Mathys
// SPDX-License-Identifier: MIT
// lzasm: a runtime assembler

#include <boost/test/unit_test.hpp>
#include "lzasm/arm/arm32/detail/reference.hpp"

namespace lzasm_unittest
{

using reference_type = ::lzasm::arm::arm32::detail::reference_type;
using reference_type_descriptor = ::lzasm::arm::arm32::detail::reference_type_descriptor;

BOOST_AUTO_TEST_SUITE(reference_type_descriptor_test)

    BOOST_AUTO_TEST_CASE(bit_mask)
    {
        BOOST_TEST(reference_type_descriptor(reference_type::abs32, 0, 0, 0, 1, 0).bit_mask == 1);
        BOOST_TEST(reference_type_descriptor(reference_type::abs32, 0, 0, 0, 3, 0).bit_mask == 7);
        BOOST_TEST(reference_type_descriptor(reference_type::abs32, 0, 0, 0, 31, 0).bit_mask == 0x7fffffff);
        BOOST_TEST(reference_type_descriptor(reference_type::abs32, 0, 0, 0, 32, 0).bit_mask == static_cast<int>(0xffffffff));
    }

BOOST_AUTO_TEST_SUITE_END()

}
