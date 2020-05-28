#include "catch.hpp"

#include <exact/exact.h>

using namespace exact;

TEST_CASE("rational comparisons") {
  Integer n = Integer(std::numeric_limits<int>::max()) * 1000;
  PositiveInteger d = 385739;

  Rational r{n, d};
  Rational larger_n{n + 1, d};
  Rational larger_d{n, d - 1};
  Rational smaller_n{n - 1, d};
  Rational smaller_d{n, d + 1};
  Rational negative{n * -1, d};
  Rational zero{0, 85};

  SECTION("equals") {
    REQUIRE(r == r);
    REQUIRE_FALSE(r == larger_n);
    REQUIRE_FALSE(r == larger_d);
    REQUIRE_FALSE(r == smaller_n);
    REQUIRE_FALSE(r == smaller_d);
    REQUIRE_FALSE(r == negative);
    REQUIRE_FALSE(r == zero);
  }
  SECTION("strictly less") {
    REQUIRE_FALSE(r < r);
    REQUIRE(r < larger_n);
    REQUIRE(r < larger_d);
    REQUIRE_FALSE(r < smaller_n);
    REQUIRE_FALSE(r < smaller_d);
    REQUIRE_FALSE(r < negative);
    REQUIRE_FALSE(r < zero);
  }
  SECTION("greater or equal") {
    REQUIRE(r >= r);
    REQUIRE_FALSE(r >= larger_n);
    auto b = r >= larger_d;
    REQUIRE_FALSE(r >= larger_d);
    REQUIRE(r >= smaller_n);
    REQUIRE(r >= smaller_d);
    REQUIRE(r >= negative);
    REQUIRE(r >= zero);
  }
}

// TEST_CASE("Rational addition") {
//  UnsignedInteger m1 = UnsignedInteger(372) << 42;
//  UnsignedInteger m2 = UnsignedInteger(942) << 75;
//  UnsignedInteger m3 = UnsignedInteger(83) << 45;
//  UnsignedInteger m4 = UnsignedInteger(3) << 65;
//  Rational r1(m2, m1);
//  Rational r2(m4, m3);
//
//  REQUIRE_NOTHROW(r1 + r2);
//  Rational r3 = r1 + r2;
//  REQUIRE(r3 > r1);
//  REQUIRE(r3 > r1);
//}
