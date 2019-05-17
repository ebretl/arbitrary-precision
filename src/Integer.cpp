#include <arbitrary_precision/Integer.h>

namespace ap {

Integer::Integer(int32_t x) {
  sign = (x < 0);
  mag = UnsignedInteger(std::abs(x));
}

Integer::Integer() : Integer(0) {}

Integer::Integer(const Integer& other) : mag(other.mag), sign(other.sign) {}

std::string Integer::PrintDecimal() const {
  return std::string(sign ? "-" : "") + mag.PrintDecimal();
}

int Integer::Compare(const Integer& other) const {
  if (!sign && !other.sign) {
    // + +
    return mag.Compare(other.mag);
  } else if (sign && other.sign) {
    // - -
    return mag.Compare(other.mag) * -1;
  } else if (!sign && other.sign) {
    // + -
    return (mag == 0 && other.mag == 0) ? 0 : 1;
  } else {
    // - +
    return (mag == 0 && other.mag == 0) ? 0 : -1;
  }
}


Integer Integer::operator+(const Integer &other) const {
  Integer out;
  if (sign == other.sign) {
    out.mag = mag + other.mag;
    out.sign = sign;
  } else {
    bool flip_order = mag < other.mag;
    if (flip_order) {
      out.mag = other.mag - mag;
    } else {
      out.mag = mag - other.mag;
    }

    out.sign = (!flip_order && sign) || (flip_order && !sign);
    out.sign &= (out.mag != 0);
  }

  return out;
}

Integer Integer::operator-(const Integer &other) const {
  Integer flipped = other;
  flipped.sign = !other.sign;
  return *this + flipped;
}

Integer Integer::operator*(const Integer &other) const {
  Integer out;
  out.mag = mag * other.mag;
  out.sign = (sign != other.sign);
  return out;
}

Integer Integer::operator/(const Integer &other) const {
  Integer out;
  out.mag = mag / other.mag;
  out.sign = (sign != other.sign);
  return out;
}

Integer Integer::operator%(const Integer &other) const {
  Integer out;
  out.mag = mag % other.mag;
  out.sign = sign;
  return out;
}

Integer Integer::Pow(Integer p) const {
  Integer out = *this;
  // std::cout << "Pow " << p << std::endl;

  while (p > 1) {
    // std::cout << p << std::endl;
    out = out * (*this);
    p = p - 1;
  }

  while (p < 1 && out != 0) {
    out = out / (*this);
    p = p + 1;
  }

  return out;
}

}  // namespace ap
