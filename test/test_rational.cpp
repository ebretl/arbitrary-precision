#include "catch.hpp"

#include <exact/exact.h>

using namespace exact;

TEST_CASE("rational comparisons") {
  Integer n = Integer(std::numeric_limits<int>::max()) * 1000;
  PositiveInteger d = 385739;

  SECTION("positive") {
    Rational r{n, d};
    Rational larger_n{n + 1, d};
    Rational larger_d{n, d - 1};
    Rational smaller_n{n - 1, d};
    Rational smaller_d{n, d + 1};

    SECTION("equals") {
      REQUIRE(r == r);
      REQUIRE_FALSE(r == larger_n);
      REQUIRE_FALSE(r == larger_d);
      REQUIRE_FALSE(r == smaller_n);
      REQUIRE_FALSE(r == smaller_d);
    }
    SECTION("not equals") {
      REQUIRE_FALSE(r != r);
      REQUIRE(r != larger_n);
      REQUIRE(r != larger_d);
      REQUIRE(r != smaller_n);
      REQUIRE(r != smaller_d);
    }
    SECTION("strictly less") {
      REQUIRE_FALSE(r < r);
      REQUIRE(r < larger_n);
      REQUIRE(r < larger_d);
      REQUIRE_FALSE(r < smaller_n);
      REQUIRE_FALSE(r < smaller_d);
    }
  }
}
