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
  using Float = ap::Float<1000>;

  auto gen_fn = [](Integer k) {
    cout << "starting " << k << endl;
    Float x = (Float(4)/(8*k+1) - Float(2)/(8*k+4) - Float(1)/(8*k+5) - Float(1)/(8*k+6)) / ap::Integer(16).Pow(k);
    cout << "finished " << k << endl;
    return x;
  };

  cout << ap::Integer(16).Pow(209) << endl;

  double t;

  t = time_now();
  Float a = 1;
  Float b = 2;
  cout << "creating a and b " << time_now() - t << endl;

  t = time_now();
  auto y = a * b;
  cout << "multiply " << time_now() - t << endl;

  t = time_now();
  auto y2 = a / b;
  cout << "divide " << time_now() - t << endl;
}
