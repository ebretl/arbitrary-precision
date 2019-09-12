#include <arbitrary_precision/arbitrary_precision.h>
#include <exact/exact.h>

ap::UnsignedInteger ap_factorial(int n) {
  ap::UnsignedInteger out = 1;
  if (n > 1) {
    out = ap::UnsignedInteger(n) * ap_factorial(n - 1);
  }
  return out;
}

exact::NonNegativeInteger exact_factorial(int n) {
  exact::NonNegativeInteger out = 1;
  if (n > 1) {
    out = exact::NonNegativeInteger(n) * exact_factorial(n - 1);
  }
  return out;
}

double time_now() {
  auto&& time_since_epoch = std::chrono::system_clock::now().time_since_epoch();
  return std::chrono::duration<double>(time_since_epoch).count();
}

int main() {
  int n = 500;

  double t_start = time_now();
  auto f1 = ap_factorial(n) / 1000000;
  std::cout << time_now() - t_start << std::endl;
  std::cout << f1 << std::endl;

  t_start = time_now();
  auto f2 = exact_factorial(n) / 1000000;
  std::cout << time_now() - t_start << std::endl;
  std::cout << f2 << std::endl;
}
