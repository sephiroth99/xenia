/**
 ******************************************************************************
 * Xenia : Xbox 360 Emulator Research Project                                 *
 ******************************************************************************
 * Copyright 2014 Ben Vanik. All rights reserved.                             *
 * Released under the BSD license - see LICENSE in the root for more details. *
 ******************************************************************************
 */

#include "xenia/base/threading.h"

#include <pthread.h>
#include <sched.h>
#include <time.h>
#include <unistd.h>

namespace xe {
namespace threading {

void EnableAffinityConfiguration() {
  // TODO(sephiroth99)
}

uint32_t current_thread_system_id() {
  return static_cast<uint32_t>(pthread_self());
}

void set_name(const std::string& name) {
  pthread_setname_np(pthread_self(), name.c_str());
}

void set_name(std::thread::native_handle_type handle, const std::string& name) {
  // TODO(sephiroth99): use handle
  // pthread_setname_np(pthread_self(), name.c_str());
}

void MaybeYield() { sched_yield(); }

void SyncMemory() {
  // TODO(sephiroth99)
}

void Sleep(std::chrono::microseconds duration) {
  timespec rqtp = {duration.count() / 1000000, duration.count() % 1000};
  nanosleep(&rqtp, nullptr);
  // TODO(benvanik): spin while rmtp >0?
}

SleepResult AlertableSleep(std::chrono::microseconds duration) {
  // TODO(sephiroth99)
  return SleepResult::kSuccess;
}

class PosixHighResolutionTimer : public HighResolutionTimer {
 public:
  PosixHighResolutionTimer(std::function<void()> callback)
      : callback_(callback) {}
  ~PosixHighResolutionTimer() override {}

  bool Initialize(std::chrono::milliseconds period) {
    // TODO(sephiroth99)
    return false;
  }

 private:
  std::function<void()> callback_;
};

std::unique_ptr<HighResolutionTimer> HighResolutionTimer::CreateRepeating(
    std::chrono::milliseconds period, std::function<void()> callback) {
  auto timer = std::make_unique<PosixHighResolutionTimer>(std::move(callback));
  if (!timer->Initialize(period)) {
    return nullptr;
  }
  return std::unique_ptr<HighResolutionTimer>(timer.release());
}

template <typename T>
class PosixHandle : public T {
 public:
  explicit PosixHandle(int handle) : handle_(handle) {}
  // TODO(sephiroth99): no POSIX common handle, each kind will need to destroy
  // itself
  ~PosixHandle() override {
    close(handle_);
    handle_ = -1;
  }

 protected:
  void* native_handle() const override {
    return reinterpret_cast<void*>(handle_);
  }

