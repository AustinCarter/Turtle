#pragma once

#include "Turtle/Core/Core.h"
#include "Turtle/Core/Log.h"
#include "Turtle/Scene/Entity.h"
#include "Turtle/Scene/Scene.h"

#include "ComponentSelectPanel.h"
#include "TilePallettePanel.h"

namespace Turtle {

class SceneHeirarchyPanel {
public:
  SceneHeirarchyPanel() = default;
  SceneHeirarchyPanel(const Ref<Scene> &scene);

  void SetContext(const Ref<Scene> &scene);

  void OnImGuiRender();

private:
  void DrawEntityNode(Entity entity);
  void DrawComponents(Entity entity);

  Ref<Scene> m_Context;
  Entity m_SelectionContext;

  bool m_AddingComponent = false;
  ComponentSelectPanel m_ComponentDialogue;
  TilePallettePanel m_TilePallette;
};

} // namespace Turtle
