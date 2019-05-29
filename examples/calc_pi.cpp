#include <iostream>
#include <tuple>
#include <numeric>
#include <fstream>

#include <arbitrary_precision/arbitrary_precision.h>

using std::cout, std::endl;


double time_now() {
  return std::chrono::duration<double>(std::chrono::system_clock::now().time_since_epoch()).count();
}

int main() {
  using ap::Integer;
  using Float = ap::Float<10000>;

  std::ofstream outfile("output.txt");
  // outfile.clear();

  auto time_start = time_now();

  Integer k = 0;
  Float pi = 0;

  // auto gen_fn = [](Integer k) {
  //   return (Float(4)/(8*k+1) - Float(2)/(8*k+4) - Float(1)/(8*k+5) - Float(1)/(8*k+6)) / ap::Integer(16).Pow(k);
  // };

  std::vector<Float> pows_2(17);
  std::iota(pows_2.begin(), pows_2.end(), 0);
  std::transform(pows_2.begin(), pows_2.end(), pows_2.begin(), [](Float x){ return Float(2).Pow(x); });

  auto gen_fn = [&pows_2](Integer k) -> Float {
    Integer k4 = k * 4;
    Integer k10 = k * 10;
    Float res = pows_2[8]/(k10+1);
    res += Float(1)/(k10+9);
    res -= pows_2[5]/(k4+1);
    res -= Float(1)/(k4*k+3);
    res -= pows_2[6]/(k10+3);
    res -= Float(4)/(k10+5);
    res -= Float(4)/(k10+7);
    res /= (pows_2[10].Pow(k) * pows_2[6]);
    if (k % 2 != 0) {
      res *= -1;
    }
    return res;
  };

  // auto gen_fn = [&pows_2](Integer k) -> Float {
  //   Integer k32 = k * 32;
  //   Float x = pows_2[15] / (k32 + 2);
  //   x += pows_2[14] / (k32 + 3);
  //   x += pows_2[14] / (k32 + 4);
  //   x -= pows_2[12] / (k32 + 7);
  //   x += pows_2[11] / (k32 + 10);
  //   x += pows_2[10] / (k32 + 11);
  //   x += pows_2[10] / (k32 + 12);
  //   x -= pows_2[8] / (k32 + 15);
  //   x += pows_2[7] / (k32 + 18);
  //   x += pows_2[6] / (k32 + 19);
  //   x += pows_2[6] / (k32 + 20);
  //   x -= pows_2[4] / (k32 + 23);
  //   x += pows_2[3] / (k32 + 26);
  //   x += pows_2[2] / (k32 + 27);
  //   x += pows_2[2] / (k32 + 28);
  //   x -= Float(1) / (k32 + 31);
  //   // x /= Float(2).Pow(16 * k + 13);
  //   x /= (pows_2[16].Pow(k) * pows_2[13]);
  //   return x;
  // };

  auto add = [](const Float& x1, const Float& x2) {
    return x1 + x2;
  };

  ap::ThreadPool pool;

  ap::Float last_pi = pi - 1;
  while (last_pi != pi) {
    last_pi = pi;

    std::vector<ap::Integer> inputs(4);
    std::iota(inputs.begin(), inputs.end(), k);
    k = k + inputs.size();

    pi = pool.MapReduce(gen_fn, add, pi, inputs);

    cout << pi << endl;
    outfile << pi << "\n";
  }

  outfile << "converged in " << (time_now() - time_start) << endl;
  outfile << pi << endl;

  outfile.close();
}
