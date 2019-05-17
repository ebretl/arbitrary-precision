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
  ap::Integer k = 0;
  ap::Float acc = f(0);

  std::function gen_fn = [f](ap::Integer k) -> ap::Float {
    ap::Float res = (f(4)/f(8*k+1) - f(2)/f(8*k+4) - f(1)/f(8*k+5) - f(1)/f(8*k+6)) / f(ap::Integer(16).Pow(k));
    return res;
  };

  // std::mutex acc_mutex;
  // ap::Float* acc_ptr = &acc;
  // std::function integrate_fn = [acc_ptr, &acc_mutex](ap::Float x) -> void {
  //   std::lock_guard lock(acc_mutex);
  //   *acc_ptr = *acc_ptr + x;
  //   cout << *acc_ptr << endl;
  // };

  ap::ThreadPool pool(4, gen_fn);
  std::deque<std::shared_ptr<ap::Future<ap::Float>>> futures;

  bool done = false;
  while (!done) {
    if (pool.IsAvailable()) {
      for (int i = 0; i < 100; i++) {
        futures.push_back(pool.Apply(k));
        k = k + 1;
      }
    }

    while (!futures.empty() && futures.front()->Available()) {
      auto new_acc = acc + futures.front()->Get();
      if (new_acc == acc) {
        done = true;
      }
      acc = new_acc;
      // cout << acc << endl;
      futures.pop_front();
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }

  cout << "converged in " << (time_now() - time_start) << endl;
}
