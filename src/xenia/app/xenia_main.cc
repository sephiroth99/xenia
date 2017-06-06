/**
 ******************************************************************************
 * Xenia : Xbox 360 Emulator Research Project                                 *
 ******************************************************************************
 * Copyright 2013 Ben Vanik. All rights reserved.                             *
 * Released under the BSD license - see LICENSE in the root for more details. *
 ******************************************************************************
 */

#include <gflags/gflags.h>

#include "xenia/app/emulator_window.h"
#include "xenia/base/debugging.h"
#include "xenia/base/logging.h"
#include "xenia/base/main.h"
#include "xenia/base/profiling.h"
#include "xenia/base/threading.h"
#include "xenia/debug/ui/debug_window.h"
#include "xenia/emulator.h"
#include "xenia/ui/file_picker.h"

// Available audio systems:
#include "xenia/apu/nop/nop_audio_system.h"
#if XE_PLATFORM_WIN32
#include "xenia/apu/xaudio2/xaudio2_audio_system.h"
#endif  // XE_PLATFORM_WIN32

// Available graphics systems:
#if XE_PLATFORM_WIN32
#include "xenia/gpu/gl4/gl4_graphics_system.h"
#endif  // XE_PLATFORM_WIN32
#include "xenia/gpu/null/null_graphics_system.h"
#include "xenia/gpu/vulkan/vulkan_graphics_system.h"

// Available input drivers:
#include "xenia/hid/nop/nop_hid.h"
#if XE_PLATFORM_WIN32
#include "xenia/hid/winkey/winkey_hid.h"
#include "xenia/hid/xinput/xinput_hid.h"
#endif  // XE_PLATFORM_WIN32

DEFINE_string(apu, "any", "Audio system. Use: [any, nop, xaudio2]");
DEFINE_string(gpu, "any", "Graphics system. Use: [any, gl4, vulkan, null]");
DEFINE_string(hid, "any", "Input system. Use: [any, nop, winkey, xinput]");

DEFINE_string(target, "", "Specifies the target .xex or .iso to execute.");
DEFINE_bool(fullscreen, false, "Toggles fullscreen");

