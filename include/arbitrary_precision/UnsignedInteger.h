#pragma once

#include <deque>
#include <string>
#include <tuple>

#include "Number.hpp"

namespace ap {

class UnsignedInteger : public Number<UnsignedInteger, uint32_t> {
 public:
  UnsignedInteger();
  UnsignedInteger(uint32_t n);

  std::string Print() const override;
  int Compare(const UnsignedInteger &t) const override;
  std::tuple<UnsignedInteger, UnsignedInteger> DivMod(
      const UnsignedInteger &) const override;
  UnsignedInteger Pow(const UnsignedInteger &) const override;

  void operator+=(const UnsignedInteger &t) override;
  void operator-=(const UnsignedInteger &t) override;
  void operator*=(const UnsignedInteger &t) override;
  void LeftShift(const UnsignedInteger &t) override;
  void RightShift(const UnsignedInteger &t) override;

 private:
  std::deque<uint8_t> data_;

  void Trim();

  void LongMultiply(UnsignedInteger &);
  void Karatsuba(UnsignedInteger &);
  void _mult_impl(UnsignedInteger &);
};

}  // namespace ap
