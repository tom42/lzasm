// SPDX-FileCopyrightText: 2021 Thomas Mathys
// SPDX-License-Identifier: MIT
// lzasm: a runtime assembler

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

#if defined(__CYGWIN__)
#define _POSIX_C_SOURCE 200112L
#endif

#if defined(__clang__) && (defined(_WIN32) || defined(_WIN64))
#define _CRT_SECURE_NO_WARNINGS
#endif

#define BOOST_TEST_MODULE divided_thumb_assembler-unittest
#include <boost/test/included/unit_test.hpp>
