#pragma once

#include <iostream>
#include <tuple>


#define EXTERNAL_OP(RET, OP) \
  friend RET operator OP (Primitive x, const T& other) { return T(x) OP other; }

#define COMPARATOR(OP) \
  bool operator OP (const T& other) const { return Compare(other) OP 0; } \
  EXTERNAL_OP(bool, OP)

#define MATH_OP(OP) \
  virtual T operator OP (const T&) const = 0; \
  EXTERNAL_OP(T, OP)

namespace ap {

template <typename T, typename Primitive>
class Number {
public:
  virtual std::string PrintDecimal() const = 0;
  friend std::ostream& operator<<(std::ostream& stream, const T& N) { return stream << N.PrintDecimal(); }

  virtual int Compare(const T&) const = 0;

  COMPARATOR(<)
  COMPARATOR(<=)
  COMPARATOR(>)
  COMPARATOR(>=)
  COMPARATOR(==)
  COMPARATOR(!=)

  MATH_OP(+)
  MATH_OP(-)
  MATH_OP(*)
  MATH_OP(/)
  MATH_OP(%)
};

}  // namespace ap
