#pragma once

#include "Turtle/Core/Application.h"

extern Turtle::Application *Turtle::CreateApplication();

int main(int argc, char **argv) {
  Turtle::Log::Init();

  TURT_PROFILE_BEGIN_SESSION("Startup", "TurtProfile-Startup.json");
  auto app = Turtle::CreateApplication();
  TURT_PROFILE_END_SESSION();

  TURT_PROFILE_BEGIN_SESSION("Runtime", "TurtProfile-Runtime.json");
  app->Run();
  TURT_PROFILE_END_SESSION();

  TURT_PROFILE_BEGIN_SESSION("Shutdown", "TurtProfile-Shutdown.json");
  delete app;
  TURT_PROFILE_END_SESSION();
}
