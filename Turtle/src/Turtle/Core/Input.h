#pragma once

#include "glm/glm.hpp"

namespace Turtle {
class Input {
public:
  static bool IsKeyPressed(int keycode);

  static bool IsMouseButtonPressed(int button);
  static float GetMouseX();
  static float GetMouseY();
  static glm::vec2 GetMousePos();
};
} // namespace Turtle
