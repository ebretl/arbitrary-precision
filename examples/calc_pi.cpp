#include <iostream>
#include <chrono>
#include <cmath>
#include <thread>
#include <atomic>
#include <deque>
#include <tuple>
#include <mutex>
#include <condition_variable>

#include <arbitrary_precision/arbitrary_precision.h>


using std::cout, std::endl;
using namespace ap;

double time_now() {
  return std::chrono::duration<double>(std::chrono::system_clock::now().time_since_epoch()).count();
}

int main() {
  auto t_start = time_now();

  Float::Factory f(150);

  Float acc = f(0);
  Integer k = 0;

  std::condition_variable cond;
  std::mutex m;
  std::atomic<int> free_cores = 4;
  auto add_fn = [&f, &cond, &free_cores, &m](const Integer& k, bool* finished, Float* partial) {
    free_cores--;

    *partial = (f(4)/f(8*k+1) - f(2)/f(8*k+4) - f(1)/f(8*k+5) - f(1)/f(8*k+6)) / f(Integer(16).Pow(k));
    *finished = true;

    std::unique_lock<std::mutex> lock(m);
    free_cores++;
    cond.notify_all();
  };

  std::deque<std::tuple<bool, Float, bool>> futures;
  bool done = false;

  while (!done) {
    bool print = false;

    for (int i = 0; i < free_cores; i++) {
      auto& [processed, x, integrated] = futures.emplace_back(false, f(0), false);
      std::thread(add_fn, k, &processed, &x).detach();
      k = k + 1;

      if (k % 10 == 0) {
        print = true;
      }
    }

    for (auto& future : futures) {
      auto& [processed, x, integrated] = future;
      if (processed && !integrated) {
        if (x == f(0)) {
          done = true;
        }
        acc = acc + x;
        integrated = true;
      }
    }

    if (print) {
      cout << acc << endl;
    }

    while (std::get<2>(futures.front())) {
      futures.pop_front();
    }

    std::unique_lock<std::mutex> lock(m);
    cond.wait(lock);
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
