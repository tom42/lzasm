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
