#pragma once

#include "Turtle/Core/Log.h"
#include "Turtle/Core/Core.h"
#include "Turtle/Scene/Scene.h"
#include "Turtle/Scene/Entity.h"

#include "FileSelector.h"
#include "ComponentSelectPanel.h"
#include "TilePallettePanel.h"


namespace Turtle {

	class SceneHeirarchyPanel
	{
	public:
		SceneHeirarchyPanel() = default;
		SceneHeirarchyPanel(const Ref<Scene>& scene);

		void SetContext(const Ref<Scene>& scene);

		void OnImGuiRender();
	private:
		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);

		Ref<Scene> m_Context;
		Entity m_SelectionContext;

		bool m_AddingComponent = false;
		ComponentSelectPanel m_ComponentDialogue;
		FileSelector m_FileSelector; 
		TilePallettePanel m_TilePallette;
	};

}