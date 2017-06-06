/**
 ******************************************************************************
 * Xenia : Xbox 360 Emulator Research Project                                 *
 ******************************************************************************
 * Copyright 2017 Ben Vanik. All rights reserved.                             *
 * Released under the BSD license - see LICENSE in the root for more details. *
 ******************************************************************************
 */

#include "xenia/ui/window_linux.h"

namespace xe {
namespace ui {

std::unique_ptr<Window> Window::Create(Loop* loop, const std::wstring& title) {
  return std::make_unique<LinuxWindow>(loop, title);
}

LinuxWindow::LinuxWindow(Loop* loop, const std::wstring& title)
    : Window(loop, title) {}

LinuxWindow::~LinuxWindow() {}

bool LinuxWindow::SetIcon(const void* buffer, size_t size) {
  // TODO(sephiroth99)
  return false;
}

void LinuxWindow::Resize(int32_t width, int32_t height) {}

void LinuxWindow::Resize(int32_t left, int32_t top, int32_t right,
                         int32_t bottom) {}

void LinuxWindow::Close() {}

std::unique_ptr<ui::MenuItem> MenuItem::Create(Type type,
                                               const std::wstring& text,
                                               const std::wstring& hotkey,
                                               std::function<void()> callback) {
  return std::make_unique<LinuxMenuItem>(type, text, hotkey, callback);
}

LinuxMenuItem::LinuxMenuItem(Type type, const std::wstring& text,
                             const std::wstring& hotkey,
                             std::function<void()> callback)
    : MenuItem(type, text, hotkey, std::move(callback)) {}

LinuxMenuItem::~LinuxMenuItem() {}

}  // namespace ui
}  // namespace xe
