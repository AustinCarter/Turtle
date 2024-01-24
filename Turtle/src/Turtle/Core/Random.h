#pragma once

#include "Turtle/Core/Log.h"
#include <cstdint>
#include <limits>
#include <random>

namespace Turtle {

class Random {
public:
  static void Init() {
    TURT_CORE_INFO("seeding radom");
    s_RandomEngine.seed(std::random_device()());
  }

  static float Float() {
    uint32_t dist = s_Distribution(s_RandomEngine);
    uint32_t max = std::numeric_limits<uint32_t>::max();
    return (float)dist / max;
  }

private:
  static std::mt19937 s_RandomEngine;
  static std::uniform_int_distribution<std::mt19937::result_type>
      s_Distribution;
};

} // namespace Turtle
