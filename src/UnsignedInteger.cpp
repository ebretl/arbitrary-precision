#include <arbitrary_precision/UnsignedInteger.h>
#include <thread>
#include <vector>


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

void UnsignedInteger::operator+=(const ap::UnsignedInteger &other) {
  if (other.data_.size() > data_.size()) {
    data_.insert(data_.end(), other.data_.size() - data_.size(), 0u);
  }

  uint8_t carry = 0;
  auto it = data_.begin();
  auto it_other = other.data_.begin();
  size_t digit = 0;
  for (; it != data_.end(); digit++, it++) {
    uint8_t val = carry + *it;
    if (it_other != other.data_.end()) {
      val += *it_other;
      it_other++;
    }

    carry = val / 10;
    *it = val % 10;
  }

  if (carry != 0) {
    data_.push_back(carry);
  }

  Trim();
}

void UnsignedInteger::operator-=(const ap::UnsignedInteger &other) {
  if (other > *this) {
    std::cerr << "Error: subtracting larger value from UnsignedInteger" << std::endl;
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
  size_t orig_size = data_.size();
  *this += other_negated;

  if (data_.size() > orig_size) {
    data_.erase(data_.begin() + orig_size, data_.end());
  }
  Trim();
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
      out = x.data_.front() * y.data_.front();  // unsigned int constructor
    } else {
      auto shift = max_digits / 2;

      auto x1 = x >> shift;
      auto y1 = y >> shift;
      auto x2 = x - (x1 << shift);
      auto y2 = y - (y1 << shift);
      auto x_sum = x1 + x2;
      auto y_sum = y1 + y2;

      auto F = _mult_impl(x1, y1);
      auto G = _mult_impl(x2, y2);
      auto H = _mult_impl(x_sum, y_sum);
      auto K = H - F - G;

      F.LeftShift(2 * shift);
      K.LeftShift(shift);

      out += F;
      out += K;
      out += G;
    }

    out.Trim();
  }

  return out;
}

UnsignedInteger UnsignedInteger::LongMultiply(UnsignedInteger& x, UnsignedInteger& y) {
  UnsignedInteger out = 0;

  if (x != 0 && y != 0) {
    // int i = x.data_.size() - 1;
    for (auto itx = x.data_.rbegin(); itx != x.data_.rend(); itx++) {
      UnsignedInteger partial, overflow;
      partial.data_.clear();
      overflow.data_.clear();

      for (auto ity = y.data_.rbegin(); ity != y.data_.rend(); ity++) {
        uint8_t val = (*itx) * (*ity);
        overflow.data_.push_front(val / 10u);
        partial.data_.push_front(val % 10u);
      }

      out.data_.push_front(0u);
      overflow.data_.push_front(0);
      partial += overflow;
      // partial.data_.insert(partial.data_.begin(), i, 0u);

      out += partial;
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
    out = LongMultiply(x, y);
  }
  return out;
  // return LongMultiply(other);
  // return Karatsuba(*this, other);
}

void UnsignedInteger::operator*=(const UnsignedInteger &other) {
  auto y = other;
  *this = _mult_impl(*this, y);
}

void UnsignedInteger::LeftShift(const UnsignedInteger& shift) {
  for (auto counter = shift; counter > 0; counter -= 1) {
    data_.push_front(0u);
  }
}

void UnsignedInteger::RightShift(const UnsignedInteger& shift) {
  for (auto counter = shift; counter > 0 && !data_.empty(); counter -= 1) {
    data_.pop_front();
  }

  if (data_.empty()) {
    data_.push_back(0u);
  }
}

std::tuple<UnsignedInteger, UnsignedInteger> UnsignedInteger::DivMod(const UnsignedInteger &D) const {
  if (D == 0) {
    std::cerr << "Error: divide by zero" << std::endl;
    throw std::exception();
  }

  auto out = std::make_tuple<UnsignedInteger, UnsignedInteger>(0, 0);
  auto& [Q, R] = out;

  Q.data_.clear();
  Q.data_.insert(Q.data_.begin(), data_.size(), 0u);

  auto it = data_.rbegin();
  auto it_q = Q.data_.rbegin();
  for (; it != data_.rend(); it++, it_q++) {
    R.data_.push_front(*it);
    while (R >= D) {
      R = R - D;
      *it_q += 1;
    }
  }

  Q.Trim();
  return out;
}

UnsignedInteger UnsignedInteger::Pow(const UnsignedInteger& p) const {
  UnsignedInteger out = (p == 0) ? 1 : *this;
  std::vector<char> stack;

  auto counter = p;

  while (counter > 1) {
    auto [q, r] = counter.DivMod(2);
    // std::cout << *this << " ^ " << p << " : " << "DivMod(" << counter << ", 2) = " << q << ", " << r << std::endl;

    if (r == 0) {
      stack.push_back(2);
      counter = std::move(q);
    } else {
      stack.push_back(1);
      counter -= 1;
    }
  }

  while (!stack.empty()) {
    auto& op = stack.back();
    if (op == 1) {
      out *= *this;
    } else {
      out *= out;
    }
    stack.pop_back();
  }

  // std::cout << out << std::endl;
  return out;
}

}  // namespace ap
