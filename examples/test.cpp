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
  double t;

  for (int x : {100, 500, 1000, 5000, 10000}) {
    t = time_now();
    ap::Float::Factory f(x);
    cout << "making factory " << time_now() - t << endl;

    t = time_now();
    auto a = f(1);
    auto b = f(2);
    cout << "creating a and b " << time_now() - t << endl;

    t = time_now();
    auto y = a * b;
    cout << "multiply " << time_now() - t << endl;

    if (x <= 500) {
      cout << y << endl;
    }
  }

  // for (int i = 0; i < 6; i++) {
  //   cout << ap::Integer(-10).Pow(i) << endl;
  // }
}
