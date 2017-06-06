/**
 ******************************************************************************
 * Xenia : Xbox 360 Emulator Research Project                                 *
 ******************************************************************************
 * Copyright 2017 Ben Vanik. All rights reserved.                             *
 * Released under the BSD license - see LICENSE in the root for more details. *
 ******************************************************************************
 */

#include "xenia/base/filesystem.h"
#include "xenia/base/logging.h"
#include "xenia/base/string.h"

#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

namespace xe {
namespace filesystem {

bool PathExists(const std::wstring& path) {
  struct stat st;
  return stat(xe::to_string(path).c_str(), &st) == 0;
}

bool CreateFile(const std::wstring& path) {
  auto f = OpenFile(path, "w");
  if (!f) {
    return false;
  }
  fclose(f);
  return true;
}

FILE* OpenFile(const std::wstring& path, const char* mode) {
  auto fixed_path = xe::fix_path_separators(path);
  return fopen(xe::to_string(fixed_path).c_str(), mode);
}

bool DeleteFile(const std::wstring& path) {
  auto fixed_path = xe::fix_path_separators(path);
  auto ret = unlink(xe::to_string(fixed_path).c_str());
  return ret == 0;
}

bool CreateFolder(const std::wstring& path) {
  return mkdir(xe::to_string(path).c_str(), 0774);
}

bool DeleteFolder(const std::wstring& path) {
  // TODO(sephiroth99)
  return true;
}

bool IsFolder(const std::wstring& path) {
  auto fixed_path = xe::fix_path_separators(path);
  struct stat st;
  auto ret = stat(xe::to_string(fixed_path).c_str(), &st);
  return ret == 0 && S_ISDIR(st.st_mode);
}

class LinuxFileHandle : public FileHandle {
 public:
  LinuxFileHandle(std::wstring path, FILE* handle)
      : FileHandle(std::move(path)), handle_(handle) {}
  ~LinuxFileHandle() override {
    fclose(handle_);
    handle_ = nullptr;
  }
  bool Read(size_t file_offset, void* buffer, size_t buffer_length,
            size_t* out_bytes_read) override {
    // TODO(sephiroth99)
    return false;
  }
  bool Write(size_t file_offset, const void* buffer, size_t buffer_length,
             size_t* out_bytes_written) override {
    // TODO(sephiroth99)
    return false;
  }
  void Flush() override { fflush(handle_); }

 private:
  FILE* handle_ = nullptr;
};

std::unique_ptr<FileHandle> FileHandle::OpenExisting(std::wstring path,
                                                     uint32_t desired_access) {
  // TODO(sephiroth99)
  return std::make_unique<LinuxFileHandle>(path, nullptr);
}

bool GetInfo(const std::wstring& path, FileInfo* out_info) {
  auto fixed_path = xe::fix_path_separators(path);
  struct stat st;
  auto ret = stat(xe::to_string(fixed_path).c_str(), &st);
  if (ret) {
    return false;
  }

  if (S_ISDIR(st.st_mode)) {
    out_info->type = FileInfo::Type::kDirectory;
    out_info->total_size = 0;
  } else {
    out_info->type = FileInfo::Type::kFile;
    out_info->total_size = st.st_size;
  }

  out_info->name = xe::find_name_from_path(path);
  out_info->create_timestamp = 0;
  out_info->access_timestamp = st.st_atime;
  out_info->write_timestamp = st.st_mtime;
  return true;
}

std::vector<FileInfo> ListFiles(const std::wstring& path) {
  std::vector<FileInfo> result;

  DIR* dir = opendir(xe::to_string(path).c_str());
  if (!dir) {
    return result;
  }

  while (auto ent = readdir(dir)) {
    FileInfo info;
    if (ent->d_type == DT_DIR) {
      info.type = FileInfo::Type::kDirectory;
      info.total_size = 0;
    } else {
      info.type = FileInfo::Type::kFile;
      info.total_size = 0;  // TODO(DrChat): Find a way to get this
    }

    info.create_timestamp = 0;
    info.access_timestamp = 0;
    info.write_timestamp = 0;
    info.name = xe::to_wstring(ent->d_name);
    result.push_back(info);
  }

  return result;
}

}  // namespace filesystem
}  // namespace xe
