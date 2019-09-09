#include <exact/non_negative_integer.h>


namespace exact {

NonNegativeInteger::NonNegativeInteger() : NonNegativeInteger(0) {}

NonNegativeInteger::NonNegativeInteger(unsigned int initial) {
  data_.push_back(initial);
}

bool NonNegativeInteger::operator==(const NonNegativeInteger& t) const {
  return compare(*this, t) == 0;
}
bool NonNegativeInteger::operator!=(const NonNegativeInteger& t) const {
  return compare(*this, t) != 0;
}
bool NonNegativeInteger::operator<(const NonNegativeInteger& t) const {
  return compare(*this, t) < 0;
}
bool NonNegativeInteger::operator<=(const NonNegativeInteger& t) const {
  return compare(*this, t) <= 0;
}
bool NonNegativeInteger::operator>(const NonNegativeInteger& t) const {
  return compare(*this, t) > 0;
}
bool NonNegativeInteger::operator>=(const NonNegativeInteger& t) const {
  return compare(*this, t) >= 0;
}

int compare(const NonNegativeInteger& a, const NonNegativeInteger& b) {
  int mag_compare = 0;
  auto a_it = a.data_.crbegin();
  const size_t a_size = a.data_.size();
  auto b_it = b.data_.crbegin();
  const size_t b_size = b.data_.size();
  size_t counter = std::max(a_size, b_size);

  while (mag_compare == 0 && counter > 0) {
    if (counter <= a_size && counter <= b_size && *a_it != *b_it) {
      mag_compare = (*a_it < *b_it) ? -1 : 1;
      ++a_it;
      ++b_it;
    } else if (counter > a_size && *b_it > 0) {
      if (*b_it > 0) mag_compare = -1;
      ++b_it;
    } else if (counter > b_size && *a_it > 0) {
      if (*a_it > 0) mag_compare = 1;
      ++a_it;
    }
    counter--;
  }

  return mag_compare;
}

void NonNegativeInteger::Trim() {
  auto first_nonzero_it = data_.end() - 1;
  while (first_nonzero_it != data_.begin() && *first_nonzero_it == 0) {
    first_nonzero_it--;
  }
  data_.erase(first_nonzero_it + 1, data_.end());
}

NonNegativeInteger& NonNegativeInteger::operator+=(
    const NonNegativeInteger& t) {
  if (t.data_.size() > data_.size()) {
    data_.insert(data_.end(), t.data_.size() - data_.size(), 0);
  }

  uint32_t carry = 0;
  auto it = data_.begin();
  auto it_other = t.data_.begin();
  for (; it != data_.end(); it++) {
    uint64_t val = carry + *it;
    if (it_other != t.data_.end()) {
      val += *it_other;
      it_other++;
    }

    carry = val >> 32u;
    *it = val & 0xFFFFFFFFu;
  }

  if (carry != 0) {
    data_.push_back(carry);
  }

  Trim();
  return *this;
}

// NonNegativeInteger& NonNegativeInteger::operator-=(
//    const NonNegativeInteger& t) const {
//  return <#initializer #>;
//}
// NonNegativeInteger& NonNegativeInteger::operator*=(
//    const NonNegativeInteger& t) const {
//  return <#initializer #>;
//}
// NonNegativeInteger& NonNegativeInteger::operator/=(
//    const NonNegativeInteger& t) const {
//  return <#initializer #>;
//}

}  // namespace exact
