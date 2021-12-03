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
#include "lzasm/arm/arm32/detail/immediate.hpp"

namespace lzasm_unittest
{

template <typename T>
using immediate = ::lzasm::arm::arm32::detail::immediate<T>;

// Clang 12 doesn't do CTAD for type aliases, so use a using declaration instead.
using ::lzasm::arm::arm32::symbol;

BOOST_AUTO_TEST_SUITE(immediate_test)

    BOOST_AUTO_TEST_SUITE(TSymbolName_is_int)

        BOOST_AUTO_TEST_CASE(value)
        {
            immediate<int> value(2);
            BOOST_TEST(2 == value.value());
            BOOST_TEST(false == value.is_symbol_reference());
        }

        BOOST_AUTO_TEST_CASE(symbol_reference)
        {
            immediate<int> reference(symbol(3));
            BOOST_TEST(3 == reference.sym().name);
            BOOST_TEST(true == reference.is_symbol_reference());
        }

    BOOST_AUTO_TEST_SUITE_END()

    BOOST_AUTO_TEST_SUITE(TSymbolName_is_std_string)

        immediate<std::string> take_and_return_immediate(immediate<std::string> i)
        {
            return i;
        }

        BOOST_AUTO_TEST_CASE(value)
        {
            immediate<std::string> value(3);
            BOOST_TEST(3 == value.value());
            BOOST_TEST(false == value.is_symbol_reference());
        }

        BOOST_AUTO_TEST_CASE(symbol_reference)
        {
            immediate<std::string> reference(symbol(std::string("name")));
            BOOST_TEST("name" == reference.sym().name);
            BOOST_TEST(true == reference.is_symbol_reference());
        }

        BOOST_AUTO_TEST_CASE(implicit_conversion_from_std_string)
        {
            BOOST_TEST("name" == take_and_return_immediate(std::string("name")).sym().name);
        }

        BOOST_AUTO_TEST_CASE(operator_equal_to)
        {
            BOOST_TEST((immediate<std::string>(1) == immediate<std::string>(0)) == false);
            BOOST_TEST((immediate<std::string>(1) == immediate<std::string>(1)) == true);
            BOOST_TEST((immediate<std::string>("b") == immediate<std::string>("a")) == false);
            BOOST_TEST((immediate<std::string>("b") == immediate<std::string>("b")) == true);
            BOOST_TEST((immediate<std::string>("1") == immediate<std::string>("b")) == false);
        }

    BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()

}
