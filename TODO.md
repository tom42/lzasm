# Things to do
* Version 0.4.0
  * Might have to introduce an arm_push directive. Obviously this is getting out of hands, but I need it for shrinkler-gba.
* Handle address overflows
  * In `object::emit8`: when the object size exceeds 4GB
  * Additions and subtractions in `object::get_relative_address`
  * Addition in `object::get_value`
