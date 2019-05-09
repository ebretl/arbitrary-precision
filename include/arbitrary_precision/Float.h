#pragma once

#include "Number.hpp"
#include "Integer.h"

namespace ap {

class Float : public Number<Float, double> {
public:
  Float();
  Float(const Integer& N, uint32_t pow10);

  struct Factory {
    Integer scale;
    uint32_t pow10;

    Factory(uint32_t);
    Float operator () (const Integer& x) const;
  };

  std::string PrintDecimal() const override;

  int Compare(const Float &t) const override;

  Float operator+(const Float &t) const override;
  Float operator-(const Float &t) const override;
  Float operator*(const Float &t) const override;
  Float operator/(const Float &t) const override;
  Float operator%(const Float &t) const override;

  uint32_t getPow10() const;
  void setPow10(uint32_t pow10);

private:
  Integer N;
  uint32_t pow10;
};

}  // namespace ap
