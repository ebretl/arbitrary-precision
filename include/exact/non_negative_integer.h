#pragma once

#include <cstdint>
#include <deque>
#include <iostream>

namespace exact {

class NonNegativeInteger {
 public:
  NonNegativeInteger();
  NonNegativeInteger(unsigned int initial);

  std::string DecimalString() const;
  friend std::ostream& operator<<(std::ostream& stream,
                                  const NonNegativeInteger& n);

  bool operator==(const NonNegativeInteger& t) const;
  bool operator!=(const NonNegativeInteger& t) const;
  bool operator<(const NonNegativeInteger& t) const;
  bool operator<=(const NonNegativeInteger& t) const;
  bool operator>(const NonNegativeInteger& t) const;
  bool operator>=(const NonNegativeInteger& t) const;

  NonNegativeInteger operator+(const NonNegativeInteger& t) const;
  NonNegativeInteger operator-(const NonNegativeInteger& t) const;
  NonNegativeInteger operator*(const NonNegativeInteger& t) const;
  NonNegativeInteger operator/(const NonNegativeInteger& t) const;
  std::pair<NonNegativeInteger, NonNegativeInteger> DivMod(
      const NonNegativeInteger& d) const;

  NonNegativeInteger& operator+=(const NonNegativeInteger& t);
  NonNegativeInteger& operator-=(const NonNegativeInteger& t);
  NonNegativeInteger& operator*=(const NonNegativeInteger& t);
  NonNegativeInteger& operator/=(const NonNegativeInteger& t);

  void operator<<=(unsigned int bits);
  void operator>>=(unsigned int bits);
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
