#include <exact/integer.h>

#include <algorithm>

namespace exact {

Integer::Integer() : Integer(0) {}

Integer::Integer(const NonNegativeInteger& other)
    : magnitude_(other), sign_(false) {}

Integer::Integer(int initial)
    : magnitude_(std::abs(initial)), sign_(initial < 0) {}

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

Integer operator+(const Integer& a, const Integer& b) {
  Integer out = a;
  out += b;
  return out;
}
Integer& Integer::operator+=(const Integer& t) {
  magnitude_ += t.magnitude_;
  return *this;
}

}  // namespace exact
