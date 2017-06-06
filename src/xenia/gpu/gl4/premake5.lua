project_root = "../../../.."
include(project_root.."/tools/build")

group("src")
project("xenia-gpu-gl4")
  uuid("da10149d-efb0-44aa-924c-a76a46e1f04d")
  kind("StaticLib")
  language("C++")
  links({
    "glew",
    "xenia-base",
    "xenia-gpu",
    "xenia-ui",
    "xenia-ui-gl",
    "xxhash",
  })
  defines({
    "GLEW_STATIC=1",
    "GLEW_MX=1",
  })
  includedirs({
    project_root.."/third_party/gflags/src",
  })
  local_platform_files()

-- TODO(benvanik): kill this and move to the debugger UI.
group("src")
project("xenia-gpu-gl4-trace-viewer")
  uuid("450f965b-a019-4ba5-bc6f-99901e5a4c8d")
  kind("WindowedApp")
  language("C++")
  links({
    "gflags",
    "glew",
    "imgui",
    "xenia-apu",
    "xenia-apu-nop",
    "xenia-apu-xaudio2",
    "xenia-base",
    "xenia-core",
    "xenia-cpu",
    "xenia-cpu-backend-x64",
    "xenia-gpu",
    "xenia-gpu-gl4",
    "xenia-hid-nop",
    "xenia-hid-winkey",
    "xenia-hid-xinput",
    "xenia-kernel",
    "xenia-ui",
    "xenia-ui-gl",
    "xenia-vfs",
  })
  flags({
    "WinMain",  -- Use WinMain instead of main.
  })
  defines({
    "GLEW_STATIC=1",
    "GLEW_MX=1",
  })
  includedirs({
    project_root.."/third_party/gflags/src",
  })
  files({
    "gl4_trace_viewer_main.cc",
    "../../base/main_"..platform_suffix..".cc",
  })

  filter("platforms:Windows")
    -- Only create the .user file if it doesn't already exist.
    local user_file = project_root.."/build/xenia-gpu-gl4-trace-viewer.vcxproj.user"
    if not os.isfile(user_file) then
      debugdir(project_root)
      debugargs({
        "--flagfile=scratch/flags.txt",
        "2>&1",
        "1>scratch/stdout-trace-viewer.txt",
      })
    end

group("src")
project("xenia-gpu-gl4-trace-dump")
  uuid("5d47299d-f37d-46b4-af48-f4e54b9e5662")
  kind("ConsoleApp")
  language("C++")
  links({
    "gflags",
    "glew",
    "imgui",
    "xenia-apu",
    "xenia-apu-nop",
    "xenia-apu-xaudio2",
    "xenia-base",
    "xenia-core",
    "xenia-cpu",
    "xenia-cpu-backend-x64",
    "xenia-gpu",
    "xenia-gpu-gl4",
    "xenia-hid-nop",
    "xenia-hid-winkey",
    "xenia-hid-xinput",
    "xenia-kernel",
    "xenia-ui",
    "xenia-ui-gl",
    "xenia-vfs",
  })
  defines({
    "GLEW_STATIC=1",
    "GLEW_MX=1",
  })
  includedirs({
    project_root.."/third_party/gflags/src",
  })
  files({
    "gl4_trace_dump_main.cc",
    "../../base/main_"..platform_suffix..".cc",
  })

  filter("platforms:Windows")
    -- Only create the .user file if it doesn't already exist.
    local user_file = project_root.."/build/xenia-gpu-gl4-trace-dump.vcxproj.user"
    if not os.isfile(user_file) then
      debugdir(project_root)
      debugargs({
        "--flagfile=scratch/flags.txt",
        "2>&1",
        "1>scratch/stdout-trace-dump.txt",
      })
    end
