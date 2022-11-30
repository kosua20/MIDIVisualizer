#pragma once
#include <condition_variable>
#include <mutex>

// Based on https://stackoverflow.com/a/27852868/1495627
namespace libremidi
{
class semaphore
{
public:
  explicit semaphore(size_t count = 0) : count_{count}
  {
  }

  semaphore(const semaphore&) = delete;
  semaphore(semaphore&&) = delete;
  semaphore& operator=(const semaphore&) = delete;
  semaphore& operator=(semaphore&&) = delete;

  void notify()
  {
    std::lock_guard<std::mutex> lock{mutex_};
    ++count_;
    cv_.notify_one();
  }

  void wait()
  {
    std::unique_lock<std::mutex> lock{mutex_};
    cv_.wait(lock, [&] { return count_ > 0; });
    --count_;
  }

  bool try_wait()
  {
    std::lock_guard<std::mutex> lock{mutex_};

    if (count_ > 0)
    {
      --count_;
      return true;
    }

    return false;
  }

  template <typename T>
  bool wait_for(const T& d)
  {
    std::unique_lock<std::mutex> lock{mutex_};
    auto finished = cv_.wait_for(lock, d, [&] { return count_ > 0; });

    if (finished)
      --count_;

    return finished;
  }

  template <typename T>
  bool wait_until(const T& t)
  {
    std::unique_lock<std::mutex> lock{mutex_};
    auto finished = cv_.wait_until(lock, t, [&] { return count_ > 0; });

    if (finished)
      --count_;

    return finished;
  }

private:
  std::mutex mutex_;
  std::condition_variable cv_;
  size_t count_;
};
}
