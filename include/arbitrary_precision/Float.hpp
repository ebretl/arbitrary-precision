#pragma once

#include <vector>

#include "Integer.h"
#include "Number.hpp"

namespace ap {

template <uint32_t digits>
class Float : public Number<Float<digits>, uint32_t> {
 public:
  Float(const Integer &N) : N(N << digits) {}
  Float(const int n) : Float(Integer(n)) {}
  Float() : Float(Integer(0)) {}

  std::string Print() const override {
    std::string out = N.Print();

    bool neg = (out[0] == '-');
    if (neg) {
      out = out.substr(1, out.size());
    }

    if (digits >= out.size()) {
      out.insert(out.begin(), digits - out.size() + 1, '0');
    }
    out.insert(out.end() - digits, '.');

    if (neg) {
      out.insert(out.begin(), '-');
    }

    return out;
  }

  int Compare(const Float &other) const override { return N.Compare(other.N); }

  void operator+=(const Float &t) override { N += t.N; }

  void operator-=(const Float &t) override { N -= t.N; }

  void operator*=(const Float &t) override {
    N *= t.N;
    N.RightShift(digits);
  }

  Float Pow(const Integer &t) const {
    Float out = (t == 0) ? 1 : *this;
    std::vector<char> stack;

    Integer counter = t;

    while (counter > 1) {
      auto [q, r] = counter.DivMod(2);

      if (r == 0) {
        stack.push_back(2);
        counter = std::move(q);
      } else {
        stack.push_back(1);
        counter -= 1;
      }
    }

    while (!stack.empty()) {
      auto &op = stack.back();
      if (op == 1) {
        out *= *this;
      } else {
        out *= out;
      }
      stack.pop_back();
    }

    return out;
  }

  Float Pow(const Float &t) const { return Pow(t.N >> digits); }

  Float Pow(const int t) const { return Pow(Integer(t)); }

  std::tuple<Float, Float> DivMod(const Float &t) const override {
    auto [Q_i, R_i] = (N << digits).DivMod(t.N);
    std::tuple<Float, Float> out;
    std::get<0>(out).N = Q_i;
    std::get<1>(out).N = R_i;
    return out;
  }

  void LeftShift(const Float &t) override { N.LeftShift(t.N >> digits); }

  void RightShift(const Float &t) override { N.RightShift(t.N >> digits); }

 private:
  Integer N;
};

}  // namespace ap
