/**
 ******************************************************************************
 * Xenia : Xbox 360 Emulator Research Project                                 *
 ******************************************************************************
 * Copyright 2016 Ben Vanik. All rights reserved.                             *
 * Released under the BSD license - see LICENSE in the root for more details. *
 ******************************************************************************
 */

#include "xenia/base/filesystem.h"

#include <cstdio>
#include <string>

#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>

#include "xenia/base/assert.h"

namespace xe {
namespace filesystem {

bool PathExists(const std::wstring& path) {
  auto fixed_path = xe::fix_path_separators(to_string(path));
  auto ret = access(fixed_path.c_str(), F_OK);
  return ret == 0;
}

bool CreateFolder(const std::wstring& path) {
  // TODO(sephiroth99)
  assert_always();
  return PathExists(path);
}

bool DeleteFolder(const std::wstring& path) {
  // TODO(sephiroth99)
  assert_always();
  return true;
}

bool IsFolder(const std::wstring& path) {
  auto fixed_path = xe::fix_path_separators(to_string(path));
  struct stat s;
  auto ret = stat(fixed_path.c_str(), &s);
  return ret == 0 && S_ISDIR(s.st_mode);
}

bool CreateFile(const std::wstring& path) {
  auto f = OpenFile(path, "w");
  if (!f)
    return false;
  fclose(f);
  return true;
}

FILE* OpenFile(const std::wstring& path, const char* mode) {
  auto fixed_path = xe::fix_path_separators(to_string(path));
  return fopen(fixed_path.c_str(), mode);
}

bool DeleteFile(const std::wstring& path) {
  auto fixed_path = xe::fix_path_separators(to_string(path));
  auto ret = unlink(fixed_path.c_str());
  return ret == 0;
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
    assert_always();
    return false;
  }
  bool Write(size_t file_offset, const void* buffer, size_t buffer_length,
             size_t* out_bytes_written) override {
    // TODO(sephiroth99)
    assert_always();
    return false;
  }
  void Flush() override { fflush(handle_); }

 private:
  FILE* handle_ = nullptr;
};

std::unique_ptr<FileHandle> FileHandle::OpenExisting(std::wstring path,
                                                     uint32_t desired_access) {
  // TODO(sephiroth99)
  assert_always();
  return std::make_unique<LinuxFileHandle>(path, nullptr);
}

bool GetInfo(const std::wstring& path, FileInfo* out_info) {
  // TODO(sephiroth99)
  assert_always();
  return false;
}

std::vector<FileInfo> ListFiles(const std::wstring& path) {
  std::vector<FileInfo> result;
  struct dirent* e;

  std::string fixed_path = xe::fix_path_separators(xe::to_string(path));
  auto dirp = opendir(fixed_path.c_str());
  if (!dirp)
    return result;

  errno = 0;
  while ((e = readdir(dirp)) != nullptr) {
    struct stat s;
    FileInfo info;

    auto name = std::string(e->d_name);
    auto ret = stat(join_paths(fixed_path, name).c_str(), &s);
    if (ret)
      break;

    if (S_ISDIR(s.st_mode)) {
      info.type = FileInfo::Type::kDirectory;
      info.total_size = 0;
    } else if (S_ISREG(s.st_mode)) {
      info.type = FileInfo::Type::kFile;
      info.total_size = s.st_size;
    } else {
      errno = 0;
      continue;
    }
    info.name = xe::to_wstring(name);
    info.create_timestamp = 0;
    info.access_timestamp = s.st_atime;
    info.write_timestamp = s.st_mtime;
    result.push_back(info);
    errno = 0;
  }

  if (errno)
    result.clear();
  return result;
}

}  // namespace filesystem
}  // namespace xe
