/**
 ******************************************************************************
 * Xenia : Xbox 360 Emulator Research Project                                 *
 ******************************************************************************
 * Copyright 2017 Ben Vanik. All rights reserved.                             *
 * Released under the BSD license - see LICENSE in the root for more details. *
 ******************************************************************************
 */

#include "xenia/ui/loop_linux.h"

namespace xe {
namespace ui {

std::unique_ptr<Loop> Loop::Create() { return std::make_unique<LinuxLoop>(); }

LinuxLoop::LinuxLoop() : thread_id_(0) {}

LinuxLoop::~LinuxLoop() {
  Quit();
}

bool LinuxLoop::is_on_loop_thread() {
  return thread_id_ == threading::current_thread_system_id();
}

void LinuxLoop::Post(std::function<void()> fn) {}

void LinuxLoop::PostDelayed(std::function<void()> fn, uint64_t delay_millis) {}

void LinuxLoop::Quit() {}

void LinuxLoop::AwaitQuit() {}

}  // namespace ui
}  // namespace xe
