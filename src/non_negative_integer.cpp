#include <exact/non_negative_integer.h>
//#include <vector>
#include <algorithm>


namespace exact {

constexpr uint64_t bitmask32 = std::numeric_limits<uint32_t>::max();

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

NonNegativeInteger operator+(const NonNegativeInteger& a,
                             const NonNegativeInteger& b) {
  NonNegativeInteger out = a;
  out += b;
  return out;
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
    *it = val & bitmask32;
  }

  if (carry != 0) {
    data_.push_back(carry);
  }

  Trim();
  return *this;
}

NonNegativeInteger operator-(const NonNegativeInteger& a,
                             const NonNegativeInteger& b) {
  NonNegativeInteger out = a;
  out -= b;
  return out;
}

NonNegativeInteger& NonNegativeInteger::operator-=(
    const NonNegativeInteger& t) {
  if (t > *this) {
    std::cerr << "Error: subtracting larger value from UnsignedInteger"
              << std::endl;
    throw std::exception();
  }

  NonNegativeInteger t_negated;
  t_negated.data_.clear();
  for (auto digit : t.data_) {
    t_negated.data_.push_back(bitmask32 - digit);
  }

  if (data_.size() > t.data_.size()) {
    t_negated.data_.insert(t_negated.data_.end(), data_.size() - t.data_.size(),
                           bitmask32);
  }

  t_negated += 1u;
  size_t orig_size = data_.size();
  *this += t_negated;

  if (data_.size() > orig_size) {
    data_.erase(data_.begin() + orig_size, data_.end());
  }
  Trim();
  return *this;
}

NonNegativeInteger operator*(const NonNegativeInteger& a,
                             const NonNegativeInteger& b) {
  if (a == 0 || b == 0) {
    return NonNegativeInteger(0);
  } else if (a == 1) {
    return b;
  } else if (b == 1) {
    return a;
  } else {
    //    if (data_.size() + y.data_.size() > 400) {
    //      Karatsuba(y);
    //    } else {
    //      LongMultiply(y);
    //    }
    return _long_multiply(a, b);
    // Karatsuba(y);
  }
}

NonNegativeInteger& NonNegativeInteger::operator*=(
    const NonNegativeInteger& t) {}

NonNegativeInteger _long_multiply(const NonNegativeInteger& a,
                                  const NonNegativeInteger& b) {
  const size_t a_size = a.data_.size();
  const size_t b_size = b.data_.size();

  NonNegativeInteger prod;
  prod.data_.clear();
  prod.data_.insert(prod.data_.end(), a.data_.size() + b.data_.size(), 0);

  for (size_t i = 0; i < a_size; i++) {
    for (size_t j = 0; j < b_size; j++) {
      uint64_t value = a.data_[i] * b.data_[j];
      uint64_t carry = value >> 32u;
      value = (value & bitmask32) + prod.data_[i + j];
      carry += value >> 32u;
      prod.data_[i + j] = value & bitmask32;

      for (size_t k = i + j; k < a_size + b_size && carry != 0; k++) {
        prod.data_[k + 1] += carry;
        carry >>= 32u;
      }
    }
  }

  prod.Trim();
  return prod;
}

NonNegativeInteger operator/(const NonNegativeInteger& n,
                             const NonNegativeInteger& d) {
  return std::get<0>(n.DivMod(d));
}

NonNegativeInteger& NonNegativeInteger::operator/=(
    const NonNegativeInteger& t) {}

std::pair<NonNegativeInteger, NonNegativeInteger> NonNegativeInteger::DivMod(
    const NonNegativeInteger& D) const {
  if (D == 0) {
    std::cerr << "Error: divide by zero" << std::endl;
    throw std::exception();
  }

  auto out = std::make_pair<NonNegativeInteger, NonNegativeInteger>(0, 0);
  auto& [Q, R] = out;

  Q.data_.clear();
  Q.data_.insert(Q.data_.end(), data_.size(), 0u);
  R.data_.clear();

  auto it = data_.rbegin();
  auto it_q = Q.data_.rbegin();
  for (; it != data_.rend(); ++it, ++it_q) {
    R.data_.push_front(*it);
    while (R >= D) {
      R -= D;
      ++(*it_q);
    }
  }

  Q.Trim();
  return out;
}

std::string NonNegativeInteger::DecimalString() const {
  std::deque<NonNegativeInteger> ds = {10, 1};
  NonNegativeInteger n = *this;
  while (n >= ds.front()) {
    ds.push_front(ds.front() * 10);
  }
  ds.pop_front();

  std::string str;
  for (const NonNegativeInteger& d : ds) {
    auto [quotient, rem] = n.DivMod(d);
    n = std::move(rem);
    str.push_back('0' + quotient.data_[0]);
  }
  return str;
}

std::ostream& operator<<(std::ostream& stream, const NonNegativeInteger& n) {
  return stream << n.DecimalString();
}

}  // namespace exact
