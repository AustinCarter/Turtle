#include "Turtle/Core/Core.h"
#include "Turtle/Utils/PlatformUtils.h"
#include "turtpch.h"

#include <portable-file-dialogs.h>
#include <vector>

namespace Turtle {
std::string FileDialogs::OpenFile(const char *filter) {
  auto file = pfd::open_file("Choose file to read", pfd::path::home(), {filter},
                             pfd::opt::none);
  if (file.result().size() <= 0) {
    return std::string();
  }
  return file.result()[0];
}
} // namespace Turtle
