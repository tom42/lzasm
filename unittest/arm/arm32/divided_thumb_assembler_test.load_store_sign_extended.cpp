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
#include "lzasm/arm/arm32/divided_thumb_assembler.hpp"
#include "assembler_test_utilities.hpp"
#include "test_utilities.hpp"

namespace lzasm_unittest
{

using namespace ::lzasm::arm::arm32;

BOOST_AUTO_TEST_SUITE(divided_thumb_assembler_test)

    BOOST_AUTO_TEST_SUITE(load_store_sign_extended)

        BOOST_AUTO_TEST_CASE(strh)
        {
            CHECK(strh(r0, r1, r2), H(0x5288));
            CHECK(strh(r1, r2, r3), H(0x52d1));
            CHECK(strh(r2, r3, r4), H(0x531a));
            CHECK(strh(r3, r4, r5), H(0x5363));
            CHECK(strh(r4, r5, r6), H(0x53ac));
            CHECK(strh(r5, r6, r7), H(0x53f5));
            CHECK(strh(r6, r7, r0), H(0x523e));
            CHECK(strh(r7, r0, r1), H(0x5247));
        }

        BOOST_AUTO_TEST_CASE(ldrh)
        {
            CHECK(ldrh(r0, r1, r2), H(0x5a88));
            CHECK(ldrh(r1, r2, r3), H(0x5ad1));
            CHECK(ldrh(r2, r3, r4), H(0x5b1a));
            CHECK(ldrh(r3, r4, r5), H(0x5b63));
            CHECK(ldrh(r4, r5, r6), H(0x5bac));
            CHECK(ldrh(r5, r6, r7), H(0x5bf5));
            CHECK(ldrh(r6, r7, r0), H(0x5a3e));
            CHECK(ldrh(r7, r0, r1), H(0x5a47));
        }

        BOOST_AUTO_TEST_CASE(ldrsb)
        {
            CHECK(ldrsb(r0, r1, r2), H(0x5688));
            CHECK(ldrsb(r1, r2, r3), H(0x56d1));
            CHECK(ldrsb(r2, r3, r4), H(0x571a));
            CHECK(ldrsb(r3, r4, r5), H(0x5763));
            CHECK(ldrsb(r4, r5, r6), H(0x57ac));
            CHECK(ldrsb(r5, r6, r7), H(0x57f5));
            CHECK(ldrsb(r6, r7, r0), H(0x563e));
            CHECK(ldrsb(r7, r0, r1), H(0x5647));
        }

        BOOST_AUTO_TEST_CASE(ldrsh)
        {
            CHECK(ldrsh(r0, r1, r2), H(0x5e88));
            CHECK(ldrsh(r1, r2, r3), H(0x5ed1));
            CHECK(ldrsh(r2, r3, r4), H(0x5f1a));
            CHECK(ldrsh(r3, r4, r5), H(0x5f63));
            CHECK(ldrsh(r4, r5, r6), H(0x5fac));
            CHECK(ldrsh(r5, r6, r7), H(0x5ff5));
            CHECK(ldrsh(r6, r7, r0), H(0x5e3e));
            CHECK(ldrsh(r7, r0, r1), H(0x5e47));
        }

    BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()

}