  int handle_ = -1;
};

WaitResult Wait(WaitHandle* wait_handle, bool is_alertable,
                std::chrono::milliseconds timeout) {
  // TODO(sephiroth99)
  return WaitResult::kFailed;
}

WaitResult SignalAndWait(WaitHandle* wait_handle_to_signal,
                         WaitHandle* wait_handle_to_wait_on, bool is_alertable,
                         std::chrono::milliseconds timeout) {
  // TODO(sephiroth99)
  return WaitResult::kFailed;
}

std::pair<WaitResult, size_t> WaitMultiple(WaitHandle* wait_handles[],
                                           size_t wait_handle_count,
                                           bool wait_all, bool is_alertable,
                                           std::chrono::milliseconds timeout) {
  // TODO(sephiroth99)
  return std::pair<WaitResult, size_t>(WaitResult::kFailed, 0);
}

class PosixEvent : public PosixHandle<Event> {
 public:
  explicit PosixEvent(int handle) : PosixHandle(handle) {}
  ~PosixEvent() override = default;
  void Set() override {}
  void Reset() override {}
  void Pulse() override {}
};

std::unique_ptr<Event> Event::CreateManualResetEvent(bool initial_state) {
  // TODO(sephiroth99)
  return std::make_unique<PosixEvent>(-1);
}

std::unique_ptr<Event> Event::CreateAutoResetEvent(bool initial_state) {
  // TODO(sephiroth99)
  return std::make_unique<PosixEvent>(-1);
}

class PosixSemaphore : public PosixHandle<Semaphore> {
 public:
  explicit PosixSemaphore(int handle) : PosixHandle(handle) {}
  ~PosixSemaphore() override = default;
  bool Release(int release_count, int* out_previous_count) override {
    // TODO(sephiroth99)
    return false;
  }
};

std::unique_ptr<Semaphore> Semaphore::Create(int initial_count,
                                             int maximum_count) {
  // TODO(sephiroth99)
  return std::make_unique<PosixSemaphore>(-1);
}

class PosixMutant : public PosixHandle<Mutant> {
 public:
  explicit PosixMutant(int handle) : PosixHandle(handle) {}
  ~PosixMutant() = default;
  bool Release() override {
    // TODO(sephiroth99)
    return false;
  }
};

std::unique_ptr<Mutant> Mutant::Create(bool initial_owner) {
  // TODO(sephiroth99)
  return std::make_unique<PosixMutant>(-1);
}

class PosixTimer : public PosixHandle<Timer> {
 public:
  explicit PosixTimer(int handle) : PosixHandle(handle) {}
  ~PosixTimer() = default;
  bool SetOnce(std::chrono::nanoseconds due_time,
               std::function<void()> opt_callback) override {
    // TODO(sephiroth99)
    return false;
  }
  bool SetRepeating(std::chrono::nanoseconds due_time,
                    std::chrono::milliseconds period,
                    std::function<void()> opt_callback) override {
    // TODO(sephiroth99)
    return false;
  }
  bool Cancel() override {
    // TODO(sephiroth99)
    return false;
  }

 private:
  std::mutex mutex_;
  std::function<void()> callback_;
};

std::unique_ptr<Timer> Timer::CreateManualResetTimer() {
  // TODO(sephiroth99)
  return std::make_unique<PosixTimer>(-1);
}

std::unique_ptr<Timer> Timer::CreateSynchronizationTimer() {
  // TODO(sephiroth99)
  return std::make_unique<PosixTimer>(-1);
}

class PosixThread : public PosixHandle<Thread> {
 public:
  explicit PosixThread(pthread_t handle) : PosixHandle(handle) {}
  ~PosixThread() = default;

  void set_name(std::string name) override {
    xe::threading::set_name(handle_, name);
    Thread::set_name(name);
  }

  int32_t priority() override {
    // TODO(sephiroth99)
    return 0;
  }
  uint32_t system_id() const override {
    // TODO(sephiroth99)
    return 0;
  }

  void set_priority(int32_t new_priority) override {
    // TODO(sephiroth99)
  }

  uint64_t affinity_mask() override {
    // TODO(sephiroth99)
    return 0;
  }

  void set_affinity_mask(uint64_t new_affinity_mask) override {
    // TODO(sephiroth99)
  }

  void QueueUserCallback(std::function<void()> callback) override {
    // TODO(sephiroth99)
  }

  bool Resume(uint32_t* out_new_suspend_count = nullptr) override {
    // TODO(sephiroth99)
    return false;
  }

  bool Suspend(uint32_t* out_previous_suspend_count = nullptr) override {
    // TODO(sephiroth99)
    return false;
  }

  void Terminate(int exit_code) override {
    // TODO(sephiroth99)
  }

 private:
  void AssertCallingThread() {
    // TODO(sephiroth99)
  }
};

std::unique_ptr<Thread> Thread::Create(CreationParameters params,
                                       std::function<void()> start_routine) {
  // TODO(sephiroth99)
  return std::make_unique<PosixThread>(0);
}

Thread* Thread::GetCurrentThread() {
  // TODO(sephiroth99)
  return nullptr;
}

void Thread::Exit(int exit_code) {
  // TODO(sephiroth99)
}

}  // namespace threading
}  // namespace xe
