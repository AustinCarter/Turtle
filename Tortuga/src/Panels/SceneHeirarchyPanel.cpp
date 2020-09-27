#include "SceneHeirarchyPanel.h"

#include <imgui/imgui.h>

#include <glm/gtc/type_ptr.hpp>

#include "Turtle/Scene/Components.h"

namespace Turtle {
	SceneHeirarchyPanel::SceneHeirarchyPanel(const Ref<Scene>& scene)
	{
		SetContext(scene);
	}

	void SceneHeirarchyPanel::SetContext(const Ref<Scene>& scene)
	{
		m_Context = scene;
	}

	void SceneHeirarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Heirarchy");

		m_Context->m_Registry.each([&](auto entityID)
		{
			Entity entity = { entityID, m_Context.get() };
			DrawEntityNode(entity);		
		});

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			m_SelectionContext = {};

		if (ImGui::Button("Add Entity"))
		{
			m_Context->CreateEntity();
		}

		ImGui::End();

		ImGui::Begin("Properties");
		if( m_SelectionContext )
		{
			DrawComponents(m_SelectionContext);
		}

		if( m_AddingComponent )
		{
			m_AddingComponent = m_ComponentDialogue.OnImGuiRender(); 
		}

		ImGui::End();
	}

	void SceneHeirarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().Tag;

		ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0)| ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
		if (ImGui::IsItemClicked())
		{
			m_SelectionContext = entity;
		}

		if (opened)
		{
			ImGui::TreePop();
		}
		
	}

	void SceneHeirarchyPanel::DrawComponents(Entity entity)
	{
		if(entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>().Tag;

			char buffer[128];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());
			if (ImGui::InputText("Tag", buffer, sizeof(buffer))) 
			{
				tag = std::string(buffer);
			}
		}

		if(entity.HasComponent<TransformComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
			{
				auto& transform = entity.GetComponent<TransformComponent>().Transform;
				ImGui::DragFloat3("Position", glm::value_ptr(transform[3]), 0.1f);

				ImGui::TreePop();
			}
		}

		if(entity.HasComponent<CameraComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera"))
			{
				auto& cameraComponent = entity.GetComponent<CameraComponent>();
				auto& camera = cameraComponent.Camera;

				ImGui::Checkbox("Primary", &cameraComponent.Primary);

				const char* projectionTypeStrings[] = {"Perspective", "Orthographic"};

				const char* currentProjectionTypeString = projectionTypeStrings[(int)(camera.GetProjectionType())];
				if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
				{
					for (int i = 0; i < 2; i++)
					{
						bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
						if(ImGui::Selectable(projectionTypeStrings[i], isSelected))
						{
							currentProjectionTypeString = projectionTypeStrings[i];
							camera.SetProjectionType((SceneCamera::ProjectionType)i);
						}

						if(isSelected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
				{

					float perspectiveFOV = glm::degrees(camera.GetPerspectiveVerticalFOV());
					if(ImGui::DragFloat("FOV", &perspectiveFOV, 0.1f))
						camera.SetPerspectiveVerticalFOV(glm::radians(perspectiveFOV));

					float perspectiveNear = camera.GetPerspectiveNearClip();
					if(ImGui::DragFloat("Near Clip", &perspectiveNear, 0.1f))
						camera.SetPerspectiveNearClip(perspectiveNear);

					float perspectiveFar = camera.GetPerspectiveFarClip();
					if(ImGui::DragFloat("Far Clip", &perspectiveFar, 0.1f))
						camera.SetPerspectiveFarClip(perspectiveFar);	
				}

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
				{
					float orthoSize = camera.GetOrthographicSize();
					if(ImGui::DragFloat("Size", &orthoSize, 0.1f))
						camera.SetOrthographicSize(orthoSize);

					float orthoNear = camera.GetOrthographicNearClip();
					if(ImGui::DragFloat("Near Clip", &orthoNear, 0.1f))
						camera.SetOrthographicNearClip(orthoNear);

					float orthoFar = camera.GetOrthographicFarClip();
					if(ImGui::DragFloat("Far Clip", &orthoFar, 0.1f))
						camera.SetOrthographicFarClip(orthoFar);		


					ImGui::Checkbox("Fixed Aspect Ratio", &cameraComponent.FixedAspectRatio);	
				}


				ImGui::TreePop();
			}
		}


		if(entity.HasComponent<SpriteRendererComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(SpriteRendererComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Sprite"))
			{
				auto& spriteComponent = entity.GetComponent<SpriteRendererComponent>();
				auto& color = spriteComponent.Color; 
				ImGui::ColorEdit4("Color", glm::value_ptr(color));

				// MIGHT BE TEMP
				if (spriteComponent.Textured)
					ImGui::Image((void*)spriteComponent.TextureID, ImVec2{ 128, 128 }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

				ImGui::TreePop();
			}
		}

		if(entity.HasComponent<NativeScriptComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(NativeScriptComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Native Script"))
			{
				auto& nativeScriptComponent = entity.GetComponent<NativeScriptComponent>();
				bool bound = nativeScriptComponent.Bound;
				const char* BoundString = bound ? "" : "No script bound";
				ImGui::Button(BoundString);
				ImGui::TreePop();
			} 
		}

		if (ImGui::Button("+ Add Component"))
		{
			// entity.AddComponenet<SpriteRendererComponent>();
			m_AddingComponent = true;
			m_ComponentDialogue.InitState(entity, m_Context);
		}
	}
}