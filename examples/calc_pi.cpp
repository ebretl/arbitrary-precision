#include <iostream>
#include <tuple>

#include <arbitrary_precision/arbitrary_precision.h>

using std::cout, std::endl;


double time_now() {
  return std::chrono::duration<double>(std::chrono::system_clock::now().time_since_epoch()).count();
}

int main() {
  auto time_start = time_now();

  ap::Float::Factory f(150);
  ap::UnsignedInteger k = 0;
  ap::Float pi = f(0);

  auto gen_fn = [&f](ap::Integer k) {
    return (f(4)/f(8*k+1) - f(2)/f(8*k+4) - f(1)/f(8*k+5) - f(1)/f(8*k+6)) / f(ap::Integer(16).Pow(k));
  };

  auto add = [](const ap::Float& x1, const ap::Float& x2) {
    return x1 + x2;
  };

  ap::ThreadPool pool;

  ap::Float last_pi = pi - f(1);
  while (last_pi != pi) {
    last_pi = pi;

    std::vector<ap::Integer> inputs(4);
    for (size_t i = 0; i < inputs.size(); i++) {
      inputs[i] = k + i;
    }
    k = k + inputs.size();

    pi = pool.MapReduce(gen_fn, add, pi, inputs);

    // cout << pi << endl;
  }

  cout << "converged in " << (time_now() - time_start) << endl;
  cout << pi << endl;
}
