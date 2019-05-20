#include <arbitrary_precision/UnsignedInteger.h>
#include <thread>


namespace ap {

UnsignedInteger::UnsignedInteger() : UnsignedInteger(0u) {}

UnsignedInteger::UnsignedInteger(uint32_t n) {
  data_.clear();
  for (uint8_t shift = 0; shift < 32; shift += 8) {
    auto byte = (n >> shift) & 0xFFu;
    data_.push_back(byte);
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

std::string UnsignedInteger::PrintRaw() const {
  std::string out;
  for (auto byte : data_) {
    out += std::to_string(static_cast<int>(byte)) + " ";
  }
  if (!out.empty()) {
    out.erase(out.size() - 1);
  }
  return out;
}

std::string UnsignedInteger::PrintDecimal() const {
  std::string out;
  std::vector<int> digits;

  UnsignedInteger N = *this;
  UnsignedInteger K = 1;
  while (K <= N) {
    K = K * 10;
  }

  while (K > 0) {
    if (N >= K) {
      N = N - K;
      digits.back()++;
    } else {
      K = K / 10;
      digits.push_back(0);
    }
  }

  if (digits.size() > 1) {
    digits.erase(digits.end() - 1);
  }

  for (int n : digits) {
    out += std::to_string(n);
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
  uint16_t carry = 0;
  for (size_t byte_num = 0; byte_num < std::max(data_.size(), other.data_.size()); byte_num++) {
    uint16_t val = carry;
    if (byte_num < data_.size()) {
      val += data_[byte_num];
    }
    if (byte_num < other.data_.size()) {
      val += other.data_[byte_num];
    }
    carry = val >> 8u;
    val = val & 0xFFu;
    out.data_.push_back(val);
  }

  if (carry != 0) {
    out.data_.push_back(carry);
  }

  return out;
}

UnsignedInteger UnsignedInteger::operator-(const ap::UnsignedInteger &other) const {
  UnsignedInteger other_negated;
  other_negated.data_.clear();
  for (auto byte : other.data_) {
    other_negated.data_.push_back(~byte);
  }

  if (data_.size() > other.data_.size()) {
    other_negated.data_.insert(other_negated.data_.end(), data_.size() - other.data_.size(), 255u);
  }

  other_negated = other_negated + 1;
  UnsignedInteger res = *this + other_negated;

  if (res.data_.size() > data_.size()) {
    res.data_.erase(res.data_.begin() + data_.size(), res.data_.end());
  }
  res.Trim();
  return res;
}

UnsignedInteger UnsignedInteger::Karatsuba(UnsignedInteger x, UnsignedInteger y) {
  // std::cout << "Karatsuba " << x.PrintRaw() << " | " << y.PrintRaw() << std::endl;

  UnsignedInteger out = 0;
  x.Trim();
  y.Trim();

  if (x != 0 && y != 0) {
    // std::cout << "0" << std::endl;

    auto max_bytes = std::max(x.data_.size(), y.data_.size());
    x.data_.insert(x.data_.end(), max_bytes - x.data_.size(), 0u);
    y.data_.insert(y.data_.end(), max_bytes - y.data_.size(), 0u);

    // std::cout << "1" << std::endl;

    if (max_bytes == 1) {
      out = x.data_[0] * y.data_[0];  // unsigned int constructor
    } else {
      auto shift_bytes = max_bytes / 2;

      auto x1 = x.ByteShifted(-shift_bytes);  // right shift
      auto x2 = x - x1.ByteShifted(shift_bytes);
      auto y1 = y.ByteShifted(-shift_bytes);
      auto y2 = y - y1.ByteShifted(shift_bytes);

      // std::cout << "x1 " << x1.PrintRaw() << std::endl;
      // std::cout << "x2 " << x2.PrintRaw() << std::endl;
      // std::cout << "y1 " << y1.PrintRaw() << std::endl;
      // std::cout << "y2 " << y2.PrintRaw() << std::endl;

      // std::this_thread::sleep_for(std::chrono::milliseconds(100));

      auto F = x1 * y1;
      auto G = x2 * y2;
      auto H = (x1 + x2) * (y1 + y2);
      auto K = H - F - G;

      out = F.ByteShifted(2 * shift_bytes) + K.ByteShifted(shift_bytes) + G;
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
        auto a = static_cast<uint16_t>(data_[i]);
        auto b = static_cast<uint16_t>(other.data_[j]);
        uint16_t val = a * b;
        overflow.data_.push_back(val >> 8u);
        direct.data_.push_back(val & 0xFFu);
      }

      UnsignedInteger partial = direct + overflow;
      partial.data_.insert(partial.data_.begin(), i, 0u);

      out = out + partial;
    }
  }

  out.Trim();
  return out;
}

UnsignedInteger UnsignedInteger::operator*(const UnsignedInteger &other) const {
  UnsignedInteger out;
  auto max_size = std::max(data_.size(), other.data_.size());
  if (max_size > 250) {
    out = Karatsuba(*this, other);
  } else {
    out = LongMultiply(other);
  }
  return out;
}

UnsignedInteger UnsignedInteger::ByteShifted(int32_t shift) const {
  UnsignedInteger out = *this;
  if (shift > 0) {
    out.data_.insert(out.data_.begin(), shift, 0u);
  } else if (shift < 0) {
    shift = std::min(std::abs(shift), static_cast<int32_t>(out.data_.size()));
    out.data_.erase(out.data_.begin(), out.data_.begin() + std::abs(shift));
  }

  if (out.data_.empty()) {
    out.data_.push_back(0u);
  }

  return out;
}

bool UnsignedInteger::GetBit(uint32_t b) const {
  uint32_t i = b / 8u;
  uint8_t j = b % 8u;
  return (i < data_.size()) ? ((data_[i] >> j) & 1u) : false;
}

void UnsignedInteger::SetBit(uint32_t b, bool val) {
  uint32_t i = b / 8u;
  uint8_t j = b % 8u;

  if (i >= data_.size()) {
    data_.insert(data_.end(), i - data_.size(), 0u);
  }

  uint8_t v = (data_[i] >> j) & 1u;
  if (v && !val) {
    data_[i] -= 1u << j;
  } else if (!v && val) {
    data_[i] += 1u << j;
  }
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
    for (int j = 7; j >= 0; j--) {
      R = R * 2;
      R.SetBit(0, this->GetBit(8 * i + j));
      if (R >= D) {
        R = R - D;
        Q.SetBit(8 * i + j, true);
      }
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
