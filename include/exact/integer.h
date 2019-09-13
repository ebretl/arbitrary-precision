#pragma once

#include "non_negative_integer.h"


namespace exact {

class Integer {
 public:
  Integer();
  Integer(int initial);
  Integer(const NonNegativeInteger& other);
  Integer(const NonNegativeInteger& other, bool sign);

  EXACT_DECLARE_PRINT_OPERATORS(Integer)

  EXACT_DECLARE_COMPARISON_OPERATORS(Integer)

  EXACT_DECLARE_ARITHMETIC_OPERATORS(Integer)

 private:
  NonNegativeInteger magnitude_;  // magnitude storage
  bool sign_;                     // true for negative
};

}  // namespace exact
