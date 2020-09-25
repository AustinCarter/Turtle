#include "ComponentAddPanel.h"

#include <imgui/imgui.h>

#include "Turtle/Scene/Components.h";

namespace Turtle {
	bool ComponentAddPanel::OnImGuiRender()
	{
		ImGui::Begin("Add Component");

		ImGui::Button("Component Type To Add", {ImGui::GetContentRegionAvailWidth(), 0 });

		static int selected = -1;
		//NOTE: Order matters, must match ComponentType order
		const char* componentTypes[] = {"SpriteRendererComponent", "CameraComponent", "NativeScriptComponent"};
		if(ImGui::Selectable("None", -1==m_ComponentType))
			m_ComponentType = -1;
        for (int i = 0; i < 3; i++)
        {
        	//TODO: PROBABLY DON'T WANT TO HARD CODE COMPONENT TYPES
        	//TODO: SHOULDN'T BE ABLE TO ADD INVALID COMPONENT TYPE 
            if (ImGui::Selectable(componentTypes[i], i==m_ComponentType))
            {
                m_ComponentType = i;
            }
        }
        ImGui::Separator();
		if(ImGui::Button("Select"))
		{
			m_ComponentCreated = true;

			if(m_ComponentType == (int)ComponentType::SpriteRendererComponent)
			{
				m_Entity.AddComponenet<SpriteRendererComponent>();
			}
			else if(m_ComponentType == (int)ComponentType::CameraComponent)
			{
				m_Entity.AddComponenet<CameraComponent>();
				m_Context->OnCameraAdd(m_Entity.GetComponent<CameraComponent>());
			}
			else if(m_ComponentType == (int)ComponentType::NativeScriptComponent)
			{
				//NOTE: won't work until script instantiating is moved to a scene on play method
				//tries to create an instance without a script chosen (could also set it to some default script)
				m_Entity.AddComponenet<NativeScriptComponent>();
			}

		}
		ImGui::End();

		if(m_ComponentCreated)
		{
			ResetState();
			return false;
		}

		return true;
	}

	void ComponentAddPanel::ResetState()
	{
		m_ComponentCreated = false;
		m_ComponentType = -1;
	}
}