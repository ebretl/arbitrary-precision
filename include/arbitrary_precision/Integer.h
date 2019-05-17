#pragma once

#include "Number.hpp"
#include "UnsignedInteger.h"

namespace ap {

class Integer : public Number<Integer, int32_t> {
public:
  Integer();
  Integer(int32_t x);
  Integer(const Integer&);

  std::string PrintDecimal() const override;

  int Compare(const Integer&) const override;

  Integer operator+(const Integer &t) const override;
  Integer operator-(const Integer &t) const override;
  Integer operator*(const Integer &t) const override;
  Integer operator/(const Integer &t) const override;
  Integer operator%(const Integer &t) const override;

  Integer Pow(Integer) const;

private:
  UnsignedInteger mag;
  bool sign;
};

}  // namespace ap
