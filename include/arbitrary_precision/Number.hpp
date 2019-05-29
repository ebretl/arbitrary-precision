#pragma once

#include <iostream>
#include <tuple>


#define EXTERNAL_OP(RET, OP) \
  friend RET operator OP (Primitive x, const T& other) { return T(x) OP other; }

#define COMPARATOR(OP) \
  bool operator OP (const T& other) const { return Compare(other) OP 0; } \
  EXTERNAL_OP(bool, OP)

#define MATH_OP_INTERNAL(OP) \
  virtual void operator OP (const T&) = 0;


namespace ap {

// T should subclass this Number interface. It's self-referential but works
template <typename T, typename Primitive>
class Number {
public:

  // ************** pure virtual interface **************
  virtual std::string Print() const = 0;

  virtual int Compare(const T&) const = 0;  // positive for >, 0 for ==, -1 for <

  virtual std::tuple<T, T> DivMod(const T&) const = 0;  // quotient and remainder

  virtual T Pow(const T&) const = 0;

  MATH_OP_INTERNAL(+=)
  MATH_OP_INTERNAL(-=)
  MATH_OP_INTERNAL(*=)

  virtual void LeftShift(const T&) = 0;
  virtual void RightShift(const T&) = 0;


  // ************** generic implementations **************
  friend std::ostream& operator<<(std::ostream& stream, const T& t) {
    return stream << t.Print();
  }

  // these generate comparison operator definitions and do not need to be overridden (just implement Compare)
  COMPARATOR(<)
  COMPARATOR(<=)
  COMPARATOR(>)
  COMPARATOR(>=)
  COMPARATOR(==)
  COMPARATOR(!=)

  T operator+(const T& t) const {
    auto out = reinterpret_cast<const T&>(*this);
    out += t;
    return out;
  }
  EXTERNAL_OP(T, +);

  T operator-(const T& t) const {
    auto out = reinterpret_cast<const T&>(*this);
    out -= t;
    return out;
  }
  EXTERNAL_OP(T, -);

  T operator*(const T& t) const {
    auto out = reinterpret_cast<const T&>(*this);
    out *= t;
    return out;
  }
  EXTERNAL_OP(T, *);

  T operator/(const T& t) const {
    return std::get<0>(DivMod(t));
  }
  EXTERNAL_OP(T, /);

  void operator/=(const T& t) {
    reinterpret_cast<T&>(*this) = *this / t;
  }

  T operator%(const T& t) const {
    return std::get<1>(DivMod(t));
  }
  EXTERNAL_OP(T, %);

  T operator<<(const T& t) const {
    auto out = reinterpret_cast<const T&>(*this);
    out.LeftShift(t);
    return out;
  }

  T operator>>(const T& t) const {
    auto out = reinterpret_cast<const T&>(*this);
    out.RightShift(t);
    return out;
  }

  T& operator++() {
    *this += 1;
    return reinterpret_cast<T&>(*this);
  }

  const T operator++(int) {
    auto out = reinterpret_cast<const T&>(*this);
    ++*this;
    return out;
  }

  T& operator--() {
    *this -= 1;
    return reinterpret_cast<const T&>(*this);
  }

  const T operator--(int) {
    auto out = reinterpret_cast<const T&>(*this);
    --*this;
    return out;
  }
};

}  // namespace ap
