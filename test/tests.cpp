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

TEST_CASE("add positive integers") {
  REQUIRE(Integer(3) + Integer(7) == Integer(10));
  REQUIRE(Integer(42) + Integer(0) == Integer(42));
}

TEST_CASE("add negative integers") {
  REQUIRE(Integer(-3) + Integer(-7) == Integer(-10));
  REQUIRE(Integer(-42) + Integer(-0) == Integer(-42));
}
