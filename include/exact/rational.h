#pragma once

#include "integer.h"
#include "type_utils.h"
#include "positive_integer.h"

namespace exact {

class Rational {
 public:
  Rational();
  Rational(int n, int d);
  Rational(const Integer& n, const PositiveInteger& d);
  Rational(int n);
  Rational(const Integer& n);

  EXACT_DECLARE_PRINT_OPERATORS(Rational)

  EXACT_DECLARE_COMPARISON_OPERATORS(Rational)

  EXACT_DECLARE_ARITHMETIC_OPERATORS(Rational)

 private:
  Integer N;
  PositiveInteger D;
};

}  // namespace exact
