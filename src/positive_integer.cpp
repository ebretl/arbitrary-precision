#include <exact/exceptions.h>
#include <exact/positive_integer.h>

namespace exact {

PositiveInteger::PositiveInteger(unsigned int initial)
    : PositiveInteger(NonNegativeInteger(initial)) {}

PositiveInteger::PositiveInteger(int initial)
    : PositiveInteger(NonNegativeInteger(std::max(0, initial))) {}

PositiveInteger::PositiveInteger(const NonNegativeInteger& other) {
  if (other < 1) {
    throw OperationException("initialized PositiveInteger with " +
                             to_string(other));
  }
  magnitude_ = other;
}

bool PositiveInteger::operator==(const PositiveInteger& t) const {
  return magnitude_ == t.magnitude_;
}

bool PositiveInteger::operator!=(const PositiveInteger& t) const {
  return magnitude_ != t.magnitude_;
}

bool PositiveInteger::operator<(const PositiveInteger& t) const {
  return magnitude_ < t.magnitude_;
}

std::ostream& operator<<(std::ostream& stream, const PositiveInteger& t) {
  return stream;
}

const NonNegativeInteger& abs(const PositiveInteger& t) { return t.magnitude_; }
PositiveInteger& PositiveInteger::operator+=(const PositiveInteger& t) {
  magnitude_ += t.magnitude_;
  return *this;
}

PositiveInteger& PositiveInteger::operator-=(const PositiveInteger& t) {
  if (t.magnitude_ >= magnitude_) {
    throw OperationException(
        "PositiveInteger subtraction result would not be positive");
  }
  magnitude_ -= t.magnitude_;
  return *this;
}

PositiveInteger PositiveInteger::operator-(const PositiveInteger& t) const {
  return PositiveInteger(*this) -= t;
}

PositiveInteger PositiveInteger::operator+(const PositiveInteger& t) const {
  return PositiveInteger(*this) += t;
}

}  // namespace exact
