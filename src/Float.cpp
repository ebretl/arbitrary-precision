#include <arbitrary_precision/Float.h>

namespace ap {

Float::Float(const Integer& N, uint32_t pow10) : N(N), pow10(pow10) {}

Float::Float() : Float(0, 0) {}

std::string Float::PrintDecimal() const {
  std::string out = N.PrintDecimal();
  if (pow10 >= out.size()) {
    out.insert(out.begin(), pow10 - out.size() + 1, '0');
  }
  out.insert(out.end() - pow10, '.');
  return out;
}

int Float::Compare(const Float &other) const {
  return N.Compare(other.N);
}

Float Float::operator+(const Float &other) const {
  Float out;
  out.N = N + other.N;
  out.pow10 = pow10;
  return out;
}

Float Float::operator-(const Float &other) const {
  Float out;
  out.N = N - other.N;
  out.pow10 = pow10;
  return out;
}

Float Float::operator*(const Float &other) const {
  Float out;
  out.N = N * other.N;
  out.pow10 = pow10;
  return out;
}

Float Float::operator/(const Float &other) const {
  Float out;
  out.N = (N * Integer(10).Pow(pow10) / other.N);
  out.pow10 = pow10;
  return out;
}

Float Float::operator%(const Float &other) const {
  Float out;
  out.N = N % other.N;
  out.pow10 = pow10;
  return out;
}

uint32_t Float::getPow10() const {
  return pow10;
}

void Float::setPow10(uint32_t p) {
  if (p > pow10) {
    N = N * Integer(10).Pow(p - pow10);
  } else if (p < pow10) {
    N = N / Integer(10).Pow(pow10 - p);
  }
  pow10 = p;
}

Float::Factory::Factory(uint32_t precision) {
  scale = Integer(10).Pow(precision);
  pow10 = precision;
}

Float Float::Factory::operator () (const Integer& x) const {
  return Float(x * scale, pow10);
}

}  // namespace ap
