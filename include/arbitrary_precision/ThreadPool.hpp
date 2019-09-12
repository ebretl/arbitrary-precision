#pragma once

#include <atomic>
#include <deque>
#include <functional>
#include <future>
#include <thread>
#include <type_traits>

namespace ap {

namespace _thread_pool_metafuncs {

std::tuple<> get_each_arg(int i) { return std::make_tuple<>(); }

template <typename T0, typename... Ts>
std::tuple<T0, Ts...> get_each_arg(int i, const std::vector<T0>& in0,
                                   const std::vector<Ts>&... ins) {
  return std::tuple_cat(std::make_tuple(in0[i]), get_each_arg(i, ins...));
}

template <typename T>
size_t min_list_size(const std::vector<T>& v) {
  return v.size();
}

template <typename T0, typename... Ts>
size_t min_list_size(const std::vector<T0>& vec0,
                     const std::vector<Ts>&... vecs) {
  return std::min(vec0.size(), min_list_size(vecs...));
}

template <typename... _Args>
std::vector<std::tuple<_Args...>> arg_lists_to_star_args_list(
    const std::vector<_Args>&... arg_lists) {
  std::vector<std::tuple<_Args...>> star_args_list(min_list_size(arg_lists...));

  for (size_t i = 0; i < star_args_list.size(); i++) {
    star_args_list[i] = get_each_arg(i, arg_lists...);
  }

  return star_args_list;
}

}  // namespace _thread_pool_metafuncs


class ThreadPool {
 public:
  ThreadPool(size_t n_threads) : free_threads_(n_threads) {}
  ThreadPool() : ThreadPool(std::thread::hardware_concurrency()) {}

  template <typename _Fn, typename... _Args>
  std::vector<std::result_of_t<_Fn(_Args...)>> StarMap(
      const _Fn& f, const std::vector<std::tuple<_Args...>>& args_list) {
    using _Res = std::result_of_t<_Fn(_Args...)>;

    std::vector<_Res> out(args_list.size());
    size_t done_jobs = 0;

    std::unique_lock lock(mutex_);

    for (size_t i = 0; i < args_list.size(); i++) {
      if (free_threads_ <= 0) {
        cond_.wait(lock);
      }

      free_threads_--;

      auto run_job = [&, i] {
        _Res x = std::apply(f, args_list[i]);

        std::unique_lock lock(mutex_);
        out[i] = x;
        free_threads_++;
        done_jobs++;
        cond_.notify_all();
      };

      std::thread(run_job).detach();
    }

    while (done_jobs < args_list.size()) {
      cond_.wait(lock);
    }

    return out;
  }

  template <typename _Fn, typename... _Args>
  std::vector<std::result_of_t<_Fn(_Args...)>> Map(
      const _Fn& f, const std::vector<_Args>&... arg_lists) {
    auto star_args_list =
        _thread_pool_metafuncs::arg_lists_to_star_args_list(arg_lists...);
    return StarMap(f, star_args_list);
  }

  template <typename AccType, typename MapFn, typename ReduceFn,
            typename... MapArgTypes>
  AccType StarMapReduce(
      const MapFn& map_fn, const ReduceFn& reduce_fn, AccType accumulator,
      const std::vector<std::tuple<MapArgTypes...>>& map_args_list) {
    auto acc_ptr = &accumulator;
    std::mutex acc_mutex;

    std::function map_reduce_func = [&,
                                     acc_ptr](const MapArgTypes&... map_args) {
      auto map_result = map_fn(map_args...);

      std::lock_guard lock(acc_mutex);
      *acc_ptr = reduce_fn(*acc_ptr, map_result);

      return 0;  // dummy return type
    };

    StarMap(map_reduce_func, map_args_list);

    return accumulator;
  }

  template <typename AccType, typename MapFn, typename ReduceFn,
            typename... MapArgTypes>
  AccType MapReduce(const MapFn& map_fn, const ReduceFn& reduce_fn,
                    const AccType& accumulator,
                    const std::vector<MapArgTypes>&... map_arg_lists) {
    auto star_args_list =
        _thread_pool_metafuncs::arg_lists_to_star_args_list(map_arg_lists...);
    return StarMapReduce(map_fn, reduce_fn, accumulator, star_args_list);
  }

 private:
  int free_threads_;

  std::mutex mutex_;
  std::condition_variable_any cond_;
};


}  // namespace ap
