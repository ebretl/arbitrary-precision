#include <exact/non_negative_integer.h>

#include <exact/exceptions.h>
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
    if (counter <= a_size && counter <= b_size) {
      if (*a_it != *b_it) {
        mag_compare = (*a_it < *b_it) ? -1 : 1;
      }
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

NonNegativeInteger NonNegativeInteger::operator+(
    const NonNegativeInteger& t) const {
  NonNegativeInteger out = *this;
  out += t;
  return out;
}

NonNegativeInteger& NonNegativeInteger::operator+=(
    const NonNegativeInteger& t) {
  if (t.data_.size() > data_.size()) {
    data_.insert(data_.end(), t.data_.size() - data_.size(), 0);
  }

  uint64_t carry = 0;
  auto it = data_.begin();
  auto it_other = t.data_.begin();
  for (; it != data_.end(); it++) {
    uint64_t val = carry + static_cast<uint64_t>(*it);
    if (it_other != t.data_.end()) {
      val += static_cast<uint64_t>(*it_other);
      ++it_other;
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

NonNegativeInteger NonNegativeInteger::operator-(
    const NonNegativeInteger& t) const {
  NonNegativeInteger out = *this;
  out -= t;
  return out;
}

NonNegativeInteger& NonNegativeInteger::operator-=(
    const NonNegativeInteger& t) {
  if (t > *this) {
    throw exact::OperationException(
        "subtracting larger value from UnsignedInteger");
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

NonNegativeInteger NonNegativeInteger::operator*(
    const NonNegativeInteger& t) const {
  if (*this == 0 || t == 0) {
    return NonNegativeInteger(0);
  } else if (*this == 1) {
    return t;
  } else if (t == 1) {
    return *this;
  } else {
    constexpr size_t min_karatsuba_size = 100;
    if (data_.size() >= min_karatsuba_size &&
        t.data_.size() >= min_karatsuba_size) {
      return _karatsuba(*this, t);
    } else {
      return _long_multiply(*this, t);
    }
    //    return _long_multiply(*this, t);
  }
}

NonNegativeInteger& NonNegativeInteger::operator*=(
    const NonNegativeInteger& t) {
  *this = *this * t;
  return *this;
}

NonNegativeInteger _long_multiply(const NonNegativeInteger& a,
                                  const NonNegativeInteger& b) {
  const size_t a_size = a.data_.size();
  const size_t b_size = b.data_.size();

  NonNegativeInteger prod;
  prod.data_.clear();
  prod.data_.insert(prod.data_.end(), a_size + b_size, 0);

  for (size_t i = 0; i < a_size; i++) {
    for (size_t j = 0; j < b_size; j++) {
      uint64_t value =
          static_cast<uint64_t>(a.data_[i]) * static_cast<uint64_t>(b.data_[j]);
      uint64_t carry = value >> 32u;
      value = (value & bitmask32) + prod.data_[i + j];
      carry += value >> 32u;
      prod.data_[i + j] = value & bitmask32;

      for (size_t k = i + j; k < a_size + b_size && carry != 0; k++) {
        value = static_cast<uint64_t>(prod.data_[k + 1]) + carry;
        prod.data_[k + 1] = (value & bitmask32);
        carry = (carry + (value >> 32u)) >> 32u;
      }
    }
  }

  prod.Trim();
  return prod;
}

NonNegativeInteger _karatsuba(const NonNegativeInteger& a,
                              const NonNegativeInteger& b) {
  NonNegativeInteger x = a;
  NonNegativeInteger y = b;

  auto max_size = std::max(x.data_.size(), y.data_.size());
  auto shift = max_size * 16;

  auto x1 = x >> shift;
  auto y1 = y >> shift;
  auto x2 = x - (x1 << shift);
  auto y2 = y - (y1 << shift);

  NonNegativeInteger out = (x1 + x2) * (y1 + y2);  // K
  auto F = x1 * y1;
  auto G = x2 * y2;
  out -= F;
  out -= G;

  F <<= shift * 2;
  out <<= shift;

  out += F;
  out += G;

  return out;
}

NonNegativeInteger NonNegativeInteger::operator/(
    const NonNegativeInteger& t) const {
  return std::get<0>(DivMod(*this, t));
}

NonNegativeInteger& NonNegativeInteger::operator/=(
    const NonNegativeInteger& t) {
  *this = *this / t;
  return *this;
}

std::pair<NonNegativeInteger, NonNegativeInteger> DivMod(
    const NonNegativeInteger& N, const NonNegativeInteger& D) {
  if (D == 0) {
    throw exact::OperationException("divide by zero");
  }

  auto out = std::make_pair<NonNegativeInteger, NonNegativeInteger>(0, 0);
  auto& [Q, R] = out;

  Q.data_.clear();
  Q.data_.insert(Q.data_.end(), N.data_.size(), 0u);

  auto it = N.data_.rbegin();
  auto it_q = Q.data_.rbegin();
  while (it != N.data_.rend()) {
    for (unsigned char bit_counter = 0; bit_counter < 32; bit_counter++) {
      R <<= 1;
      R.data_.front() += (*it >> (31u - bit_counter)) & 0x1u;
      if (R >= D) {
        *it_q += (0x1u << (31u - bit_counter));
        R -= D;
      }
    }
    ++it;
    ++it_q;
  }

  Q.Trim();
  return out;
}

std::string to_string(const NonNegativeInteger& t) {
  std::deque<NonNegativeInteger> ds = {1};
  NonNegativeInteger n = t;
  while (n > ds.front()) {
    ds.push_front(ds.front() * 10);
  }
  if (ds.size() > 1 && ds.front() > n) {
    ds.pop_front();
  }

  std::string str;
  while (!ds.empty()) {
    str.push_back('0');
    while (n >= ds.front()) {
      n -= ds.front();
      ++str.back();
    }
    ds.pop_front();
  }

  return str;
}

std::ostream& operator<<(std::ostream& stream, const NonNegativeInteger& n) {
  return stream << to_string(n);
}

NonNegativeInteger& NonNegativeInteger::operator<<=(unsigned int bits) {
  if (bits >= 32) {
    data_.insert(data_.begin(), bits / 32u, 0);
    bits = bits % 32u;
  }

  if (bits > 0) {
    data_.push_back(0);
    auto big_it = data_.rbegin();
    auto small_it = big_it + 1;
    while (small_it != data_.rend()) {
      *big_it += (*small_it >> (32u - bits));
      *small_it <<= bits;
      big_it = small_it;
      ++small_it;
    }
    if (data_.back() == 0) {
      data_.pop_back();
    }
  }

  return *this;
}

NonNegativeInteger& NonNegativeInteger::operator>>=(unsigned int bits) {
  if (bits >= 32) {
    auto n_remove = bits / 32u;
    if (n_remove >= data_.size()) {
      data_.erase(data_.begin() + 1, data_.end());
      data_.front() = 0;
      bits = 0;
    } else {
      data_.erase(data_.begin(), data_.begin() + n_remove);
      bits = bits % 32;
    }
  }

  if (bits > 0) {
    data_.front() >>= bits;

    auto small_it = data_.begin();
    auto big_it = small_it + 1;
    while (big_it != data_.end()) {
      *small_it += (*big_it << (32u - bits));
      *big_it >>= bits;
      small_it = big_it;
      ++big_it;
    }
    Trim();
  }

  return *this;
}

NonNegativeInteger NonNegativeInteger::operator<<(unsigned int bits) {
  NonNegativeInteger copy(*this);
  copy <<= bits;
  return copy;
}

NonNegativeInteger NonNegativeInteger::operator>>(unsigned int bits) {
  NonNegativeInteger copy(*this);
  copy >>= bits;
  return copy;
}

}  // namespace exact
