<!--
SPDX-FileCopyrightText: 2021 Thomas Mathys
SPDX-License-Identifier: MIT
lzasm: a runtime assembler
-->

# Things to do
* Version 0.4.0
  * Might have to introduce an arm_push directive. Obviously this is getting out of hands, but I need it for shrinkler-gba.
  * Improve build/release automation. For starters
    * Automatically build and run tests (what operating systems? what compilers?)
    * Run `reuse lint` to ensure we still comply with the REUSE specification
* Handle address overflows
  * In `object::emit8`: when the object size exceeds 4GB
  * Additions and subtractions in `object::get_relative_address`
  * Addition in `object::get_value`
