#include "Window.h"
#include "Platform/Windows/WindowsWindow.h"

namespace Turtle {

Scope<Window> Window::Create(const WindowProps &props) {
  return CreateScope<WindowsWindow>(props);
}
} // namespace Turtle
