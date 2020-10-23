#include "ComponentSelectPanel.h"

#include <imgui/imgui.h>

#include "Turtle/Scene/Components.h";

namespace Turtle {
	void ComponentSelectPanel::Display()
	{
		ImGui::Begin("Add Component");

		ImGui::Button("Component Type To Add", {ImGui::GetContentRegionAvailWidth(), 0 });

		static int selected = -1;
		//NOTE: At the moment order matters, must match ComponentType order
		const char* componentTypes[] = {"SpriteRendererComponent", "CameraComponent", "NativeScriptComponent", "ParticleSpawnerComponenet", "TileSetComponent"};
		ImGui::BeginChild("Component Select", ImVec2(ImGui::GetWindowContentRegionWidth(), 200), true);
		if(ImGui::Selectable("None", -1==m_ComponentType, ImGuiSelectableFlags_AllowDoubleClick))
		{
			m_ComponentType = -1;
			if(ImGui::IsMouseDoubleClicked(0))
				m_ComponentSelected = true;
		}
        for (int i = 0; i < sizeof(componentTypes) / sizeof(char*); i++)
        {
            if (ImGui::Selectable(componentTypes[i], i==m_ComponentType, ImGuiSelectableFlags_AllowDoubleClick))
            {
                m_ComponentType = i;
                if (ImGui::IsMouseDoubleClicked(0))
                {
                	m_ComponentSelected = true;
                }
            }
        }
        ImGui::EndChild();
		if(ImGui::Button("Select"))
		{
			m_ComponentSelected = true;
		}
		ImGui::End();
	}

	void ComponentSelectPanel::Close()
	{
		m_Active = false;
	}
}