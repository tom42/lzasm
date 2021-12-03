# Things to do
* Handle address overflows
  * In `object::emit8`: when the object size exceeds 4GB
  * Additions and subtractions in `object::get_relative_address`
  * Addition in `object::get_value`
