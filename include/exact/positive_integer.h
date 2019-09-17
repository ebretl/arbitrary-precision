#pragma once

#include "non_negative_integer.h"
#include "type_utils.h"

namespace exact {

class PositiveInteger {
 public:
  PositiveInteger() = delete;
  PositiveInteger(uint32_t initial);
  PositiveInteger(int32_t initial);
  PositiveInteger(const NonNegativeInteger& other);

  EXACT_DECLARE_PRINT_OPERATORS(PositiveInteger)

  EXACT_DECLARE_COMPARISON_OPERATORS(PositiveInteger)

  EXACT_DECLARE_ARITHMETIC_OPERATORS(PositiveInteger)

  friend const NonNegativeInteger& abs(const PositiveInteger& t);

 private:
  NonNegativeInteger magnitude_;  // magnitude storage
};

}  // namespace exact
