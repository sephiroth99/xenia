/**
 ******************************************************************************
 * Xenia : Xbox 360 Emulator Research Project                                 *
 ******************************************************************************
 * Copyright 2015 Ben Vanik. All rights reserved.                             *
 * Released under the BSD license - see LICENSE in the root for more details. *
 ******************************************************************************
 */

#include "xenia/cpu/stack_walker.h"

#include <gflags/gflags.h>

#include <mutex>

#include "xenia/base/logging.h"
#include "xenia/cpu/backend/backend.h"
#include "xenia/cpu/backend/code_cache.h"
#include "xenia/cpu/processor.h"

DEFINE_bool(debug_symbol_loader, false,
            "Enable dbghelp debug logging and validation.");

namespace xe {
namespace cpu {

bool InitializeStackWalker() {
  // TODO(sephiroth99)
  return false;
}

class LinuxStackWalker : public StackWalker {
 public:
  explicit LinuxStackWalker(backend::CodeCache* code_cache) {
    // Get the boundaries of the code cache so we can quickly tell if a symbol
    // is ours or not.
    // We store these globally so that the Sym* callbacks can access them.
    // They never change, so it's fine even if they are touched from multiple
    // threads.
    code_cache_ = code_cache;
    code_cache_min_ = code_cache_->base_address();
    code_cache_max_ = code_cache_->base_address() + code_cache_->total_size();
  }

  bool Initialize() {
    return InitializeStackWalker();
  }

  size_t CaptureStackTrace(uint64_t* frame_host_pcs, size_t frame_offset,
                           size_t frame_count,
                           uint64_t* out_stack_hash) override {
    if (out_stack_hash) {
      *out_stack_hash = 0;
    }
    // TODO(sephiroth99)
    return 0;
  }

  size_t CaptureStackTrace(void* thread_handle, uint64_t* frame_host_pcs,
                           size_t frame_offset, size_t frame_count,
                           const X64Context* in_host_context,
                           X64Context* out_host_context,
                           uint64_t* out_stack_hash) override {
    // TODO(sephiroth99)
    return 0;
  }

  bool ResolveStack(uint64_t* frame_host_pcs, StackFrame* frames,
                    size_t frame_count) override {
    // TODO(sephiroth99)
    return false;
  }

 private:
  std::mutex dbghelp_mutex_;

  static xe::cpu::backend::CodeCache* code_cache_;
  static uint32_t code_cache_min_;
  static uint32_t code_cache_max_;
};

xe::cpu::backend::CodeCache* LinuxStackWalker::code_cache_ = nullptr;
uint32_t LinuxStackWalker::code_cache_min_ = 0;
uint32_t LinuxStackWalker::code_cache_max_ = 0;

std::unique_ptr<StackWalker> StackWalker::Create(
    backend::CodeCache* code_cache) {
  auto stack_walker = std::make_unique<LinuxStackWalker>(code_cache);
  if (!stack_walker->Initialize()) {
    XELOGE("Unable to initialize stack walker");
    return nullptr;
  }
  return std::unique_ptr<StackWalker>(stack_walker.release());
}

}  // namespace cpu
}  // namespace xe
