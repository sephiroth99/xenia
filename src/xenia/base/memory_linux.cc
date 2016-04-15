/**
 ******************************************************************************
 * Xenia : Xbox 360 Emulator Research Project                                 *
 ******************************************************************************
 * Copyright 2015 Ben Vanik. All rights reserved.                             *
 * Released under the BSD license - see LICENSE in the root for more details. *
 ******************************************************************************
 */

#include "xenia/base/memory.h"

namespace xe {
namespace memory {

size_t page_size() {
  // TODO(sephiroth99)
  return 0;
}

size_t allocation_granularity() {
  // TODO(sephiroth99)
  return 0;
}

void* AllocFixed(void* base_address, size_t length,
                 AllocationType allocation_type, PageAccess access) {
  // TODO(sephiroth99)
  return nullptr;
}

bool DeallocFixed(void* base_address, size_t length,
                  DeallocationType deallocation_type) {
  // TODO(sephiroth99)
  return false;
}

bool Protect(void* base_address, size_t length, PageAccess access,
             PageAccess* out_old_access) {
  // TODO(sephiroth99)
  return false;
}

bool QueryProtect(void* base_address, size_t& length, PageAccess& access_out) {
  // TODO(sephiroth99)
  return false;
}

FileMappingHandle CreateFileMappingHandle(std::wstring path, size_t length,
                                          PageAccess access, bool commit) {
  // TODO(sephiroth99)
  return nullptr;
}

void CloseFileMappingHandle(FileMappingHandle handle) {
  // TODO(sephiroth99)
}

void* MapFileView(FileMappingHandle handle, void* base_address, size_t length,
                  PageAccess access, size_t file_offset) {
  // TODO(sephiroth99)
  return nullptr;
}

bool UnmapFileView(FileMappingHandle handle, void* base_address,
                   size_t length) {
  // TODO(sephiroth99)
  return false;
}

}  // namespace memory
}  // namespace xe
