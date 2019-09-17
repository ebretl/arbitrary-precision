#pragma once

#include <cstdint>
#include <deque>
#include <iostream>

#include "type_utils.h"

namespace exact {

class UnsignedInteger {
 public:
  UnsignedInteger();
  UnsignedInteger(unsigned int initial);

  EXACT_DECLARE_PRINT_OPERATORS(UnsignedInteger)

  EXACT_DECLARE_COMPARISON_OPERATORS(UnsignedInteger)

  EXACT_DECLARE_ARITHMETIC_OPERATORS(UnsignedInteger)

  UnsignedInteger& operator<<=(unsigned int bits);
  UnsignedInteger& operator>>=(unsigned int bits);
  UnsignedInteger operator<<(unsigned int bits);
  UnsignedInteger operator>>(unsigned int bits);

 private:
  std::deque<uint32_t> data_;  // magnitude storage (base 2^32)

  void Trim();

  friend int compare(const UnsignedInteger& a, const UnsignedInteger& b);
  friend UnsignedInteger _long_multiply(const UnsignedInteger& a,
                                           const UnsignedInteger& b);
  friend UnsignedInteger _karatsuba(const UnsignedInteger& a,
                                       const UnsignedInteger& b);
};

}  // namespace exact
