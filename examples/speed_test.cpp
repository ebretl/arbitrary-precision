#include <arbitrary_precision/arbitrary_precision.h>
#include <exact/exact.h>

template <typename T>
T factorial(int n) {
  T out = 1;
  if (n > 1) {
    out = T(n) * factorial<T>(n - 1);
  }
  return out;
}

double time_now() {
  auto&& time_since_epoch = std::chrono::system_clock::now().time_since_epoch();
  return std::chrono::duration<double>(time_since_epoch).count();
}

int main() {
  int n1 = 1000;
  int n2 = 950;

  double t_start = time_now();
  auto f1 = factorial<ap::Integer>(n1) / factorial<ap::Integer>(n2);
  std::cout << "without print: " << time_now() - t_start << std::endl;
  std::cout << f1 << std::endl;
  std::cout << "with print: " << time_now() - t_start << std::endl;

  t_start = time_now();
  auto f2 = factorial<exact::Integer>(n1) / factorial<exact::Integer>(n2);
  std::cout << "without print: " << time_now() - t_start << std::endl;
  std::cout << f2 << std::endl;
  std::cout << "with print: " << time_now() - t_start << std::endl;

  t_start = time_now();
  auto x1 = exact::Integer(10);
  for (int i = 0; i < 18; i++) {
    x1 *= x1;
  }
  std::cout << time_now() - t_start << std::endl;
  //  std::cout << x1 << std::endl;

  t_start = time_now();
  auto x2 = exact::Integer(10);
  for (int i = 0; i < 18; i++) {
    x2 = x2 * x2;
  }
  std::cout << time_now() - t_start << std::endl;
  //  std::cout << x2 << std::endl;
}
