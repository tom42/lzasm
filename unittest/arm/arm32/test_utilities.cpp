// SPDX-FileCopyrightText: 2021 Thomas Mathys
// SPDX-License-Identifier: MIT
// lzasm: a runtime assembler

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
