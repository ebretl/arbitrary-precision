#pragma once

#include "type_utils.h"
#include "unsigned_integer.h"

namespace exact {

class PositiveInteger {
 public:
  UnsignedInteger magnitude;  // magnitude storage

  PositiveInteger() = delete;
  PositiveInteger(uint32_t initial);
  PositiveInteger(int32_t initial);
  PositiveInteger(const UnsignedInteger& other);

  EXACT_DECLARE_PRINT_OPERATORS(PositiveInteger)

  EXACT_DECLARE_COMPARISON_OPERATORS(PositiveInteger)

  EXACT_DECLARE_ARITHMETIC_OPERATORS(PositiveInteger)
};

}  // namespace exact
