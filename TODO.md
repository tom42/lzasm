<!--
SPDX-FileCopyrightText: 2021 Thomas Mathys
SPDX-License-Identifier: MIT
lzasm: a runtime assembler
-->

# Things to do
* Version 0.4.0
  * Improve build/release automation. For starters
    * Automatically build and run tests (what operating systems? what compilers?)
    * Run `reuse lint` to ensure we still comply with the REUSE specification
* Wish list, no particular version
  * Better exception messages
    * Include symbol name if applicable (e.g. "Undefined symbol 'symbol name'" rather than just "Undefined symbol")
    * Include position information if applicable (location counter / fixup location)
  * Local labels
  * A way to refer to the current location counter, so that one can e.g. code an infinite loop like this `br(".")`
    * Problem: how to refer to the current location counter in a generic way, e.g. when symbols are not strings?
    * We'd like this to be relocatable too.
* Handle address overflows
  * In `object::emit8`: when the object size exceeds 4GB
  * Additions and subtractions in `object::get_relative_address`
  * Addition in `object::get_value`
