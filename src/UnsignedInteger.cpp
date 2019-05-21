#include <arbitrary_precision/UnsignedInteger.h>
#include <thread>


namespace ap {

UnsignedInteger::UnsignedInteger() : UnsignedInteger(0u) {}

UnsignedInteger::UnsignedInteger(uint32_t n) {
  data_.clear();
  uint32_t divisor = 1;
  for (int i = 0; i < 10; i++) {
    auto digit = (n / divisor) % 10;
    data_.push_back(digit);
    divisor *= 10;
  }
  Trim();
}

UnsignedInteger::UnsignedInteger(const UnsignedInteger& other) {
  data_ = other.data_;
}

void UnsignedInteger::Trim() {
  auto last_nonzero_it = data_.end();
  while (last_nonzero_it != data_.begin() + 1 && *(last_nonzero_it - 1) == 0) {
    last_nonzero_it--;
  }
  data_.erase(last_nonzero_it, data_.end());
}

std::string UnsignedInteger::Print() const {
  std::string out;
  for (auto it = data_.rbegin(); it != data_.rend(); ++it) {
    out += std::to_string(static_cast<int>(*it));
  }
  return out;
}

int UnsignedInteger::Compare(const ap::UnsignedInteger &other) const {
  int res = 0;
  for (size_t i = std::max(data_.size(), other.data_.size()); i > 0 && res == 0; i--) {
    auto j = i - 1;
    if (j < data_.size() && j < other.data_.size() && data_[j] != other.data_[j]) {
      res = (data_[j] < other.data_[j]) ? -1 : 1;
    } else if (j >= data_.size() && other.data_[j] > 0) {
      res = -1;
    } else if (j >= other.data_.size() && data_[j] > 0) {
      res = 1;
    }
  }
  return res;
}

UnsignedInteger UnsignedInteger::operator+(const ap::UnsignedInteger &other) const {
  UnsignedInteger out;
  out.data_.clear();
  uint8_t carry = 0;
  for (size_t byte_num = 0; byte_num < std::max(data_.size(), other.data_.size()); byte_num++) {
    uint8_t val = carry;
    if (byte_num < data_.size()) {
      val += data_[byte_num];
    }
    if (byte_num < other.data_.size()) {
      val += other.data_[byte_num];
    }
    carry = val / 10;
    out.data_.push_back(val % 10);
  }

  if (carry != 0) {
    out.data_.push_back(carry);
  }

  return out;
}

UnsignedInteger UnsignedInteger::operator-(const ap::UnsignedInteger &other) const {
  if (other > *this) {
    throw std::exception();
  }

  UnsignedInteger other_negated;
  other_negated.data_.clear();
  for (auto digit : other.data_) {
    other_negated.data_.push_back(9u - digit);
  }

  if (data_.size() > other.data_.size()) {
    other_negated.data_.insert(other_negated.data_.end(), data_.size() - other.data_.size(), 9u);
  }

  other_negated = other_negated + 1u;
  UnsignedInteger res = *this + other_negated;

  if (res.data_.size() > data_.size()) {
    res.data_.erase(res.data_.begin() + data_.size(), res.data_.end());
  }
  res.Trim();
  return res;
}

UnsignedInteger UnsignedInteger::Karatsuba(UnsignedInteger& x, UnsignedInteger& y) {
  UnsignedInteger out = 0;
  x.Trim();
  y.Trim();

  if (x != 0 && y != 0) {
    auto max_digits = std::max(x.data_.size(), y.data_.size());
    x.data_.insert(x.data_.end(), max_digits - x.data_.size(), 0u);
    y.data_.insert(y.data_.end(), max_digits - y.data_.size(), 0u);

    if (max_digits == 1) {
      out = x.data_[0] * y.data_[0];  // unsigned int constructor
    } else {
      auto shift = max_digits / 2;

      auto x1 = x >> shift;
      auto x2 = x - (x1 << shift);
      auto y1 = y >> shift;
      auto y2 = y - (y1 << shift);
      auto x_sum = x1 + x2;
      auto y_sum = y1 + y2;

      auto F = _mult_impl(x1, y1);
      auto G = _mult_impl(x2, y2);
      auto H = _mult_impl(x_sum, y_sum);
      auto K = H - F - G;

      out = F << (2 * shift) + K << (shift) + G;
    }

    out.Trim();
  }

  return out;
}

UnsignedInteger UnsignedInteger::LongMultiply(const UnsignedInteger& other) const {
  UnsignedInteger out = 0;

  if (*this != 0 && other != 0) {
    for (size_t i = 0; i < data_.size(); i++) {
      UnsignedInteger direct, overflow;
      direct.data_.clear();
      overflow.data_.clear();
      overflow.data_.push_back(0);

      for (size_t j = 0; j < other.data_.size(); j++) {
        uint8_t val = data_[i] * other.data_[j];
        overflow.data_.push_back(val / 10u);
        direct.data_.push_back(val % 10u);
      }

      UnsignedInteger partial = direct + overflow;
      partial.data_.insert(partial.data_.begin(), i, 0u);

      out = out + partial;
    }
  }

  out.Trim();
  return out;
}

UnsignedInteger UnsignedInteger::_mult_impl(UnsignedInteger& x, UnsignedInteger& y) {
  UnsignedInteger out;
  auto max_size = std::max(x.data_.size(), y.data_.size());
  if (max_size > 250) {
    out = Karatsuba(x, y);
  } else {
    out = x.LongMultiply(y);
  }
  return out;
}

UnsignedInteger UnsignedInteger::operator*(const UnsignedInteger &other) const {
  auto x = *this;
  auto y = other;
  return _mult_impl(x, y);
}

UnsignedInteger UnsignedInteger::operator<<(const UnsignedInteger& shift) const {
  UnsignedInteger out = *this;
  auto counter = shift;

  for (; counter > 0; counter = counter - 1) {
    out.data_.insert(out.data_.begin(), 0u);
  }

  return out;
}

UnsignedInteger UnsignedInteger::operator>>(const UnsignedInteger& shift) const {
  UnsignedInteger out = *this;
  auto counter = shift;

  for (; counter > 0 && !out.data_.empty(); counter = counter - 1) {
    out.data_.erase(out.data_.begin());
  }

  if (out.data_.empty()) {
    out.data_.push_back(0u);
  }

  return out;
}

std::tuple<UnsignedInteger, UnsignedInteger> UnsignedInteger::DivMod(const UnsignedInteger &D) const {
  if (D == 0) {
    throw std::exception();
  }

  auto out = std::make_tuple<UnsignedInteger, UnsignedInteger>(0, 0);
  auto& [Q, R] = out;

  Q.data_.clear();
  Q.data_.insert(Q.data_.begin(), data_.size(), 0u);

  for (int i = Q.data_.size() - 1; i >= 0; i--) {
    R.data_.insert(R.data_.begin(), data_[i]);
    while (R >= D) {
      R = R - D;
      Q.data_[i] += 1;
    }
  }

  Q.Trim();
  return out;
}

UnsignedInteger UnsignedInteger::operator/(const UnsignedInteger &D) const {
  return std::get<0>(DivMod(D));
}

UnsignedInteger UnsignedInteger::operator%(const UnsignedInteger &D) const {
  return std::get<1>(DivMod(D));
}

UnsignedInteger UnsignedInteger::Pow(const UnsignedInteger& p) const {
  UnsignedInteger out = 1;

  std::cout << *this << " ^ " << p << std::endl;

  auto [q, r] = p.DivMod(2);

  if (p > 3 && r == 0) {
    out = Pow(q) * Pow(q);
  } else if (p > 1) {
    out = *this * Pow(p - 1);
  } else if (p == 1) {
    out = *this;
  }

  return out;
}

}  // namespace ap
