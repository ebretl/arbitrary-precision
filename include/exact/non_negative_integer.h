#pragma once

#include <cstdint>
#include <deque>

namespace exact {

class NonNegativeInteger {
 public:
  NonNegativeInteger();
  NonNegativeInteger(unsigned int initial);

  bool operator==(const NonNegativeInteger& t) const;
  bool operator!=(const NonNegativeInteger& t) const;
  bool operator<(const NonNegativeInteger& t) const;
  bool operator<=(const NonNegativeInteger& t) const;
  bool operator>(const NonNegativeInteger& t) const;
  bool operator>=(const NonNegativeInteger& t) const;

  friend NonNegativeInteger operator+(const NonNegativeInteger& a,
                                      const NonNegativeInteger& b);
  friend NonNegativeInteger operator-(const NonNegativeInteger& a,
                                      const NonNegativeInteger& b);
  friend NonNegativeInteger operator*(const NonNegativeInteger& a,
                                      const NonNegativeInteger& b);
  friend NonNegativeInteger operator/(const NonNegativeInteger& a,
                                      const NonNegativeInteger& b);
  friend std::pair<NonNegativeInteger, NonNegativeInteger> DivMod(
      const NonNegativeInteger& n, const NonNegativeInteger& d);

  NonNegativeInteger& operator+=(const NonNegativeInteger& t);
  NonNegativeInteger& operator-=(const NonNegativeInteger& t);
  NonNegativeInteger& operator*=(const NonNegativeInteger& t);
  NonNegativeInteger& operator/=(const NonNegativeInteger& t);


 private:
  std::deque<uint32_t> data_;  // magnitude storage (base 2^32)

  friend int compare(const NonNegativeInteger& a, const NonNegativeInteger& b);
  void Trim();
};

}  // namespace exact
