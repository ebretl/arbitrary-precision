#include <exact/rational.h>

namespace exact {

Rational::Rational(const Integer& n, const PositiveInteger& d) : N(n), D(d) {}

Rational::Rational(int n, int d)
    : N(std::abs(n), n < 0 != d < 0), D(std::abs(d)) {}

Rational::Rational(int n) : N(n), D(1) {}

Rational::Rational(const Integer& n) : N(n), D(1) {}

bool Rational::operator==(const Rational& t) const {
  if (N == t.N && D == t.D) {
    return true;
  }

  auto [q1, r1] = DivMod(abs(N), abs(D));
  auto [q2, r2] = DivMod(abs(t.N), abs(t.D));
  return q1 == q2 && (Rational(D, r1) == Rational(t.D, r2));
}

bool Rational::operator!=(const Rational& t) const { return !(*this == t); }

bool Rational::operator<(const Rational& t) const {}

bool Rational::operator<=(const Rational& t) const { return false; }
bool Rational::operator>=(const Rational& t) const { return false; }
bool Rational::operator>(const Rational& t) const { return false; }
Rational& Rational::operator+=(const Rational& t) { return *this; }
Rational& Rational::operator-=(const Rational& t) { return *this; }
Rational& Rational::operator*=(const Rational& t) { return *this; }
Rational& Rational::operator/=(const Rational& t) { return *this; }
//Rational Rational::operator/(const Rational& t) const { return Rational(); }
//std::pair<Rational, Rational> DivMod(const Rational& a, const Rational& b) {
//  return std::pair<Rational, Rational>();
//}
//Rational Rational::operator+(const Rational& t) const { return Rational(); }
//Rational Rational::operator*(const Rational& t) const { return Rational(); }
//Rational Rational::operator-(const Rational& t) const { return Rational(); }

std::string to_string(const Rational& t) { return std::__cxx11::string(); }
std::ostream& operator<<(std::ostream& stream, const Rational& t) {
  return stream;
}

}  // namespace exact
