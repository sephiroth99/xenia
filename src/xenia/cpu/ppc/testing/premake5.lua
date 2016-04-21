project_root = "../../../../.."
include(project_root.."/tools/build")

group("tests")
project("xenia-cpu-ppc-tests")
  uuid("2a57d5ac-4024-4c49-9cd3-aa3a603c2ef8")
  kind("ConsoleApp")
  language("C++")
  links({
    "capstone",
    "gflags",
    "xenia-base",
    "xenia-core",
    "xenia-cpu",
    "xenia-cpu-backend-x64",

    -- TODO(benvanik): remove these dependencies.
    "xenia-kernel",
    "xenia-vfs",
  })
  files({
    "ppc_testing_main.cc",
    "../../../base/main_"..platform_suffix..".cc",
  })
  files({
    "*.s",
  })
  includedirs({
    project_root.."/third_party/gflags/src",
  })
  filter("files:*.s")
    flags({"ExcludeFromBuild"})
  filter("platforms:Windows")
    debugdir(project_root)
    debugargs({
      "--flagfile=scratch/flags.txt",
      "2>&1",
      "1>scratch/stdout-testing.txt",
    })
