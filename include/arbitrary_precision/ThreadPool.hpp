#pragma once

#include <functional>
#include <thread>
#include <atomic>
#include <future>
#include <deque>

namespace ap {


template <typename ResultT>
struct Future {
  ResultT data_;
  bool ready_;
  std::recursive_mutex mutex_;
  std::condition_variable_any cond_;

  Future() {
    ready_ = false;
  }

  Future(const Future& other) = delete;

  bool Available() {
    std::lock_guard lock(mutex_);
    return ready_;
  }

  ResultT& Get() {
    std::lock_guard lock(mutex_);
    return data_;
  }

  void Set(const ResultT& other) {
    std::lock_guard lock(mutex_);
    data_ = other;
    ready_ = true;
    // std::cout << "set" << std::endl;
    cond_.notify_all();
  }

  void DoWhenComplete(std::function<void(ResultT&)> f) {
    std::thread([this, f] {
      std::unique_lock lock(mutex_);
      while (!Available()) {
        cond_.wait_for(lock, std::chrono::duration<double>(0.1));
      }
      f(Get());
    }).detach();
  }
};


template <typename ResultT, typename... Args>
class ThreadPool {
public:


  ThreadPool(unsigned int jobs, std::function<ResultT(Args...)> function) {
    available_jobs_ = jobs;
    func = function;
    done_ = false;

    auto f = [this] {
      __run_jobs_thread();
    };
    std::thread(f).detach();
  }

  ~ThreadPool() {
    done_ = true;

    std::unique_lock<std::mutex> lock(mutex_);
    cond_.notify_all();
  }

  void __run_jobs_thread() {
    std::unique_lock<std::mutex> lock(mutex_);

    while (!done_) {

      if (!start_queue_.empty() && available_jobs_ > 0) {
        std::tuple<Args...> args = start_queue_.front();
        start_queue_.pop_front();
        auto future_ptr = finish_queue_.front();
        finish_queue_.pop_front();
        available_jobs_--;

        auto f = [this, args, future_ptr] {
          auto res = std::apply(func, args);
          future_ptr->Set(res);

          std::unique_lock<std::mutex> lock(mutex_);
          available_jobs_++;
          cond_.notify_all();
        };
        std::thread(f).detach();
        // std::cout << "started new thread " << std::get<0>(args) << std::endl;
      }

      else {
        cond_.wait_for(lock, std::chrono::duration<double>(1.0));  // wait for job start or finish event
      }
    }
  }

  bool IsAvailable() {
    return available_jobs_ > 0;
  }

  std::shared_ptr<Future<ResultT>> Apply(Args... args) {
    std::unique_lock<std::mutex> lock(mutex_);
    start_queue_.push_back(std::make_tuple(args...));
    auto future_ptr = std::make_shared<Future<ResultT>>();
    finish_queue_.push_back(future_ptr);
    // std::cout << std::get<0>(start_queue_.back()) << std::endl;
    cond_.notify_all();
    return finish_queue_.back();
  }

private:
  std::function<ResultT(Args...)> func;

  std::deque<std::tuple<Args...>> start_queue_;
  std::deque<std::shared_ptr<Future<ResultT>>> finish_queue_;

  std::atomic<int> available_jobs_;
  std::condition_variable cond_;
  std::mutex mutex_;
  std::atomic<bool> done_;
};

}
