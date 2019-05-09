#include <iostream>
#include <chrono>
#include <cmath>
#include <thread>
#include <atomic>
#include <deque>
#include <tuple>

#include <arbitrary_precision/arbitrary_precision.h>


using std::cout, std::endl;
using namespace ap;

double time_now() {
  return std::chrono::duration<double>(std::chrono::system_clock::now().time_since_epoch()).count();
}

int main() {
  auto t_start = time_now();

  Float::Factory f(1000);

  Float acc = f(0);
  Integer k = 0;

  // std::condition_variable cond;
  // std::mutex m;
  // std::unique_lock<std::mutex> lock(m);
  auto add_fn = [&f](const Integer& k, std::atomic_bool* finished, Float* partial) {
    *partial = (f(4)/f(8*k+1) - f(2)/f(8*k+4) - f(1)/f(8*k+5) - f(1)/f(8*k+6)) / f(Integer(16).Pow(k));
    *finished = true;
  };

  std::deque<std::tuple<std::atomic_bool, Float, bool>> futures;
  int free_cores = 2;
  bool done = false;

  while (!done) {
    while (free_cores > 0) {
      auto& [processed, x, integrated] = futures.emplace_back(false, f(0), false);
      std::thread(add_fn, k, &processed, &x).detach();
      free_cores--;
      k = k + 1;

      if (k % 50 == 0) {
        cout << acc << endl;
      }
    }

    for (auto& future : futures) {
      auto& [processed, x, integrated] = future;
      if (processed && !integrated) {
        if (x == f(0)) {
          done = true;
        }
        acc = acc + x;
        free_cores++;
        integrated = true;
      }
    }

    while (std::get<2>(futures.front())) {
      futures.pop_front();
    }

    std::this_thread::sleep_for(std::chrono::duration<double>(0.01));
  }

  for (auto& future : futures) {
    auto& [processed, x, integrated] = future;
    while (!processed) {
      std::this_thread::sleep_for(std::chrono::duration<double>(0.01));
    }
    acc = acc + x;
  }

  cout << "converged in " << (time_now() - t_start) << " seconds" << endl;
  cout << acc << endl;
}
