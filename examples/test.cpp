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
  std::tuple<> t1 = std::make_tuple();
  std::tuple<int> t2 = std::make_tuple(1);
  auto cat = std::tuple_cat(t1, t2);
  auto [x] = cat;
  cout << x << endl;

  ap::ThreadPool tps(4);

  std::mt19937 rand{std::random_device{}()};
  std::uniform_int_distribution d(5, 2000);

  auto func = [&](int x, char c) -> int {
    std::this_thread::sleep_for(std::chrono::milliseconds(d(rand)));
    cout << x << " " << c << endl;
    return x * x;
  };

  std::vector<int> input1(15);
  std::vector<char> input2(20);
  for (size_t i = 0; i < input1.size(); i++) {
    // input[i] = std::make_tuple(i);
    input1[i] = i;
    input2[i] = 'a' + i;
  }

  auto result = tps.Map(func, input1, input2);

  cout << "results" << endl;

  for (const auto& x : result) {
    cout << x << endl;
  }

}
