#pragma once

#include "non_negative_integer.h"
#include "positive_integer.h"
#include "type_utils.h"

namespace exact {

class Integer {
 public:
  Integer();
  Integer(int32_t initial);
  Integer(const NonNegativeInteger& other);
  Integer(const NonNegativeInteger& other, bool sign);
  Integer(const PositiveInteger& other);

  EXACT_DECLARE_PRINT_OPERATORS(Integer)

  EXACT_DECLARE_COMPARISON_OPERATORS(Integer)

  EXACT_DECLARE_ARITHMETIC_OPERATORS(Integer)

  friend const NonNegativeInteger& abs(const Integer& t);

 private:
  NonNegativeInteger magnitude_;  // magnitude storage
  bool sign_;                     // true for negative
};

}  // namespace exact
