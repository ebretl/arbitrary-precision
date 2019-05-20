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
  ap::Float acc = f(0);

  auto acc_ptr = &acc;
  std::mutex mutex;
  auto gen_fn = [f, acc_ptr, &mutex](ap::Integer k) -> bool {
    ap::Float res = (f(4)/f(8*k+1) - f(2)/f(8*k+4) - f(1)/f(8*k+5) - f(1)/f(8*k+6)) / f(ap::Integer(16).Pow(k));

    std::unique_lock lock(mutex);
    *acc_ptr = *acc_ptr + res;
    lock.unlock();

    return res == f(0);
  };

  ap::ThreadPool pool(4);

  bool done = false;
  while (!done) {
    std::vector<ap::Integer> inputs(8);
    for (int i = 0; i < inputs.size(); i++) {
      inputs[i] = k + i;
    }
    k = k + inputs.size();

    std::vector<bool> results = pool.Map(gen_fn, inputs);

    done = std::any_of(results.begin(), results.end(), [](bool b){return b;});

    // cout << acc << endl;
  }

  cout << "converged in " << (time_now() - time_start) << endl;
  cout << acc << endl;
}
