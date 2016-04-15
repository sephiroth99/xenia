/**
 ******************************************************************************
 * Xenia : Xbox 360 Emulator Research Project                                 *
 ******************************************************************************
 * Copyright 2016 Ben Vanik. All rights reserved.                             *
 * Released under the BSD license - see LICENSE in the root for more details. *
 ******************************************************************************
 */

#include "xenia/ui/loop_linux.h"

#include "xenia/base/assert.h"

namespace xe {
namespace ui {

class PostedFn {
 public:
  explicit PostedFn(std::function<void()> fn) : fn_(std::move(fn)) {}
  void Call() { fn_(); }

 private:
  std::function<void()> fn_;
};

std::unique_ptr<Loop> Loop::Create() { return std::make_unique<LinuxLoop>(); }

LinuxLoop::LinuxLoop() : thread_id_(0) {
  xe::threading::Fence init_fence;
  thread_ = std::thread([&init_fence, this]() {
    xe::threading::set_name("Linux Loop");
    thread_id_ = threading::current_thread_system_id();

    init_fence.Signal();

    ThreadMain();

    quit_fence_.Signal();
  });
  init_fence.Wait();
}

LinuxLoop::~LinuxLoop() {
  Quit();
  thread_.join();

  std::lock_guard<std::mutex> lock(pending_timers_mutex_);
  while (!pending_timers_.empty()) {
    auto timer = pending_timers_.back();
    pending_timers_.pop_back();
    delete timer;
  }
}

void LinuxLoop::ThreadMain() {
  // TODO(sephiroth99): Implement.
  assert_always();

  UIEvent e(nullptr);
  on_quit(&e);
}

bool LinuxLoop::is_on_loop_thread() {
  return thread_id_ == threading::current_thread_system_id();
}

void LinuxLoop::Post(std::function<void()> fn) {
  assert_true(thread_id_ != 0);
  // TODO(sephiroth99): Implement.
  assert_always();
}

void LinuxLoop::PostDelayed(std::function<void()> fn, uint64_t delay_millis) {
  // TODO(sephiroth99): Implement.
  assert_always();
}

void LinuxLoop::Quit() {
  assert_true(thread_id_ != 0);
  // TODO(sephiroth99): Implement.
  assert_always();
}

void LinuxLoop::AwaitQuit() { quit_fence_.Wait(); }

}  // namespace ui
}  // namespace xe
