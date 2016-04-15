/**
 ******************************************************************************
 * Xenia : Xbox 360 Emulator Research Project                                 *
 ******************************************************************************
 * Copyright 2014 Ben Vanik. All rights reserved.                             *
 * Released under the BSD license - see LICENSE in the root for more details. *
 ******************************************************************************
 */

#include "xenia/base/mapped_memory.h"

#include <sys/mman.h>
#include <cstdio>
#include <mutex>

#include "xenia/base/string.h"

namespace xe {

class PosixMappedMemory : public MappedMemory {
 public:
  PosixMappedMemory(const std::wstring& path, Mode mode)
      : MappedMemory(path, mode), file_handle(nullptr) {}

  ~PosixMappedMemory() override {
    if (data_) {
      munmap(data_, size_);
    }
    if (file_handle) {
      fclose(file_handle);
    }
  }

  FILE* file_handle;
};

std::unique_ptr<MappedMemory> MappedMemory::Open(const std::wstring& path,
                                                 Mode mode, size_t offset,
                                                 size_t length) {
  const char* mode_str;
  int prot;
  switch (mode) {
    case Mode::kRead:
      mode_str = "rb";
      prot = PROT_READ;
      break;
    case Mode::kReadWrite:
      mode_str = "r+b";
      prot = PROT_READ | PROT_WRITE;
      break;
  }

  auto mm = std::make_unique<PosixMappedMemory>(path, mode);

  mm->file_handle = fopen(xe::to_string(path).c_str(), mode_str);
  if (!mm->file_handle) {
    return nullptr;
  }

  size_t map_length;
  map_length = length;
  if (!length) {
    fseeko(mm->file_handle, 0, SEEK_END);
    map_length = ftello(mm->file_handle);
    fseeko(mm->file_handle, 0, SEEK_SET);
  }
  mm->size_ = map_length;

  mm->data_ =
      mmap(0, map_length, prot, MAP_SHARED, fileno(mm->file_handle), offset);
  if (!mm->data_) {
    return nullptr;
  }

  return std::move(mm);
}

class PosixChunkedMappedMemoryWriter : public ChunkedMappedMemoryWriter {
 public:
  PosixChunkedMappedMemoryWriter(const std::wstring& path, size_t chunk_size,
                                 bool low_address_space)
      : ChunkedMappedMemoryWriter(path, chunk_size, low_address_space) {}

  ~PosixChunkedMappedMemoryWriter() override {
    std::lock_guard<std::mutex> lock(mutex_);
    chunks_.clear();
  }

  uint8_t* Allocate(size_t length) override {
    std::lock_guard<std::mutex> lock(mutex_);
    if (!chunks_.empty()) {
      uint8_t* result = chunks_.back()->Allocate(length);
      if (result != nullptr) {
        return result;
      }
    }
    auto chunk = std::make_unique<Chunk>(chunk_size_);
    auto chunk_path = path_ + L"." + std::to_wstring(chunks_.size());
    if (!chunk->Open(chunk_path, low_address_space_)) {
      return nullptr;
    }
    uint8_t* result = chunk->Allocate(length);
    chunks_.push_back(std::move(chunk));
    return result;
  }

  void Flush() override {
    std::lock_guard<std::mutex> lock(mutex_);
    for (auto& chunk : chunks_) {
      chunk->Flush();
    }
  }

  void FlushNew() override {
    std::lock_guard<std::mutex> lock(mutex_);
    for (auto& chunk : chunks_) {
      chunk->FlushNew();
    }
  }

 private:
  class Chunk {
   public:
    explicit Chunk(size_t capacity)
        : data_(nullptr),
          offset_(0),
          capacity_(capacity),
          last_flush_offset_(0) {}

    ~Chunk() {
      if (data_) {
        // TODO(sephiroth99)
      }
    }

    bool Open(const std::wstring& path, bool low_address_space) {
      // TODO(sephiroth99)
      return false;
    }

    uint8_t* Allocate(size_t length) {
      // TODO(sephiroth99)
      return nullptr;
    }

    void Flush() {
      // TODO(sephiroth99)
    }

    void FlushNew() {
      // TODO(sephiroth99)
    }

   private:
    uint8_t* data_;
    size_t offset_;
    size_t capacity_;
    size_t last_flush_offset_;
  };

  std::mutex mutex_;
  std::vector<std::unique_ptr<Chunk>> chunks_;
};

std::unique_ptr<ChunkedMappedMemoryWriter> ChunkedMappedMemoryWriter::Open(
    const std::wstring& path, size_t chunk_size, bool low_address_space) {
  // TODO(sephiroth99)
  return nullptr;
}

}  // namespace xe
