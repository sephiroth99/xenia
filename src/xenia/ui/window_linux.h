/**
 ******************************************************************************
 * Xenia : Xbox 360 Emulator Research Project                                 *
 ******************************************************************************
 * Copyright 2017 Ben Vanik. All rights reserved.                             *
 * Released under the BSD license - see LICENSE in the root for more details. *
 ******************************************************************************
 */

#ifndef XENIA_UI_WINDOW_LINUX_H_
#define XENIA_UI_WINDOW_LINUX_H_

#include "xenia/ui/menu_item.h"
#include "xenia/ui/window.h"

namespace xe {
namespace ui {

class LinuxWindow : public Window {
  using super = Window;

 public:
  LinuxWindow(Loop* loop, const std::wstring& title);
  ~LinuxWindow() override;

  NativePlatformHandle native_platform_handle() const override { return nullptr; }
  NativeWindowHandle native_handle() const override { return nullptr; }

  bool SetIcon(const void* buffer, size_t size) override;

  void Resize(int32_t width, int32_t height) override;
  void Resize(int32_t left, int32_t top, int32_t right,
              int32_t bottom) override;

  void Close() override;
};

class LinuxMenuItem : public MenuItem {
 public:
  LinuxMenuItem(Type type, const std::wstring& text, const std::wstring& hotkey,
                std::function<void()> callback);
  ~LinuxMenuItem() override;
};

}  // namespace ui
}  // namespace xe

#endif  // XENIA_UI_WINDOW_LINUX_H_
