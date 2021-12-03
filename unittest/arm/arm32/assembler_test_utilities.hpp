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

#ifndef LZASM_ASSEMBLER_TEST_UTILITIES_HPP_INCLUDED
#define LZASM_ASSEMBLER_TEST_UTILITIES_HPP_INCLUDED

#include <boost/test/unit_test.hpp>
#include <stdexcept>
#include "lzasm/arm/arm32/divided_thumb_assembler.hpp"
#include "test_utilities.hpp"

namespace lzasm_unittest
{

#define CHECK_PROGRAM(assembler, origin, ...)                                   \
{                                                                               \
    /* Link and verify (assumes little endian ARM) */                           \
    auto program = assembler.link(origin);                                      \
    auto expected_bytes = to_bytevector(__VA_ARGS__);                           \
    BOOST_TEST(program == expected_bytes, boost::test_tools::per_element());    \
}

// Checks whether an instruction can be successfully assembled
#define CHECK(instruction, expected_data)           \
{                                                   \
    ::lzasm::arm::arm32::divided_thumb_assembler a; \
    a.instruction;                                  \
    CHECK_PROGRAM(a, 0, expected_data);             \
}

// Checks whether attempting to assemble an instruction throws an exception
#define CHECK_THROWS(assembly_code, exception_predicate, ...)                           \
{                                                                                       \
    ::lzasm::arm::arm32::divided_thumb_assembler a;                                     \
    BOOST_REQUIRE_EXCEPTION(a.assembly_code, std::runtime_error, exception_predicate);  \
    CHECK_PROGRAM(a, 0, __VA_ARGS__);                                                   \
}

#define CHECK_LINK_THROWS(assembler, origin, exception_predicate)                               \
{                                                                                               \
    BOOST_REQUIRE_EXCEPTION(assembler.link(origin), std::runtime_error, exception_predicate);   \
}

void space(::lzasm::arm::arm32::divided_thumb_assembler& a, int nbytes);

}

#endif
