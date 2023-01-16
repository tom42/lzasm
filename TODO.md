# Things to do
* Version 0.4.0
  * Use SPDX:
    * Use SPDX file tags for license and copyright everywhere. Keep the 'lzasm: a runtime assembler' line around, though.
      Eg:
      // SPDX-FileCopyrightText: 2021 Thomas Mathys
      // SPDX-License-Identifier: MIT
      // lzasm: a runtime assembler
    * Consider using reuse: https://reuse.software/tutorial/
  * Might have to introduce an arm_push directive. Obviously this is getting out of hands, but I need it for shrinkler-gba.
* Handle address overflows
  * In `object::emit8`: when the object size exceeds 4GB
  * Additions and subtractions in `object::get_relative_address`
  * Addition in `object::get_value`
