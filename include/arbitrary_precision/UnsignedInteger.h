#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <tuple>

#include "Number.hpp"

namespace ap {

class UnsignedInteger : public Number<UnsignedInteger, uint32_t> {
public:
  UnsignedInteger();
  UnsignedInteger(uint32_t n);

  std::string PrintDecimal() const override;
  std::string PrintRaw() const;

  UnsignedInteger operator+(const UnsignedInteger &t) const override;
  UnsignedInteger operator-(const UnsignedInteger &t) const override;
  UnsignedInteger operator*(const UnsignedInteger &t) const override;
  UnsignedInteger operator/(const UnsignedInteger &t) const override;
  UnsignedInteger operator%(const UnsignedInteger &t) const override;

  int Compare(const UnsignedInteger &t) const override;

  std::tuple<UnsignedInteger, UnsignedInteger> DivMod(const UnsignedInteger &t) const;
  UnsignedInteger ByteShifted(int32_t shift) const;
  UnsignedInteger Pow(UnsignedInteger) const;

  bool GetBit(uint32_t bit) const;
  void SetBit(uint32_t b, bool val);

private:
  std::vector<uint8_t> data_;

  void Trim();
};

}  // namespace ap
