#pragma once

#include "Number.hpp"
#include "Integer.h"

namespace ap {

template <uint32_t digits>
class Float : public Number<Float<digits>, uint32_t> {
public:
  Float(const Integer& N) : N(N << digits) {}
  Float(const int n) : Float(Integer(n)) {}
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

  void operator+=(const Float &t) override {
    N += t.N;
  }

  void operator-=(const Float &t) override {
    N -= t.N;
  }

  void operator*=(const Float &t) override {
    N *= t.N;
    N.RightShift(digits);
  }

  Float Pow(const Float &t) const {
    return Float(N.Pow(t.N));
  }

  std::tuple<Float, Float> DivMod(const Float &t) const override {
    auto [Q_i, R_i] = (N << digits).DivMod(t.N);
    std::tuple<Float, Float> out;
    std::get<0>(out).N = Q_i;
    std::get<1>(out).N = R_i;
    return out;
  }

  void LeftShift(const Float &t) override {
    N.LeftShift(t.N >> digits);
  }

  void RightShift(const Float &t) override {
    N.RightShift(t.N >> digits);
  }

private:
  Integer N;
};

}  // namespace ap
