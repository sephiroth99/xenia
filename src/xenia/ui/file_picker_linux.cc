/**
 ******************************************************************************
 * Xenia : Xbox 360 Emulator Research Project                                 *
 ******************************************************************************
 * Copyright 2015 Ben Vanik. All rights reserved.                             *
 * Released under the BSD license - see LICENSE in the root for more details. *
 ******************************************************************************
 */

#include "xenia/ui/file_picker.h"

#include "xenia/base/assert.h"

namespace xe {
namespace ui {

class LinuxFilePicker : public FilePicker {
 public:
  LinuxFilePicker();
  ~LinuxFilePicker() override;

  bool Show(void* parent_window_handle) override;

 private:
};

std::unique_ptr<FilePicker> FilePicker::Create() {
  return std::make_unique<LinuxFilePicker>();
}

LinuxFilePicker::LinuxFilePicker() = default;

LinuxFilePicker::~LinuxFilePicker() = default;

bool LinuxFilePicker::Show(void* parent_window_handle) {
  return false;
}

}  // namespace ui
}  // namespace xe
