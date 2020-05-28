#include <exact/rational.h>

namespace exact {

Rational::Rational(const Integer& n, const PositiveInteger& d) : N(n), D(d) {}

Rational::Rational(int n, int d)
    : N(std::abs(n), (n < 0) != (d < 0)), D(std::abs(d)) {}

Rational::Rational(int n) : N(n), D(1) {}

Rational::Rational(const Integer& n) : N(n), D(1) {}

bool Rational::operator==(const Rational& t) const {
  if (N == t.N && (N == 0 || D == t.D)) {
    return true;
  }
  if ((N < 0) != (t.N < 0)) {
    return false;
  }

  auto [q1, r1] = DivMod(N.magnitude, D.magnitude);
  auto [q2, r2] = DivMod(t.N.magnitude, t.D.magnitude);
  return q1 == q2 && (Rational(D, r1) == Rational(t.D, r2));
}

bool Rational::operator!=(const Rational& t) const { return !(*this == t); }

bool Rational::operator<(const Rational& t) const {
  if (D == t.D || N.sign != t.N.sign) return N < t.N;
  auto [q1, r1] = DivMod(N.magnitude, D.magnitude);
  auto [q2, r2] = DivMod(t.N.magnitude, t.D.magnitude);
  if (q1 < q2) return true;
  if (r1 == 0) return (q1 == q2) && (r1 < r2);
  if (r2 == 0) return false;
  return Rational(D, r1) > Rational(t.D, r2);
}

bool Rational::operator<=(const Rational& t) const {
  if (D == t.D || N.sign != t.N.sign) return N <= t.N;
  auto [q1, r1] = DivMod(N.magnitude, D.magnitude);
  auto [q2, r2] = DivMod(t.N.magnitude, t.D.magnitude);
  std::cout << "<=" << std::endl;
  std::cout << "q1 " << q1 << ", r1 " << r1 << std::endl;
  std::cout << "q2 " << q2 << ", r2 " << r2 << std::endl;
  return (q1 <= q2) || Rational(D, r1) >= Rational(t.D, r2);
}

bool Rational::operator>(const Rational& t) const {
  if (D == t.D || N.sign != t.N.sign) return N > t.N;
  auto [q1, r1] = DivMod(N.magnitude, D.magnitude);
  auto [q2, r2] = DivMod(t.N.magnitude, t.D.magnitude);
  return (q1 > q2) || Rational(D, r1) < Rational(t.D, r2);
}

bool Rational::operator>=(const Rational& t) const {
  if (D == t.D || N.sign != t.N.sign) return N >= t.N;
  auto [q1, r1] = DivMod(N.magnitude, D.magnitude);
  auto [q2, r2] = DivMod(t.N.magnitude, t.D.magnitude);
  std::cout << ">=" << std::endl;
  std::cout << "q1 " << q1 << ", r1 " << r1 << std::endl;
  std::cout << "q2 " << q2 << ", r2 " << r2 << std::endl;
  return (q1 >= q2) || Rational(D, r1) <= Rational(t.D, r2);
}

Rational& Rational::operator+=(const Rational& t) { return *this; }
Rational& Rational::operator-=(const Rational& t) { return *this; }
Rational& Rational::operator*=(const Rational& t) { return *this; }
Rational& Rational::operator/=(const Rational& t) { return *this; }
// Rational Rational::operator/(const Rational& t) const { return Rational(); }
// std::pair<Rational, Rational> DivMod(const Rational& a, const Rational& b) {
//  return std::pair<Rational, Rational>();
//}
// Rational Rational::operator+(const Rational& t) const { return Rational(); }
// Rational Rational::operator*(const Rational& t) const { return Rational(); }
// Rational Rational::operator-(const Rational& t) const { return Rational(); }

std::string to_string(const Rational& t) { return std::__cxx11::string(); }
std::ostream& operator<<(std::ostream& stream, const Rational& t) {
  return stream;
}

}  // namespace exact
