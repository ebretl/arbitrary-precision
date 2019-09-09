#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do
                           // this in one cpp file
#include "catch.hpp"

#include <exact/exact.h>

using namespace exact;

TEST_CASE("non-negative integer comparable to int") {
  NonNegativeInteger n = 42;
  REQUIRE(n == 42);

  REQUIRE(n > 0);
  REQUIRE(n > 10);

  REQUIRE(n >= 0);
  REQUIRE(n >= 10);
  REQUIRE(n >= 42);

  REQUIRE(n < 100);

  REQUIRE(n <= 100);
  REQUIRE(n <= 42);
}

TEST_CASE("integer comparable to int") {
  REQUIRE(Integer(0) == 0);
  REQUIRE(Integer(1) == 1);
  REQUIRE(Integer(-1) == -1);
  REQUIRE(Integer(-1) < 1);
  REQUIRE(Integer(1) > -1);
  REQUIRE(Integer(1) >= 1);
  REQUIRE(Integer(1) <= 1);
  REQUIRE(Integer(-1) >= -1);
  REQUIRE(Integer(-1) <= -1);
}

TEST_CASE("integer x != -x") {
  Integer a(42);
  Integer b(-42);
  REQUIRE(a != b);
}

TEST_CASE("add positive ints") {
  REQUIRE(Integer(3) + Integer(7) == Integer(10));
  REQUIRE(Integer(42) + Integer(0) == Integer(42));
}

TEST_CASE("add negative ints") {
  REQUIRE(Integer(-3) + Integer(-7) == Integer(-10));
  REQUIRE(Integer(-42) + Integer(-0) == Integer(-42));
}

TEST_CASE("add mixed ints") {
  REQUIRE(Integer(-3) + Integer(7) == Integer(4));
  REQUIRE(Integer(42) + Integer(-12) == Integer(30));
}

TEST_CASE("print small") {
  REQUIRE(NonNegativeInteger(5).DecimalString() == "5");
  REQUIRE(Integer(10).DecimalString() == "10");
  REQUIRE(Integer(-2044).DecimalString() == "-2044");
}

TEST_CASE("NonNegativeInteger multiply") {
  REQUIRE(NonNegativeInteger(42) * NonNegativeInteger(10) ==
          NonNegativeInteger(420));
  REQUIRE(NonNegativeInteger(42) * NonNegativeInteger(0) ==
          NonNegativeInteger(0));
}

TEST_CASE("division nonnegative int") {
  NonNegativeInteger q, r;
  auto res = NonNegativeInteger(13).DivMod(3);
  q = std::get<0>(res);
  r = std::get<1>(res);
  REQUIRE(q == 4);
  REQUIRE(r == 1);

  res = NonNegativeInteger(100).DivMod(10);
  q = std::get<0>(res);
  r = std::get<1>(res);
  REQUIRE(q == 10);
  REQUIRE(r == 0);
}
