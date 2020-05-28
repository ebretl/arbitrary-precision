#pragma once

#include "positive_integer.h"
#include "type_utils.h"
#include "unsigned_integer.h"

namespace exact {

class Integer {
 public:
  UnsignedInteger magnitude;  // magnitude storage
  bool sign;                  // true for negative

  Integer();
  Integer(int32_t initial);
  Integer(const UnsignedInteger& other);
  Integer(const UnsignedInteger& other, bool sign);
  Integer(const PositiveInteger& other);

  EXACT_DECLARE_PRINT_OPERATORS(Integer)

  EXACT_DECLARE_COMPARISON_OPERATORS(Integer)

  EXACT_DECLARE_ARITHMETIC_OPERATORS(Integer)
};

}  // namespace exact
