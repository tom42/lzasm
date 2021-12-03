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
#include "lzasm/arm/arm32/detail/symbol.hpp"

namespace lzasm_unittest
{

using namespace ::lzasm::arm::arm32;

BOOST_AUTO_TEST_SUITE(symbol_test)

    BOOST_AUTO_TEST_SUITE(TSymbolName_is_int)

        BOOST_AUTO_TEST_CASE(construction)
        {
            BOOST_TEST(1 == symbol<int>(1).name);
            BOOST_TEST(2 == symbol<int>(2).name);
        }

        BOOST_AUTO_TEST_CASE(operator_less_than)
        {
            BOOST_TEST((symbol<int>(1) < symbol<int>(0)) == false);
            BOOST_TEST((symbol<int>(1) < symbol<int>(1)) == false);
            BOOST_TEST((symbol<int>(1) < symbol<int>(2)) == true);
        }

        BOOST_AUTO_TEST_CASE(operator_equal_to)
        {
            BOOST_TEST((symbol<int>(1) == symbol<int>(0)) == false);
            BOOST_TEST((symbol<int>(1) == symbol<int>(1)) == true);
        }

    BOOST_AUTO_TEST_SUITE_END()

    BOOST_AUTO_TEST_SUITE(TSymbolName_is_std_string)

        BOOST_AUTO_TEST_CASE(construction_from_std_string)
        {
            symbol<std::string> s(std::string("a"));
            BOOST_TEST("a" == s.name);
        }

        BOOST_AUTO_TEST_CASE(construction_from_c_string)
        {
            symbol<std::string> s("bcd");
            BOOST_TEST("bcd" == s.name);
        }

        BOOST_AUTO_TEST_CASE(operator_less_than)
        {
            BOOST_TEST((symbol<std::string>("b") < symbol<std::string>("a")) == false);
            BOOST_TEST((symbol<std::string>("b") < symbol<std::string>("b")) == false);
            BOOST_TEST((symbol<std::string>("b") < symbol<std::string>("c")) == true);
        }

        BOOST_AUTO_TEST_CASE(operator_equal_to)
        {
            BOOST_TEST((symbol<std::string>("b") == symbol<std::string>("a")) == false);
            BOOST_TEST((symbol<std::string>("b") == symbol<std::string>("b")) == true);
        }

    BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()

}
