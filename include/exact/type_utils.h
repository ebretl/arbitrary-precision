#pragma once

#include <iostream>
#include <string>
#include <tuple>

#define EXACT_DECLARE_COMPARISON_OPERATORS(T) \
  bool operator==(const T& t) const;          \
  bool operator!=(const T& t) const;          \
  bool operator<(const T& t) const;           \
  bool operator<=(const T& t) const;          \
  bool operator>(const T& t) const;           \
  bool operator>=(const T& t) const;

#define EXACT_DECLARE_ARITHMETIC_OPERATORS(T)            \
  T operator+(const T& t) const;                         \
  T operator-(const T& t) const;                         \
  T operator*(const T& t) const;                         \
  T operator/(const T& t) const;                         \
  friend std::pair<T, T> DivMod(const T& a, const T& b); \
  T& operator+=(const T& t);                             \
  T& operator-=(const T& t);                             \
  T& operator*=(const T& t);                             \
  T& operator/=(const T& t);

#define EXACT_DECLARE_PRINT_OPERATORS(T)    \
  friend std::string to_string(const T& t); \
  friend std::ostream& operator<<(std::ostream& stream, const T& t);
