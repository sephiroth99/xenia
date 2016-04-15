/**
 ******************************************************************************
 * Xenia : Xbox 360 Emulator Research Project                                 *
 ******************************************************************************
 * Copyright 2015 Ben Vanik. All rights reserved.                             *
 * Released under the BSD license - see LICENSE in the root for more details. *
 ******************************************************************************
 */

#include "xenia/cpu/backend/x64/x64_code_cache.h"

#include <cstdlib>
#include <cstring>

#include "xenia/base/assert.h"
#include "xenia/base/clock.h"
#include "xenia/base/logging.h"
#include "xenia/base/math.h"
#include "xenia/base/memory.h"
#include "xenia/cpu/function.h"

namespace xe {
namespace cpu {
namespace backend {
namespace x64 {

// Size of unwind info per function.
// TODO(benvanik): move this to emitter.
static const uint32_t kUnwindInfoSize = 4 + (2 * 1 + 2 + 2);

class LinuxX64CodeCache : public X64CodeCache {
 public:
  LinuxX64CodeCache();
  ~LinuxX64CodeCache() override;

  bool Initialize() override;

  void* LookupUnwindInfo(uint64_t host_pc) override;

 private:
  UnwindReservation RequestUnwindReservation(uint8_t* entry_address) override;
  void PlaceCode(uint32_t guest_address, void* machine_code, size_t code_size,
                 size_t stack_size, void* code_address,
                 UnwindReservation unwind_reservation) override;

  void InitializeUnwindEntry(uint8_t* unwind_entry_address,
                             size_t unwind_table_slot, void* code_address,
                             size_t code_size, size_t stack_size);

  // Growable function table system handle.
  void* unwind_table_handle_ = nullptr;
  // Actual unwind table entries. TODO(sephiroth99): fix type
  std::vector<void*> unwind_table_;
  // Current number of entries in the table.
  std::atomic<uint32_t> unwind_table_count_ = {0};
};

std::unique_ptr<X64CodeCache> X64CodeCache::Create() {
  return std::make_unique<LinuxX64CodeCache>();
}

LinuxX64CodeCache::LinuxX64CodeCache() = default;

LinuxX64CodeCache::~LinuxX64CodeCache() {
  // TODO(sephiroth99)
}

bool LinuxX64CodeCache::Initialize() {
  if (!X64CodeCache::Initialize()) {
    return false;
  }

  // Compute total number of unwind entries we should allocate.
  // We don't support reallocing right now, so this should be high.
  unwind_table_.resize(kMaximumFunctionCount);

  // TODO(sephiroth99)
  return false;
}

LinuxX64CodeCache::UnwindReservation
LinuxX64CodeCache::RequestUnwindReservation(uint8_t* entry_address) {
  UnwindReservation unwind_reservation;
  unwind_reservation.data_size = xe::round_up(kUnwindInfoSize, 16);
  unwind_reservation.table_slot = ++unwind_table_count_;
  unwind_reservation.entry_address = entry_address;
  assert_false(unwind_table_count_ >= kMaximumFunctionCount);

  return unwind_reservation;
}

void LinuxX64CodeCache::PlaceCode(uint32_t guest_address, void* machine_code,
                                  size_t code_size, size_t stack_size,
                                  void* code_address,
                                  UnwindReservation unwind_reservation) {
  // TODO(sephiroth99)
}

void LinuxX64CodeCache::InitializeUnwindEntry(uint8_t* unwind_entry_address,
                                              size_t unwind_table_slot,
                                              void* code_address,
                                              size_t code_size,
                                              size_t stack_size) {
  // TODO(sephiroth99)
}

void* LinuxX64CodeCache::LookupUnwindInfo(uint64_t host_pc) {
  // TODO(sephiroth99)
  return nullptr;
}

}  // namespace x64
}  // namespace backend
}  // namespace cpu
}  // namespace xe
