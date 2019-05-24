#include <arbitrary_precision/Integer.h>

namespace ap {

Integer::Integer(int32_t x) {
  sign = (x < 0);
  mag = UnsignedInteger(std::abs(x));
}

Integer::Integer() : Integer(0) {}

Integer::Integer(const UnsignedInteger& other) : mag(other), sign(false) {}

std::string Integer::Print() const {
  return std::string(sign ? "-" : "") + mag.Print();
}

int Integer::Compare(const Integer& other) const {
  if (!sign && !other.sign) {
    // + +
    return mag.Compare(other.mag);
  } else if (sign && other.sign) {
    // - -
    return mag.Compare(other.mag) * -1;
  } else if (!sign && other.sign) {
    // + -
    return (mag == 0 && other.mag == 0) ? 0 : 1;
  } else {
    // - +
    return (mag == 0 && other.mag == 0) ? 0 : -1;
  }
}


void Integer::operator+=(const Integer &other) {
  if (sign == other.sign) {
    mag += other.mag;
  } else {
    bool flip_order = mag < other.mag;
    if (flip_order) {
      mag = other.mag - mag;
    } else {
      mag -= other.mag;
    }

    sign = ((!flip_order && sign) || (flip_order && !sign)) && (mag != 0);
  }
}

void Integer::operator-=(const Integer &other) {
  Integer flipped = other;
  flipped.sign = !other.sign;
  *this += flipped;
}

void Integer::operator*=(const Integer &other) {
  mag *= other.mag;
  sign = (sign != other.sign);
}

void Integer::LeftShift(const ap::Integer &other) {
  if (other.sign) {
    mag.RightShift(other.mag);
  } else {
    mag.LeftShift(other.mag);
  }
}

void Integer::RightShift(const ap::Integer &other) {
  if (other.sign) {
    mag.LeftShift(other.mag);
  } else {
    mag.RightShift(other.mag);
  }
}

Integer Integer::Pow(const Integer& p) const {
  Integer out = *this;

  if (p > 1) {
    out = Integer(mag.Pow(p.mag));
    out.sign = sign && (p % 2 != 0);
  } else if (p < 1) {
    auto counter = p;

    while (counter < 1 && out != 0) {
      out = out / (*this);
      counter += 1;
    }
  }

  return out;
}

std::tuple<Integer, Integer> Integer::DivMod(const Integer& t) const {
  auto [Q_u, R_u] = mag.DivMod(t.mag);

  auto out = std::make_tuple<Integer, Integer>(Q_u, R_u);

  std::get<0>(out).sign = (sign != t.sign);  // quotient sign
  std::get<1>(out).sign = sign;  // remainder sign

  return out;
}

}  // namespace ap
