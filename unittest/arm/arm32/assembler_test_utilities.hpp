// SPDX-FileCopyrightText: 2021 Thomas Mathys
// SPDX-License-Identifier: MIT
// lzasm: a runtime assembler

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
