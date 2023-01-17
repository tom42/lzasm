// SPDX-FileCopyrightText: 2021 Thomas Mathys
// SPDX-License-Identifier: MIT
// lzasm: a runtime assembler

#include "assembler_test_utilities.hpp"

namespace lzasm_unittest
{

void space(::lzasm::arm::arm32::divided_thumb_assembler& a, int nbytes)
{
    for (int i = 0; i < nbytes; ++i)
    {
        a.byte(0);
    }
}

}
