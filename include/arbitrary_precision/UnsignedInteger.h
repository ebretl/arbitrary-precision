#pragma once

#include <string>
#include <vector>
#include <tuple>

#include "Number.hpp"

namespace ap {

class UnsignedInteger : public Number<UnsignedInteger, uint32_t> {
public:
  UnsignedInteger();
  UnsignedInteger(uint32_t n);
  UnsignedInteger(const UnsignedInteger&);

  std::string Print() const override;

  UnsignedInteger operator+(const UnsignedInteger &t) const override;
  UnsignedInteger operator-(const UnsignedInteger &t) const override;
  UnsignedInteger operator*(const UnsignedInteger &t) const override;
  UnsignedInteger operator/(const UnsignedInteger &t) const override;
  UnsignedInteger operator%(const UnsignedInteger &t) const override;
  UnsignedInteger operator<<(const UnsignedInteger &t) const override;
  UnsignedInteger operator>>(const UnsignedInteger &t) const override;

  int Compare(const UnsignedInteger &t) const override;

  std::tuple<UnsignedInteger, UnsignedInteger> DivMod(const UnsignedInteger&) const;

  UnsignedInteger Pow(const UnsignedInteger&) const;

private:
  std::vector<uint8_t> data_;

  void Trim();

  UnsignedInteger LongMultiply(const UnsignedInteger&) const;
  static UnsignedInteger Karatsuba(UnsignedInteger&, UnsignedInteger&);
  static UnsignedInteger _mult_impl(UnsignedInteger&, UnsignedInteger&);
};

}  // namespace ap
