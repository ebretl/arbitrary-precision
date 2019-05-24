#pragma once

#include "Number.hpp"
#include "UnsignedInteger.h"

namespace ap {

class Integer : public Number<Integer, int32_t> {
public:
  Integer();
  Integer(int32_t x);
  Integer(const UnsignedInteger&);

  std::string Print() const override;
  int Compare(const Integer &t) const override;
  std::tuple<Integer, Integer> DivMod(const Integer&) const override;
  Integer Pow(const Integer&) const override;

  void operator+=(const Integer &t) override;
  void operator-=(const Integer &t) override;
  void operator*=(const Integer &t) override;
  void LeftShift(const Integer &t) override;
  void RightShift(const Integer &t) override;

private:
  UnsignedInteger mag;
  bool sign;
};

}  // namespace ap
