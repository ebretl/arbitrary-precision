#pragma once

#include "non_negative_integer.h"


namespace exact {

class Integer {
 private:
  NonNegativeInteger magnitude_;  // magnitude storage (base 2^64)
  bool sign_;                     // true for negative

 public:
  Integer();
  Integer(int initial);
  Integer(const NonNegativeInteger& other);

  std::string DecimalString() const;
  friend std::ostream& operator<<(std::ostream& stream,
                                  const NonNegativeInteger& n);

  bool operator==(const Integer& t) const;
  bool operator!=(const Integer& t) const;
  bool operator<(const Integer& t) const;
  bool operator<=(const Integer& t) const;
  bool operator>(const Integer& t) const;
  bool operator>=(const Integer& t) const;

  friend Integer operator+(const Integer& a, const Integer& b);
  friend Integer operator-(const Integer& a, const Integer& b);
  friend Integer operator*(const Integer& a, const Integer& b);
  friend Integer operator/(const Integer& a, const Integer& b);
  friend std::pair<Integer, Integer> DivMod(const Integer& n, const Integer& d);

  Integer& operator+=(const Integer& t);
  Integer& operator-=(const Integer& t);
  Integer& operator*=(const Integer& t);
  Integer& operator/=(const Integer& t);
};

}  // namespace exact
