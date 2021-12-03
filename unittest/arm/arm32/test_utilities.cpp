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
#include "test_utilities.hpp"

namespace lzasm_unittest
{

bytevector to_bytevector()
{
    return bytevector();
}

bytevector to_bytevector(const bytevector& bytes)
{
    return bytes;
}

bytevector to_bytevector(const hwordvector& halfwords)
{
    bytevector bytes;
    for (auto halfword : halfwords)
    {
        bytes.push_back(halfword & 255);
        bytes.push_back((halfword >> 8) & 255);
    }
    return bytes;
}

bytevector to_bytevector(const wordvector& words)
{
    bytevector bytes;
    for (auto word : words)
    {
        bytes.push_back(word & 255);
        bytes.push_back((word >> 8) & 255);
        bytes.push_back((word >> 16) & 255);
        bytes.push_back((word >> 24) & 255);
    }
    return bytes;
}

bool is_alignment_out_of_range(const std::exception& e)
{
    BOOST_CHECK_EQUAL("Alignment out of range", e.what());
    return true;
}

bool is_immediate_out_of_range(const std::exception& e)
{
    BOOST_CHECK_EQUAL("Immediate value is out of range", e.what());
    return true;
}

bool is_misaligned_immediate_value(const std::exception& e)
{
    BOOST_CHECK_EQUAL("Misaligned immediate value", e.what());
    return true;
}

bool is_origin_too_large(const std::exception& e)
{
    BOOST_CHECK_EQUAL("Origin too large", e.what());
    return true;
}

bool is_symbol_already_defined(const std::exception& e)
{
    BOOST_CHECK_EQUAL("Symbol is already defined", e.what());
    return true;
}

bool is_undefined_symbol(const std::exception& e)
{
    BOOST_CHECK_EQUAL("Undefined symbol", e.what());
    return true;
}

bool is_unpredictable_behavior(const std::exception& e)
{
    BOOST_CHECK_EQUAL("Unpredictable behavior", e.what());
    return true;
}

}
