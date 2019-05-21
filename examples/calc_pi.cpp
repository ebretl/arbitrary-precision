#include <iostream>
#include <tuple>

#include <arbitrary_precision/arbitrary_precision.h>

using std::cout, std::endl;


double time_now() {
  return std::chrono::duration<double>(std::chrono::system_clock::now().time_since_epoch()).count();
}

int main() {
  using ap::Integer;
  using Float = ap::Float<1000>;

  auto time_start = time_now();

  Integer k = 0;
  Float pi = 0;

  auto gen_fn = [](Integer k) {
    // cout << "starting " << k << endl;
    Float x = (Float(4)/Float(8*k+1) - Float(2)/Float(8*k+4) - Float(1)/Float(8*k+5) - Float(1)/Float(8*k+6))
        / Float(ap::Integer(16).Pow(k));
    cout << "finished " << k << endl;
    return x;
  };

  auto add = [](const Float& x1, const Float& x2) {
    return x1 + x2;
  };

  ap::ThreadPool pool(1);

  ap::Float last_pi = pi - 1;
  while (last_pi != pi) {
    last_pi = pi;

    std::vector<ap::Integer> inputs(30);
    for (size_t i = 0; i < inputs.size(); i++) {
      inputs[i] = k + i;
    }
    k = k + inputs.size();

    pi = pool.MapReduce(gen_fn, add, pi, inputs);

    cout << pi << endl;
  }

  cout << "converged in " << (time_now() - time_start) << endl;
  cout << pi << endl;
}
