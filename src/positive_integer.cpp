#include <exact/exceptions.h>
#include <exact/positive_integer.h>

namespace exact {

PositiveInteger::PositiveInteger(unsigned int initial)
    : PositiveInteger(UnsignedInteger(initial)) {}

PositiveInteger::PositiveInteger(int initial)
    : PositiveInteger(UnsignedInteger(std::max(0, initial))) {}

PositiveInteger::PositiveInteger(const UnsignedInteger& other) {
  if (other < 1) {
    throw OperationException("initialized PositiveInteger with " +
                             to_string(other));
  }
  magnitude = other;
}

bool PositiveInteger::operator==(const PositiveInteger& t) const {
  return magnitude == t.magnitude;
}

bool PositiveInteger::operator!=(const PositiveInteger& t) const {
  return magnitude != t.magnitude;
}

bool PositiveInteger::operator<(const PositiveInteger& t) const {
  return magnitude < t.magnitude;
}

bool PositiveInteger::operator>(const PositiveInteger& t) const {
  return magnitude > t.magnitude;
}

bool PositiveInteger::operator<=(const PositiveInteger& t) const {
  return magnitude <= t.magnitude;
}

bool PositiveInteger::operator>=(const PositiveInteger& t) const {
  return magnitude >= t.magnitude;
}

std::ostream& operator<<(std::ostream& stream, const PositiveInteger& t) {
  return stream << to_string(t);
}

std::string to_string(const exact::PositiveInteger& t) {
  return to_string(t.magnitude);
}

PositiveInteger& PositiveInteger::operator-=(const PositiveInteger& t) {
  if (t.magnitude >= magnitude) {
    throw OperationException(
        "PositiveInteger subtraction result would not be positive");
  }
  magnitude -= t.magnitude;
  return *this;
}

PositiveInteger& PositiveInteger::operator+=(const PositiveInteger& t) {
  magnitude += t.magnitude;
  return *this;
}

PositiveInteger PositiveInteger::operator-(const PositiveInteger& t) const {
  if (t.magnitude >= magnitude) {
    throw OperationException(
        "PositiveInteger subtraction result would not be positive");
  }
  return PositiveInteger(*this) -= t;
}

PositiveInteger PositiveInteger::operator+(const PositiveInteger& t) const {
  return PositiveInteger(*this) += t;
}

PositiveInteger& PositiveInteger::operator*=(const exact::PositiveInteger& t) {
  magnitude *= t.magnitude;
  return *this;
}

PositiveInteger PositiveInteger::operator*(
    const exact::PositiveInteger& t) const {
  return PositiveInteger(magnitude * t.magnitude);
}

PositiveInteger& PositiveInteger::operator/=(const exact::PositiveInteger& t) {
  if (t > *this) {
    throw OperationException(
        "Dividing PositiveInteger by a larger value would result in zero");
  }
  magnitude /= t.magnitude;
  return *this;
}

PositiveInteger PositiveInteger::operator/(
    const exact::PositiveInteger& t) const {
  if (t > *this) {
    throw OperationException(
        "Dividing PositiveInteger by a larger value would result in zero");
  }
  return PositiveInteger(magnitude / t.magnitude);
}

}  // namespace exact
