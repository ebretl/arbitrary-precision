#include <exact/integer.h>

#include <algorithm>

namespace exact {

Integer::Integer() : Integer(0) {}

Integer::Integer(const NonNegativeInteger& other)
    : magnitude_(other), sign_(false) {}

Integer::Integer(const NonNegativeInteger& other, bool sign)
    : magnitude_(other), sign_(sign) {}

Integer::Integer(int32_t initial)
    : magnitude_(std::abs(initial)), sign_(initial < 0) {}

Integer::Integer(const PositiveInteger& other) : magnitude_(abs(other)) {}

bool Integer::operator==(const Integer& t) const {
  if (sign_ == t.sign_) {  // same sign
    return magnitude_ == t.magnitude_;
  }
  return magnitude_ == 0;
}

bool Integer::operator!=(const Integer& t) const {
  if (sign_ == t.sign_) {  // same sign
    return magnitude_ != t.magnitude_;
  }
  return magnitude_ != 0;
}

bool Integer::operator<(const Integer& t) const {
  if (!sign_ && !t.sign_) {  // + +
    return magnitude_ < t.magnitude_;
  } else if (sign_ && t.sign_) {  // - -
    return magnitude_ > t.magnitude_;
  } else if (sign_ && !t.sign_) {  // - +
    return (magnitude_ > 0) || (t.magnitude_ > 0);
  } else {  // + -
    return (magnitude_ == 0) && (t.magnitude_ == 0);
  }
}

bool Integer::operator<=(const Integer& t) const {
  if (!sign_ && !t.sign_) {  // + +
    return magnitude_ <= t.magnitude_;
  } else if (sign_ && t.sign_) {  // - -
    return magnitude_ >= t.magnitude_;
  } else {
    return sign_ && !t.sign_;
  }
}

bool Integer::operator>(const Integer& t) const {
  if (!sign_ && !t.sign_) {  // + +
    return magnitude_ > t.magnitude_;
  } else if (sign_ && t.sign_) {  // - -
    return magnitude_ < t.magnitude_;
  } else if (sign_ && !t.sign_) {  // - +
    return (magnitude_ > 0) || (t.magnitude_ > 0);
  } else {  // + -
    return (magnitude_ != 0) && (t.magnitude_ != 0);
  }
}

bool Integer::operator>=(const Integer& t) const {
  if (!sign_ && !t.sign_) {  // + +
    return magnitude_ >= t.magnitude_;
  } else if (sign_ && t.sign_) {  // - -
    return magnitude_ <= t.magnitude_;
  } else {
    return !sign_ && t.sign_;
  }
}

Integer Integer::operator+(const Integer& t) const {
  Integer out = *this;
  out += t;
  return out;
}

Integer& Integer::operator+=(const Integer& t) {
  if (sign_ == t.sign_) {
    magnitude_ += t.magnitude_;
  } else {
    bool flip_order = magnitude_ < t.magnitude_;
    if (flip_order) {
      magnitude_ = t.magnitude_ - magnitude_;
      sign_ = t.sign_;
    } else {
      magnitude_ -= t.magnitude_;
    }

    sign_ = sign_ && (magnitude_ != 0);
  }
  return *this;
}

Integer Integer::operator-(const Integer& t) const {
  Integer out = *this;
  out -= t;
  return out;
}

Integer& Integer::operator-=(const Integer& t) {
  Integer t2(t.magnitude_, !t.sign_);
  return *this += t2;
}

Integer Integer::operator*(const Integer& t) const {
  return Integer(magnitude_ * t.magnitude_, sign_ != t.sign_);
}

Integer& Integer::operator*=(const Integer& t) {
  magnitude_ *= t.magnitude_;
  sign_ = (sign_ != t.sign_);
  return *this;
}

std::pair<Integer, Integer> DivMod(const Integer& N, const Integer& D) {
  auto [Q_pos, R_pos] = DivMod(N.magnitude_, D.magnitude_);
  // signs python-style
  return std::make_pair(Integer(Q_pos, N.sign_ != D.sign_),
                        Integer((N.sign_ == D.sign_) ? R_pos : D - R_pos));
}

Integer& Integer::operator/=(const Integer& t) {
  magnitude_ /= t.magnitude_;
  sign_ = (sign_ != t.sign_);
  return *this;
}

Integer Integer::operator/(const Integer& t) const {
  return std::get<0>(DivMod(*this, t));
}


std::string to_string(const Integer& t) {
  std::string str = t.sign_ ? "-" : "";
  return str + to_string(t.magnitude_);
}

std::ostream& operator<<(std::ostream& stream, const Integer& t) {
  return stream << to_string(t);
}

const NonNegativeInteger& abs(const Integer& t) { return t.magnitude_; }

}  // namespace exact
