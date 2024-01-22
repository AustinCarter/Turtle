#ifdef TURT_PLATFORM_WINDOWS
#include "Turtle/Utils/PlatformUtils.h"
#include "turtpch.h"

#include <GLFW/glfw3.h>
#include <commdlg.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include "Turtle/Core/Application.h"

namespace Turtle {

std::string FileDialogs::OpenFile(const char *filter) {
  OPENFILENAMEA ofn;
  CHAR szFile[260] = {0};
  ZeroMemory(&ofn, sizeof(OPENFILENAME));
  ofn.lStructSize = sizeof(OPENFILENAME);
  ofn.hwndOwner = glfwGetWin32Window(
      (GLFWwindow *)Application::Get().GetWindow().GetNativeWindow());
  ofn.lpstrFile = szFile;
  ofn.nMaxFile = sizeof(szFile);
  ofn.lpstrFilter = filter;
  ofn.nFilterIndex = 1;
  ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
  if (GetOpenFileNameA(&ofn) == TRUE) {
    return ofn.lpstrFile;
  }
  return std::string();
}

std::string FileDialogs::SaveFile(const char *filter) {
  OPENFILENAMEA ofn;
  CHAR szFile[260] = {0};
  ZeroMemory(&ofn, sizeof(OPENFILENAME));
  ofn.lStructSize = sizeof(OPENFILENAME);
  ofn.hwndOwner = glfwGetWin32Window(
      (GLFWwindow *)Application::Get().GetWindow().GetNativeWindow());
  ofn.lpstrFile = szFile;
  ofn.nMaxFile = sizeof(szFile);
  ofn.lpstrFilter = filter;
  ofn.nFilterIndex = 1;
  ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
  if (GetSaveFileNameA(&ofn) == TRUE) {
    return ofn.lpstrFile;
  }
  return std::string();
}

} // namespace Turtle
#endif