namespace xe {
namespace app {

std::unique_ptr<apu::AudioSystem> CreateAudioSystem(cpu::Processor* processor) {
  if (FLAGS_apu.compare("nop") == 0) {
    return apu::nop::NopAudioSystem::Create(processor);
#if XE_PLATFORM_WIN32
  } else if (FLAGS_apu.compare("xaudio2") == 0) {
    return apu::xaudio2::XAudio2AudioSystem::Create(processor);
#endif  // XE_PLATFORM_WIN32
  } else {
    // Create best available.
    std::unique_ptr<apu::AudioSystem> best;

#if XE_PLATFORM_WIN32
    best = apu::xaudio2::XAudio2AudioSystem::Create(processor);
    if (best) {
      return best;
    }
#endif  // XE_PLATFORM_WIN32

    // Fallback to nop.
    return apu::nop::NopAudioSystem::Create(processor);
  }
}

std::unique_ptr<gpu::GraphicsSystem> CreateGraphicsSystem() {
  if (FLAGS_gpu.compare("vulkan") == 0) {
    return std::unique_ptr<gpu::GraphicsSystem>(
        new xe::gpu::vulkan::VulkanGraphicsSystem());
#if XE_PLATFORM_WIN32
  } else if (FLAGS_gpu.compare("gl4") == 0) {
    return std::unique_ptr<gpu::GraphicsSystem>(
        new xe::gpu::gl4::GL4GraphicsSystem());
#endif  // XE_PLATFORM_WIN32
  } else if (FLAGS_gpu.compare("null") == 0) {
    return std::unique_ptr<gpu::GraphicsSystem>(
        new xe::gpu::null::NullGraphicsSystem());
  } else {
    // Create best available.
    std::unique_ptr<gpu::GraphicsSystem> best;

    best = std::unique_ptr<gpu::GraphicsSystem>(
        new xe::gpu::vulkan::VulkanGraphicsSystem());
    if (best) {
      return best;
    }

    // Nothing!
    return nullptr;
  }
}

std::vector<std::unique_ptr<hid::InputDriver>> CreateInputDrivers(
    ui::Window* window) {
  std::vector<std::unique_ptr<hid::InputDriver>> drivers;
  if (FLAGS_hid.compare("nop") == 0) {
    drivers.emplace_back(xe::hid::nop::Create(window));
#if XE_PLATFORM_WIN32
  } else if (FLAGS_hid.compare("winkey") == 0) {
    drivers.emplace_back(xe::hid::winkey::Create(window));
  } else if (FLAGS_hid.compare("xinput") == 0) {
    drivers.emplace_back(xe::hid::xinput::Create(window));
#endif  // XE_PLATFORM_WIN32
  } else {
#if XE_PLATFORM_WIN32
    auto xinput_driver = xe::hid::xinput::Create(window);
    if (xinput_driver) {
      drivers.emplace_back(std::move(xinput_driver));
    }
    auto winkey_driver = xe::hid::winkey::Create(window);
    if (winkey_driver) {
      drivers.emplace_back(std::move(winkey_driver));
    }
#endif  // XE_PLATFORM_WIN32
    if (drivers.empty()) {
      // Fallback to nop if none created.
      drivers.emplace_back(xe::hid::nop::Create(window));
    }
  }
  return drivers;
}

int xenia_main(const std::vector<std::wstring>& args) {
  Profiler::Initialize();
  Profiler::ThreadEnter("main");

  // Create the emulator but don't initialize so we can setup the window.
  auto emulator = std::make_unique<Emulator>(L"");

  // Main emulator display window.
  auto emulator_window = EmulatorWindow::Create(emulator.get());

  // Setup and initialize all subsystems. If we can't do something
  // (unsupported system, memory issues, etc) this will fail early.
  X_STATUS result =
      emulator->Setup(emulator_window->window(), CreateAudioSystem,
                      CreateGraphicsSystem, CreateInputDrivers);
  if (XFAILED(result)) {
    XELOGE("Failed to setup emulator: %.8X", result);
    return 1;
  }

  // Set a debug handler.
  // This will respond to debugging requests so we can open the debug UI.
  std::unique_ptr<xe::debug::ui::DebugWindow> debug_window;
  if (FLAGS_debug) {
    emulator->processor()->set_debug_listener_request_handler([&](
        xe::cpu::Processor* processor) {
      if (debug_window) {
        return debug_window.get();
      }
      emulator_window->loop()->PostSynchronous([&]() {
        debug_window = xe::debug::ui::DebugWindow::Create(
            emulator.get(), emulator_window->loop());
        debug_window->window()->on_closed.AddListener([&](xe::ui::UIEvent* e) {
          emulator->processor()->set_debug_listener(nullptr);
          emulator_window->loop()->Post([&]() { debug_window.reset(); });
        });
      });
      return debug_window.get();
    });
  }

  auto evt = xe::threading::Event::CreateAutoResetEvent(false);
  emulator->on_launch.AddListener([&]() {
    emulator_window->UpdateTitle();
    evt->Set();
  });

  bool exiting = false;
  emulator_window->loop()->on_quit.AddListener([&](ui::UIEvent* e) {
    exiting = true;
    evt->Set();
  });

  // Grab path from the flag or unnamed argument.
  std::wstring path;
  if (!FLAGS_target.empty() || args.size() >= 2) {
    if (!FLAGS_target.empty()) {
      // Passed as a named argument.
      // TODO(benvanik): find something better than gflags that supports
      // unicode.
      path = xe::to_wstring(FLAGS_target);
    } else {
      // Passed as an unnamed argument.
      path = args[1];
    }
  }

  // Toggles fullscreen
  if (FLAGS_fullscreen) emulator_window->ToggleFullscreen();

  if (!path.empty()) {
    // Normalize the path and make absolute.
    std::wstring abs_path = xe::to_absolute_path(path);
    result = emulator->LaunchPath(abs_path);
    if (XFAILED(result)) {
      XELOGE("Failed to launch target: %.8X", result);
      emulator.reset();
      emulator_window.reset();
      return 1;
    }
  }

  // Now, we're going to use the main thread to drive events related to
  // emulation.
  while (!exiting) {
    xe::threading::Wait(evt.get(), false);

    while (true) {
      emulator->WaitUntilExit();
      if (emulator->TitleRequested()) {
        emulator->LaunchNextTitle();
      } else {
        break;
      }
    }
  }

  debug_window.reset();
  emulator.reset();
  emulator_window.reset();

  Profiler::Dump();
  Profiler::Shutdown();
  return 0;
}

}  // namespace app
}  // namespace xe

DEFINE_ENTRY_POINT(L"xenia", L"xenia some.xex", xe::app::xenia_main);
