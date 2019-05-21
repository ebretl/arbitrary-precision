#pragma once

#include "Number.hpp"
#include "Integer.h"

namespace ap {

template <uint32_t digits>
class Float : public Number<Float<digits>, uint32_t> {
public:
  Float(const Integer& N) : N(N << digits) {}
  Float(const uint32_t n) : Float(Integer(n)) {}
  Float() : Float(Integer(0)) {}

  std::string Print() const override {
    std::string out = N.Print();
    if (digits >= out.size()) {
      out.insert(out.begin(), digits - out.size() + 1, '0');
    }
    out.insert(out.end() - digits, '.');
    return out;
  }

  int Compare(const Float &other) const override {
    return N.Compare(other.N);
  }

  Float operator+(const Float &t) const override {
    Float out;
    out.N = N + t.N;
    return out;
  }

  Float operator-(const Float &t) const override {
    Float out;
    out.N = N - t.N;
    return out;
  }

  Float operator*(const Float &t) const override {
    Float out;
    out.N = (N * t.N) >> digits;
    return out;
  }

  Float operator/(const Float &t) const override {
    Float out;
    out.N = (N << digits) / t.N;
    return out;
  }

  Float operator%(const Float &t) const override {
    Float out;
    out.N = N % t.N;
    return out;
  }

  Float operator<<(const Float &t) const override {
    Float out;
    out.N = N << t.N;
    return out;
  }

  Float operator>>(const Float &t) const override {
    Float out;
    out.N = N >> t.N;
    return out;
  }

private:
  Integer N;
};

}  // namespace ap
