# Things to do
* Version 0.3.0
  * Give divided_thumb_assembler a virtual destructor
    * This allows for shorthand notation when deriving a special assembler, e.g. mov(r0, r1) instead of a.mov(r0, r1)
    * Update documentation: just a short section on that, leave most of documentation in current state, using using a.xxx() syntax
* Handle address overflows
  * In `object::emit8`: when the object size exceeds 4GB
  * Additions and subtractions in `object::get_relative_address`
  * Addition in `object::get_value`
