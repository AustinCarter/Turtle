#include "SceneHeirarchyPanel.h"

#include <imgui/imgui.h>

#include <glm/gtc/type_ptr.hpp>

#include "Turtle/Scene/Components.h"
#include "Turtle/Core/AssetManager.h"

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
		if (ImGui::BeginPopupContextItem())
		{
			bool removed = ImGui::Selectable("Delete Entity");
			if (removed)
			{
				m_Context->m_Registry.destroy((entt::entity)(uint32_t)entity);
				if(m_SelectionContext == entity)
					m_SelectionContext = {};
			}
			ImGui::EndPopup();
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
			bool open = ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera");
			bool removed = false;
			if (ImGui::BeginPopupContextItem("Camera Component Context Menu"))
			{
				removed = ImGui::Selectable("Remove Component");
				if (removed)
				{
					entity.RemoveComponent<CameraComponent>();
					// open = false;
				}
				ImGui::EndPopup();
				if(removed && open) ImGui::TreePop();
			}
			if (open && !removed)
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
			bool open = ImGui::TreeNodeEx((void*)typeid(SpriteRendererComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Sprite");
			bool removed = false;
			if (ImGui::BeginPopupContextItem("Sprite Renderer Component Context Menu"))
			{
				removed = ImGui::Selectable("Remove Component");
				if (removed)
				{
					entity.RemoveComponent<SpriteRendererComponent>();
					// open = false;
				}
				ImGui::EndPopup();
				if(open && removed) ImGui::TreePop();
			}
			if (open && !removed)
			{
				auto& spriteComponent = entity.GetComponent<SpriteRendererComponent>();
				auto& color = spriteComponent.Color; 
				ImGui::ColorEdit4("Color", glm::value_ptr(color));

				if (spriteComponent.Textured)
				{
					ImGui::Image((void*)spriteComponent.RendererID, ImVec2{ 128, 128 }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
					if(ImGui::Button("Change Texture"))
					{
						m_FileSelector.SetFilter(".png");
						m_FileSelector.Open();
					}
					
					ImGui::SameLine();

					if(ImGui::Button("Remove Texture"))
						spriteComponent.Textured = false;

					
				}
				
				else if(ImGui::Button("Add Texture"))
				{
					m_FileSelector.SetFilter(".png");
					m_FileSelector.Open();
				}

				if(m_FileSelector.Active())
				{
					m_FileSelector.Display();
					if(m_FileSelector.HasSelected())
					{
						std::string path = m_FileSelector.GetSelection();
						m_FileSelector.Close();
						Ref<Texture2D> tex = AssetManager::CreateTexture(path);
						spriteComponent.TextureID = tex->GetAssetID();
						spriteComponent.RendererID = tex->GetRendererID();
						spriteComponent.Textured = true;
					}
				}

				ImGui::TreePop();
			}
			
		}

		if(entity.HasComponent<NativeScriptComponent>())
		{
			bool open = ImGui::TreeNodeEx((void*)typeid(NativeScriptComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Native Script");
			bool removed = false;
			if (ImGui::BeginPopupContextItem("Camera Component Context Menu"))
			{
				removed = ImGui::Selectable("Remove Component");
				if (removed)
				{
					entity.RemoveComponent<NativeScriptComponent>();
					// open = false;
				}
				ImGui::EndPopup();
				if(open && removed) ImGui::TreePop();
			}
			if (open && !removed)
			{
				auto& nativeScriptComponent = entity.GetComponent<NativeScriptComponent>();
				bool bound = nativeScriptComponent.Bound;
				const char* BoundString = bound ? "" : "No script bound";	
				ImGui::Button(BoundString);
				ImGui::TreePop();
			} 
		}

		if(entity.HasComponent<ParticleSpawnerComponenet>())
		{
			bool open = ImGui::TreeNodeEx((void*)typeid(ParticleSpawnerComponenet).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Particle Spawner");
			bool removed = false;
			if (ImGui::BeginPopupContextItem("Particle Spawner Component Context Menu"))
			{
				removed = ImGui::Selectable("Remove Component");
				if (removed)
				{
					entity.RemoveComponent<ParticleSpawnerComponenet>();
					// open = false;
				}
				ImGui::EndPopup();
				if(open && removed) ImGui::TreePop();
			}
			if (open && !removed)
			{
				auto& particleSpawnerComponenet = entity.GetComponent<ParticleSpawnerComponenet>();
				ParticleProps& particle = particleSpawnerComponenet.Particle;
				ImGui::Checkbox("Random Rotate", &particle.RandomRotate);
				ImGui::DragFloat2("Velocity", glm::value_ptr(particle.Velocity),  0.03f, -5.0f, 5.0f);
				ImGui::DragFloat2("Velocity Variation", glm::value_ptr(particle.VelocityVariation),  0.02f, 0.0f, 10.0f);
				ImGui::ColorEdit4("Color Begin", glm::value_ptr(particle.ColorBegin));
				ImGui::ColorEdit4("Color End", glm::value_ptr(particle.ColorEnd));
				ImGui::DragFloat("Size Begin", &particle.SizeBegin, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Size End", &particle.SizeEnd, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Size Variation", &particle.SizeVariation, 0.01f, 0.0f, 1.0f);
				// ImGui::DragFloat("Rotation", &particle.Rotation, 0.03f, 0.0f, 5.0f);
				ImGui::DragFloat("LifeTime", &particle.LifeTime, 0.03f, 0.0f, 5.0f);
				ImGui::DragInt("Emission Rate", (int*)(&particleSpawnerComponenet.EmissionRate), 0.03f, 0, 10);
				ImGui::TreePop();
			} 
		}

		if (ImGui::Button("+ Add Component"))
		{
			m_ComponentDialogue.Open();
		}

		if(m_ComponentDialogue.Active())
		{
			m_ComponentDialogue.Display();
			if(m_ComponentDialogue.HasSelected())
			{
				int compType = m_ComponentDialogue.GetComponentSelection();
				m_ComponentDialogue.Close();

				switch (compType)
				{
					case (int)ComponentTypes::SpriteRendererComponent: 
					{
						entity.AddComponent<SpriteRendererComponent>(); 
						break;
					}
					case (int)ComponentTypes::CameraComponent: 
					{
						entity.AddComponent<CameraComponent>(); 
						m_Context->OnCameraAdd(entity.GetComponent<CameraComponent>()); 
						break;
					}
					case (int)ComponentTypes::NativeScriptComponent:
					{
						entity.AddComponent<NativeScriptComponent>(); 
						break;
					}
					case -1:
						break;
					default:
						TURT_CORE_ERROR("Component Type cannot be added");
				}
			}
		}
	}
}