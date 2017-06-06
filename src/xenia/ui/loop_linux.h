/**
 ******************************************************************************
 * Xenia : Xbox 360 Emulator Research Project                                 *
 ******************************************************************************
 * Copyright 2017 Ben Vanik. All rights reserved.                             *
 * Released under the BSD license - see LICENSE in the root for more details. *
 ******************************************************************************
 */

#ifndef XENIA_UI_LOOP_LINUX_H_
#define XENIA_UI_LOOP_LINUX_H_

#include "xenia/base/threading.h"
#include "xenia/ui/loop.h"

namespace xe {
namespace ui {

class LinuxLoop : public Loop {
 public:
  LinuxLoop();
  ~LinuxLoop() override;

  bool is_on_loop_thread() override;

  void Post(std::function<void()> fn) override;
  void PostDelayed(std::function<void()> fn, uint64_t delay_millis) override;

  void Quit() override;
  void AwaitQuit() override;

 private:
  uint32_t thread_id_;
};

}  // namespace ui
}  // namespace xe

#endif  // XENIA_UI_LOOP_LINUX_H_
