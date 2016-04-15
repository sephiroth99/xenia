/**
 ******************************************************************************
 * Xenia : Xbox 360 Emulator Research Project                                 *
 ******************************************************************************
 * Copyright 2014 Ben Vanik. All rights reserved.                             *
 * Released under the BSD license - see LICENSE in the root for more details. *
 ******************************************************************************
 */

#ifndef XENIA_UI_GL_GL_CONTEXT_H_
#define XENIA_UI_GL_GL_CONTEXT_H_

#include <gflags/gflags.h>

#include <memory>

#include "xenia/ui/gl/blitter.h"
#include "xenia/ui/gl/gl.h"
#include "xenia/ui/graphics_context.h"

DECLARE_bool(thread_safe_gl);

// TODO(benvanik): hide Win32 stuff.
#if XE_PLATFORM_WIN32
typedef struct HDC__* HDC;
typedef struct HGLRC__* HGLRC;
#endif  // XE_PLATFORM_WIN32

#if XE_PLATFORM_LINUX
#include "third_party/GL/glxew.h"

// X11 has a lot of definitions with very generic names...
// Redefine some of them here with more unique names.
#undef Status
#define XStatus int
typedef XID XWindow;
#endif  // XE_PLATFORM_LINUX

namespace xe {
namespace ui {
namespace gl {

class GLImmediateDrawer;
class GLProvider;

class GLContext : public GraphicsContext {
 public:
  ~GLContext() override;

  ImmediateDrawer* immediate_drawer() override;

  bool is_current() override;
  bool MakeCurrent() override;
  void ClearCurrent() override;
  bool WasLost() override;

  void BeginSwap() override;
  void EndSwap() override;

  std::unique_ptr<RawImage> Capture() override;

  Blitter* blitter() { return &blitter_; }

 private:
  friend class GLProvider;

  static std::unique_ptr<GLContext> Create(GraphicsProvider* provider,
                                           Window* target_window,
                                           GLContext* share_context = nullptr);
  static std::unique_ptr<GLContext> CreateOffscreen(GraphicsProvider* provider,
                                                    GLContext* parent_context);

 private:
  GLContext(GraphicsProvider* provider, Window* target_window);

  bool Initialize(GLContext* share_context);
  void AssertExtensionsPresent();

  void SetupDebugging();
  void DebugMessage(GLenum source, GLenum type, GLuint id, GLenum severity,
                    GLsizei length, const GLchar* message);
  static void GLAPIENTRY DebugMessageThunk(GLenum source, GLenum type,
                                           GLuint id, GLenum severity,
                                           GLsizei length,
                                           const GLchar* message,
                                           GLvoid* user_param);

  std::unique_ptr<GLEWContext> glew_context_;

#if XE_PLATFORM_WIN32
  HDC dc_ = nullptr;
  HGLRC glrc_ = nullptr;
  std::unique_ptr<WGLEWContext> wglew_context_;
#endif  // XE_PLATFORM_WIN32

#if XE_PLATFORM_LINUX
  Display* dpy_ = nullptr;
  XWindow win_;
  GLXFBConfig fbc_;
  GLXContext glrc_;
  std::unique_ptr<GLXEWContext> glxew_context_;
#endif  // XE_PLATFORM_LINUX

  Blitter blitter_;
  std::unique_ptr<GLImmediateDrawer> immediate_drawer_;

  bool context_lost_ = false;
  bool robust_access_supported_ = false;
};

}  // namespace gl
}  // namespace ui
}  // namespace xe

#endif  // XENIA_UI_GL_GL_CONTEXT_H_
