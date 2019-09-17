#include "catch.hpp"

#include <exact/exact.h>

using namespace exact;

TEST_CASE("constructor throws") {
  REQUIRE_THROWS(PositiveInteger(0));
  REQUIRE_THROWS(PositiveInteger(-1));
  REQUIRE_THROWS(PositiveInteger(NonNegativeInteger(0)));
}

TEST_CASE("PositiveInteger comparisons") {
  NonNegativeInteger base =
      (NonNegativeInteger(10) << 483) + (NonNegativeInteger(3736) << 184);
  PositiveInteger n = base;

  PositiveInteger larger = base + 1;
  PositiveInteger way_larger = base << 100;
  PositiveInteger smaller = base - 1;
  PositiveInteger way_smaller = 1;

  SECTION("equals") {
    REQUIRE(n == n);
    REQUIRE_FALSE(n == larger);
    REQUIRE_FALSE(n == way_larger);
    REQUIRE_FALSE(n == smaller);
    REQUIRE_FALSE(n == way_smaller);
  }
  SECTION("not equals") {
    REQUIRE_FALSE(n != n);
    REQUIRE(n != larger);
    REQUIRE(n != way_larger);
    REQUIRE(n != smaller);
    REQUIRE(n != way_smaller);
  }
  SECTION("strictly less") {
    REQUIRE_FALSE(n < n);
    REQUIRE(n < larger);
    REQUIRE(n < way_larger);
    REQUIRE_FALSE(n < smaller);
    REQUIRE_FALSE(n < way_smaller);
  }
}

TEST_CASE("positive integer addition") {
  NonNegativeInteger n1 =
      (NonNegativeInteger(10) << 483) + (NonNegativeInteger(3736) << 184);
  NonNegativeInteger n2 =
      (NonNegativeInteger(47) << 190) + (NonNegativeInteger(3837) << 91);
  PositiveInteger p1 = n1;
  PositiveInteger p2 = n2;

  REQUIRE(p1 + p2 == n1 + n2);
}

TEST_CASE("positive integer subtraction") {
  NonNegativeInteger n1 =
      (NonNegativeInteger(10) << 483) + (NonNegativeInteger(3736) << 184);
  NonNegativeInteger n2 =
      (NonNegativeInteger(47) << 190) + (NonNegativeInteger(3837) << 91);
  PositiveInteger p1 = n1;
  PositiveInteger p2 = n2;

  REQUIRE(p1 - p2 == n1 - n2);
  REQUIRE_THROWS(p2 - p1);
}
