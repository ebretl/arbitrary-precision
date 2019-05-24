#include <iostream>
#include <tuple>
#include <numeric>
#include <random>
#include <array>
#include <chrono>

#include <arbitrary_precision/arbitrary_precision.h>

using std::cout, std::endl;


double time_now() {
  return std::chrono::duration<double>(std::chrono::system_clock::now().time_since_epoch()).count();
}

int main() {
  using ap::Integer;
  using Float = ap::Float<10>;

  double t;
  Integer sum = 1;
  Integer limit = 100;
  cout << "limit " << limit << endl;

  t = time_now();
  for (Integer x = 1; x <= limit; x += 1) {
    // cout << "sum " << sum << " add " << x << endl;
    sum *= x;
  }
  cout << time_now() - t << endl;
  cout << sum << endl;

  sum = 1;
  t = time_now();
  for (Integer x = 1; x <= limit; x += 1) {
    sum = sum * x;
  }
  cout << time_now() - t << endl;
  cout << sum << endl;
}
