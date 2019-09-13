#pragma once

#include <cstdint>
#include <deque>
#include <iostream>

#include "type_utils.h"

namespace exact {

class NonNegativeInteger {
 public:
  NonNegativeInteger();
  NonNegativeInteger(unsigned int initial);

  EXACT_DECLARE_PRINT_OPERATORS(NonNegativeInteger)

  EXACT_DECLARE_COMPARISON_OPERATORS(NonNegativeInteger)

  EXACT_DECLARE_ARITHMETIC_OPERATORS(NonNegativeInteger)

  NonNegativeInteger& operator<<=(unsigned int bits);
  NonNegativeInteger& operator>>=(unsigned int bits);
  NonNegativeInteger operator<<(unsigned int bits);
  NonNegativeInteger operator>>(unsigned int bits);

 private:
  std::deque<uint32_t> data_;  // magnitude storage (base 2^32)

  void Trim();

  friend int compare(const NonNegativeInteger& a, const NonNegativeInteger& b);
  friend NonNegativeInteger _long_multiply(const NonNegativeInteger& a,
                                           const NonNegativeInteger& b);
  friend NonNegativeInteger _karatsuba(const NonNegativeInteger& a,
                                       const NonNegativeInteger& b);
};

}  // namespace exact
