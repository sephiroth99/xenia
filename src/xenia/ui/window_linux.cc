/**
 ******************************************************************************
 * Xenia : Xbox 360 Emulator Research Project                                 *
 ******************************************************************************
 * Copyright 2014 Ben Vanik. All rights reserved.                             *
 * Released under the BSD license - see LICENSE in the root for more details. *
 ******************************************************************************
 */

#include <string>

#include "xenia/base/assert.h"
#include "xenia/base/logging.h"
#include "xenia/ui/window_linux.h"

#include "third_party/GL/glxew.h"

namespace xe {
namespace ui {

std::unique_ptr<Window> Window::Create(Loop* loop, const std::wstring& title) {
  return std::make_unique<LinuxWindow>(loop, title);
}

LinuxWindow::LinuxWindow(Loop* loop, const std::wstring& title)
    : Window(loop, title) {}

LinuxWindow::~LinuxWindow() {
  OnDestroy();
  /*if (hwnd_) {
    SetWindowLongPtr(hwnd_, GWLP_USERDATA, 0);
    CloseWindow(hwnd_);
    hwnd_ = nullptr;
  }
  if (icon_) {
    DestroyIcon(icon_);
    icon_ = nullptr;
  }*/
}

NativePlatformHandle LinuxWindow::native_platform_handle() const {
  // TODO(sephiroth99): Implement.
  return nullptr;
}

bool LinuxWindow::Initialize() { return OnCreate(); }

bool LinuxWindow::OnCreate() {
  display_ = XOpenDisplay(NULL);
  if (!display_) {
    XELOGE("XOpenDisplay failed");
    return false;
  }

  XWindow root = DefaultRootWindow(display_);

  GLint att[] = {GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None};
  XVisualInfo* vi = glXChooseVisual(display_, 0, att);
  if (!vi) {
    XELOGE("glXChooseVisual failed");
    return false;
  }

  Colormap colormap = XCreateColormap(display_, root, vi->visual, AllocNone);
  XSetWindowAttributes swa;
  swa.colormap = colormap;
  swa.event_mask = ExposureMask | KeyPressMask;

  window_ = XCreateWindow(display_, root, 0, 0, width_, height_, 0, vi->depth, InputOutput, vi->visual, CWColormap | CWEventMask, &swa);
  if (!window_) {
    XELOGE("XCreateWindow failed");
    return false;
  }

  XMapWindow(display_, window_);
  XStoreName(display_, window_, reinterpret_cast<const char*>(title_.c_str()));

  // Initial state.
  if (!is_cursor_visible_) {
    //ShowCursor(FALSE);
  }
  if (has_focus_) {
    //SetFocus(hwnd_);
  }

  return super::OnCreate();
}

void LinuxWindow::OnDestroy() { super::OnDestroy(); }

void LinuxWindow::OnClose() {
  if (!closing_ && window_) {
    closing_ = true;
    XDestroyWindow(display_, window_);
  }
  super::OnClose();
}

bool LinuxWindow::set_title(const std::wstring& title) {
  if (!super::set_title(title)) {
    return false;
  }
  XStoreName(display_, window_, reinterpret_cast<const char*>(title_.c_str()));
  return true;
}

bool LinuxWindow::SetIcon(const void* buffer, size_t size) {
  // TODO(sephiroth99): Implement
  return false;
}

bool LinuxWindow::is_fullscreen() const { return fullscreen_; }

void LinuxWindow::ToggleFullscreen(bool fullscreen) {
  if (fullscreen == is_fullscreen()) {
    return;
  }

  fullscreen_ = fullscreen;
#if 0
  DWORD style = GetWindowLong(hwnd_, GWL_STYLE);
  if (fullscreen) {
    // Kill our borders and resize to take up entire primary monitor.
    // http://blogs.msdn.com/b/oldnewthing/archive/2010/04/12/9994016.aspx
    MONITORINFO mi = {sizeof(mi)};
    if (GetWindowPlacement(hwnd_, &windowed_pos_) &&
        GetMonitorInfo(MonitorFromWindow(hwnd_, MONITOR_DEFAULTTOPRIMARY),
                       &mi)) {
      SetWindowLong(hwnd_, GWL_STYLE, style & ~WS_OVERLAPPEDWINDOW);
      ::SetMenu(hwnd_, NULL);

      // Call into parent class to get around menu resizing code.
      Resize(mi.rcMonitor.left, mi.rcMonitor.top, mi.rcMonitor.right,
             mi.rcMonitor.bottom);
    }
  } else {
    // Reinstate borders, resize to 1280x720
    SetWindowLong(hwnd_, GWL_STYLE, style | WS_OVERLAPPEDWINDOW);
    SetWindowPlacement(hwnd_, &windowed_pos_);

    auto main_menu = reinterpret_cast<Win32MenuItem*>(main_menu_.get());
    if (main_menu) {
      ::SetMenu(hwnd_, main_menu->handle());
    }
  }
#endif
}

bool LinuxWindow::is_bordered() const {
#if 0
  DWORD style = GetWindowLong(hwnd_, GWL_STYLE);
  return (style & WS_OVERLAPPEDWINDOW) == WS_OVERLAPPEDWINDOW;
#endif
  return false;
}

void LinuxWindow::set_bordered(bool enabled) {
  if (is_fullscreen()) {
    // Don't screw with the borders if we're fullscreen.
    return;
  }
#if 0
  DWORD style = GetWindowLong(hwnd_, GWL_STYLE);
  if (enabled) {
    SetWindowLong(hwnd_, GWL_STYLE, style | WS_OVERLAPPEDWINDOW);
  } else {
    SetWindowLong(hwnd_, GWL_STYLE, style & ~WS_OVERLAPPEDWINDOW);
  }
#endif
}

void LinuxWindow::set_cursor_visible(bool value) {
  if (is_cursor_visible_ == value) {
    return;
  }
#if 0
  if (value) {
    ShowCursor(TRUE);
    SetCursor(nullptr);
  } else {
    ShowCursor(FALSE);
  }
#endif
}

void LinuxWindow::set_focus(bool value) {
  if (has_focus_ == value) {
    return;
  }
#if 0
  if (hwnd_) {
    if (value) {
      SetFocus(hwnd_);
    } else {
      SetFocus(nullptr);
    }
  } else {
    has_focus_ = value;
  }
#endif
}

void LinuxWindow::Resize(int32_t width, int32_t height) {
#if 0
  RECT rc = {0, 0, width, height};
  bool has_menu = !is_fullscreen() && (main_menu_ ? true : false);
  AdjustWindowRect(&rc, GetWindowLong(hwnd_, GWL_STYLE), has_menu);
  if (true) {
    rc.right += 100 - rc.left;
    rc.left = 100;
    rc.bottom += 100 - rc.top;
    rc.top = 100;
  }
  MoveWindow(hwnd_, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top,
             TRUE);
#endif
}

void LinuxWindow::Resize(int32_t left, int32_t top, int32_t right,
                         int32_t bottom) {
#if 0
  RECT rc = {left, top, right, bottom};
  bool has_menu = !is_fullscreen() && (main_menu_ ? true : false);
  AdjustWindowRect(&rc, GetWindowLong(hwnd_, GWL_STYLE), has_menu);
  MoveWindow(hwnd_, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top,
             TRUE);
#endif
}

void LinuxWindow::OnResize(UIEvent* e) {
#if 0
  RECT client_rect;
  GetClientRect(hwnd_, &client_rect);
  int32_t width = client_rect.right - client_rect.left;
  int32_t height = client_rect.bottom - client_rect.top;
  if (width != width_ || height != height_) {
    width_ = width;
    height_ = height;
    Layout();
  }
#endif
  super::OnResize(e);
}

void LinuxWindow::Invalidate() {
  super::Invalidate();
#if 0
  InvalidateRect(hwnd_, nullptr, FALSE);
#endif
}

void LinuxWindow::Close() {
  if (closing_) {
    return;
  }
  closing_ = true;
  Close();
  OnClose();
  //DestroyWindow(hwnd_);
}

void LinuxWindow::OnMainMenuChange() {
  auto main_menu = reinterpret_cast<LinuxMenuItem*>(main_menu_.get());
  // Don't actually set the menu if we're fullscreen. We'll do that later.
  if (main_menu && !is_fullscreen()) {
    //::SetMenu(hwnd_, main_menu->handle());
  }
}
#if 0
LRESULT CALLBACK Win32Window::WndProcThunk(HWND hWnd, UINT message,
                                           WPARAM wParam, LPARAM lParam) {
  Win32Window* window = nullptr;
  if (message == WM_NCCREATE) {
    auto create_struct = reinterpret_cast<LPCREATESTRUCT>(lParam);
    window = reinterpret_cast<Win32Window*>(create_struct->lpCreateParams);
    SetWindowLongPtr(hWnd, GWLP_USERDATA, (__int3264)(LONG_PTR)window);
  } else {
    window =
        reinterpret_cast<Win32Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
  }
  if (window) {
    return window->WndProc(hWnd, message, wParam, lParam);
  } else {
    return DefWindowProc(hWnd, message, wParam, lParam);
  }
}

LRESULT Win32Window::WndProc(HWND hWnd, UINT message, WPARAM wParam,
                             LPARAM lParam) {
  if (hWnd != hwnd_) {
    return DefWindowProc(hWnd, message, wParam, lParam);
  }

  if (message >= WM_MOUSEFIRST && message <= WM_MOUSELAST) {
    if (HandleMouse(message, wParam, lParam)) {
      return 0;
    } else {
      return DefWindowProc(hWnd, message, wParam, lParam);
    }
  } else if (message >= WM_KEYFIRST && message <= WM_KEYLAST) {
    if (HandleKeyboard(message, wParam, lParam)) {
      return 0;
    } else {
      return DefWindowProc(hWnd, message, wParam, lParam);
    }
  }

  switch (message) {
    case WM_NCCREATE:
      break;
    case WM_CREATE:
      break;
    case WM_DESTROY:
      OnDestroy();
      break;
    case WM_CLOSE:
      closing_ = true;
      Close();
      OnClose();
      break;

    case WM_MOVING:
      break;
    case WM_MOVE:
      break;
    case WM_SIZING:
      break;
    case WM_SIZE: {
      auto e = UIEvent(this);
      OnResize(&e);
      break;
    }

    case WM_PAINT: {
      ValidateRect(hwnd_, nullptr);
      static bool in_paint = false;
      if (!in_paint) {
        in_paint = true;
        auto e = UIEvent(this);
        OnPaint(&e);
        in_paint = false;
      }
      return 0;  // Ignored because of custom paint.
    }
    case WM_ERASEBKGND:
      return 0;  // Ignored because of custom paint.
    case WM_DISPLAYCHANGE:
      break;

    case WM_ACTIVATEAPP:
    case WM_SHOWWINDOW: {
      if (wParam == TRUE) {
        auto e = UIEvent(this);
        OnVisible(&e);
      } else {
        auto e = UIEvent(this);
        OnHidden(&e);
      }
      break;
    }

    case WM_KILLFOCUS: {
      has_focus_ = false;
      auto e = UIEvent(this);
      OnLostFocus(&e);
      break;
    }
    case WM_SETFOCUS: {
      has_focus_ = true;
      auto e = UIEvent(this);
      OnGotFocus(&e);
      break;
    }

    case WM_TABLET_QUERYSYSTEMGESTURESTATUS:
      return
          // disables press and hold (right-click) gesture
          TABLET_DISABLE_PRESSANDHOLD |
          // disables UI feedback on pen up (waves)
          TABLET_DISABLE_PENTAPFEEDBACK |
          // disables UI feedback on pen button down (circle)
          TABLET_DISABLE_PENBARRELFEEDBACK |
          // disables pen flicks (back, forward, drag down, drag up)
          TABLET_DISABLE_FLICKS | TABLET_DISABLE_TOUCHSWITCH |
          TABLET_DISABLE_SMOOTHSCROLLING | TABLET_DISABLE_TOUCHUIFORCEON |
          TABLET_ENABLE_MULTITOUCHDATA;

    case WM_MENUCOMMAND: {
      // TODO(benvanik): Redirect this to MenuItem's on_selected delegate.
      MENUINFO menu_info = {0};
      menu_info.cbSize = sizeof(menu_info);
      menu_info.fMask = MIM_MENUDATA;
      GetMenuInfo(HMENU(lParam), &menu_info);
      auto parent_item = reinterpret_cast<Win32MenuItem*>(menu_info.dwMenuData);
      auto child_item =
          reinterpret_cast<Win32MenuItem*>(parent_item->child(wParam));
      assert_not_null(child_item);
      UIEvent e(this);
      child_item->OnSelected(&e);
    } break;
  }

  return DefWindowProc(hWnd, message, wParam, lParam);
}

bool Win32Window::HandleMouse(UINT message, WPARAM wParam, LPARAM lParam) {
  int32_t x = GET_X_LPARAM(lParam);
  int32_t y = GET_Y_LPARAM(lParam);
  if (message == WM_MOUSEWHEEL) {
    POINT pt = {x, y};
    ScreenToClient(hwnd_, &pt);
    x = pt.x;
    y = pt.y;
  }

  MouseEvent::Button button = MouseEvent::Button::kNone;
  int32_t dx = 0;
  int32_t dy = 0;
  switch (message) {
    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
      button = MouseEvent::Button::kLeft;
      break;
    case WM_RBUTTONDOWN:
    case WM_RBUTTONUP:
      button = MouseEvent::Button::kRight;
      break;
    case WM_MBUTTONDOWN:
    case WM_MBUTTONUP:
      button = MouseEvent::Button::kMiddle;
      break;
    case WM_XBUTTONDOWN:
    case WM_XBUTTONUP:
      switch (GET_XBUTTON_WPARAM(wParam)) {
        case XBUTTON1:
          button = MouseEvent::Button::kX1;
          break;
        case XBUTTON2:
          button = MouseEvent::Button::kX2;
          break;
        default:
          return false;
      }
      break;
    case WM_MOUSEMOVE:
      button = MouseEvent::Button::kNone;
      break;
    case WM_MOUSEWHEEL:
      button = MouseEvent::Button::kNone;
      dx = 0;  // ?
      dy = GET_WHEEL_DELTA_WPARAM(wParam);
      break;
    default:
      // Double click/etc?
      return true;
  }

  auto e = MouseEvent(this, button, x, y, dx, dy);
  switch (message) {
    case WM_LBUTTONDOWN:
    case WM_RBUTTONDOWN:
    case WM_MBUTTONDOWN:
    case WM_XBUTTONDOWN:
      OnMouseDown(&e);
      break;
    case WM_LBUTTONUP:
    case WM_RBUTTONUP:
    case WM_MBUTTONUP:
    case WM_XBUTTONUP:
      OnMouseUp(&e);
      break;
    case WM_MOUSEMOVE:
      OnMouseMove(&e);
      break;
    case WM_MOUSEWHEEL:
      OnMouseWheel(&e);
      break;
  }
  return e.is_handled();
}

bool Win32Window::HandleKeyboard(UINT message, WPARAM wParam, LPARAM lParam) {
  auto e = KeyEvent(this, static_cast<int>(wParam), lParam & 0xFFFF0000,
                    !!(lParam & 0x2));
  switch (message) {
    case WM_KEYDOWN:
      OnKeyDown(&e);
      break;
    case WM_KEYUP:
      OnKeyUp(&e);
      break;
    case WM_CHAR:
      OnKeyChar(&e);
      break;
  }
  return e.is_handled();
}
#endif

std::unique_ptr<ui::MenuItem> MenuItem::Create(Type type,
                                               const std::wstring& text,
                                               const std::wstring& hotkey,
                                               std::function<void()> callback) {
  return std::make_unique<LinuxMenuItem>(type, text, hotkey, callback);
}

LinuxMenuItem::LinuxMenuItem(Type type, const std::wstring& text,
                             const std::wstring& hotkey,
                             std::function<void()> callback)
    : MenuItem(type, text, hotkey, std::move(callback)) {
#if 0
  switch (type) {
    case MenuItem::Type::kNormal:
      handle_ = CreateMenu();
      break;
    case MenuItem::Type::kPopup:
      handle_ = CreatePopupMenu();
      break;
    default:
      // May just be a placeholder.
      break;
  }
  if (handle_) {
    MENUINFO menu_info = {0};
    menu_info.cbSize = sizeof(menu_info);
    menu_info.fMask = MIM_MENUDATA | MIM_STYLE;
    menu_info.dwMenuData = ULONG_PTR(this);
    menu_info.dwStyle = MNS_NOTIFYBYPOS;
    SetMenuInfo(handle_, &menu_info);
  }
#endif
}

LinuxMenuItem::~LinuxMenuItem() {
  if (handle_) {
    //DestroyMenu(handle_);
  }
}

void LinuxMenuItem::OnChildAdded(MenuItem* generic_child_item) {
  auto child_item = static_cast<LinuxMenuItem*>(generic_child_item);
#if 0
  switch (child_item->type()) {
    case MenuItem::Type::kNormal:
      // Nothing special.
      break;
    case MenuItem::Type::kPopup:
      AppendMenuW(handle_, MF_POPUP,
                  reinterpret_cast<UINT_PTR>(child_item->handle()),
                  child_item->text().c_str());
      break;
    case MenuItem::Type::kSeparator:
      AppendMenuW(handle_, MF_SEPARATOR, UINT_PTR(child_item->handle_), 0);
      break;
    case MenuItem::Type::kString:
      auto full_name = child_item->text();
      if (!child_item->hotkey().empty()) {
        full_name += L"\t" + child_item->hotkey();
      }
      AppendMenuW(handle_, MF_STRING, UINT_PTR(child_item->handle_),
                  full_name.c_str());
      break;
  }
#endif
}

void LinuxMenuItem::OnChildRemoved(MenuItem* generic_child_item) {}

}  // namespace ui
}  // namespace xe
