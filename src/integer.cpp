#include <exact/integer.h>

#include <algorithm>

namespace exact {

Integer::Integer() : Integer(0) {}

Integer::Integer(const UnsignedInteger& other)
    : magnitude(other), sign(false) {}

Integer::Integer(const UnsignedInteger& other, bool sign)
    : magnitude(other), sign(sign) {}

Integer::Integer(int32_t initial)
    : magnitude(std::abs(initial)), sign(initial < 0) {}

Integer::Integer(const PositiveInteger& other) : magnitude(other.magnitude) {}

bool Integer::operator==(const Integer& t) const {
  if (sign == t.sign) {  // same sign
    return magnitude == t.magnitude;
  }
  return magnitude == 0;
}

bool Integer::operator!=(const Integer& t) const {
  if (sign == t.sign) {  // same sign
    return magnitude != t.magnitude;
  }
  return magnitude != 0;
}

bool Integer::operator<(const Integer& t) const {
  if (!sign && !t.sign) {  // + +
    return magnitude < t.magnitude;
  } else if (sign && t.sign) {  // - -
    return magnitude > t.magnitude;
  } else if (sign && !t.sign) {  // - +
    return (magnitude > 0) || (t.magnitude > 0);
  } else {  // + -
    return (magnitude == 0) && (t.magnitude == 0);
  }
}

bool Integer::operator<=(const Integer& t) const {
  if (!sign && !t.sign) {  // + +
    return magnitude <= t.magnitude;
  } else if (sign && t.sign) {  // - -
    return magnitude >= t.magnitude;
  } else {
    return sign && !t.sign;
  }
}

bool Integer::operator>(const Integer& t) const {
  if (!sign && !t.sign) {  // + +
    return magnitude > t.magnitude;
  } else if (sign && t.sign) {  // - -
    return magnitude < t.magnitude;
  } else if (sign && !t.sign) {  // - +
    return (magnitude > 0) || (t.magnitude > 0);
  } else {  // + -
    return (magnitude != 0) && (t.magnitude != 0);
  }
}

bool Integer::operator>=(const Integer& t) const {
  if (!sign && !t.sign) {  // + +
    return magnitude >= t.magnitude;
  } else if (sign && t.sign) {  // - -
    return magnitude <= t.magnitude;
  } else {
    return !sign && t.sign;
  }
}

Integer Integer::operator+(const Integer& t) const {
  Integer out = *this;
  out += t;
  return out;
}

Integer& Integer::operator+=(const Integer& t) {
  if (sign == t.sign) {
    magnitude += t.magnitude;
  } else {
    bool flip_order = magnitude < t.magnitude;
    if (flip_order) {
      magnitude = t.magnitude - magnitude;
      sign = t.sign;
    } else {
      magnitude -= t.magnitude;
    }

    sign = sign && (magnitude != 0);
  }
  return *this;
}

Integer Integer::operator-(const Integer& t) const {
  Integer out = *this;
  out -= t;
  return out;
}

Integer& Integer::operator-=(const Integer& t) {
  Integer t2(t.magnitude, !t.sign);
  return *this += t2;
}

Integer Integer::operator*(const Integer& t) const {
  return Integer(magnitude * t.magnitude, sign != t.sign);
}

Integer& Integer::operator*=(const Integer& t) {
  magnitude *= t.magnitude;
  sign = (sign != t.sign);
  return *this;
}

std::pair<Integer, Integer> DivMod(const Integer& N, const Integer& D) {
  auto [Q_pos, R_pos] = DivMod(N.magnitude, D.magnitude);
  // signs python-style
  return std::make_pair(Integer(Q_pos, N.sign != D.sign),
                        Integer((N.sign == D.sign) ? R_pos : D - R_pos));
}

Integer& Integer::operator/=(const Integer& t) {
  magnitude /= t.magnitude;
  sign = (sign != t.sign);
  return *this;
}

Integer Integer::operator/(const Integer& t) const {
  return std::get<0>(DivMod(*this, t));
}


std::string to_string(const Integer& t) {
  std::string str = t.sign ? "-" : "";
  return str + to_string(t.magnitude);
}

std::ostream& operator<<(std::ostream& stream, const Integer& t) {
  return stream << to_string(t);
}

}  // namespace exact
