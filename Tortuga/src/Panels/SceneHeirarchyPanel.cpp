#include "SceneHeirarchyPanel.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include <glm/gtc/type_ptr.hpp>

#include "Turtle/Scene/Components.h"
#include "Turtle/Core/AssetManager.h"
#include "Turtle/Utils/PlatformUtils.h"

namespace Turtle {
	SceneHeirarchyPanel::SceneHeirarchyPanel(const Ref<Scene>& scene)
	{
		SetContext(scene);
	}

	void SceneHeirarchyPanel::SetContext(const Ref<Scene>& scene)
	{
		m_Context = scene;
		m_SelectionContext = {};
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

		if(ImGui::BeginPopupContextWindow(0, 1, false))
		{
			if (ImGui::MenuItem("Create Entity"))
				m_Context->CreateEntity();

			ImGui::EndPopup();
		}

		// if (ImGui::Button("Add Entity"))
		// {
		// 	m_Context->CreateEntity();
		// }

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
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
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
				m_Context->DestroyEntity(entity);
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

	template <typename T, typename UIFunction>
	static void DrawComponent(const std::string& name, Entity entity, UIFunction funtion)
	{

	}

	static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue, float speed = 0.1f, float columnWidth = 100.f)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];
		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0});
		// ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = {lineHeight + 3.0f, lineHeight};

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.9f, 0.2f, 0.2f, 1.0f});
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
		ImGui::PushFont(boldFont);
		if (ImGui::Button("x", buttonSize))
			values.x = resetValue;
		ImGui::PopStyleColor(3);
		ImGui::PopFont();

		ImGui::SameLine();
		ImGui::DragFloat("##x", &values.x, speed, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine(); 

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.3f, 0.8f, 0.2f, 1.0f});
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
		ImGui::PushFont(boldFont);
		if (ImGui::Button("y", buttonSize))
			values.y = resetValue;
		ImGui::PopStyleColor(3);
		ImGui::PopFont();

		ImGui::SameLine();
		ImGui::DragFloat("##y", &values.y, speed, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine(); 

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.1f, 0.25f, 0.8f, 1.0f});
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.2f, 0.35f, 0.9f, 1.0f});
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.1f, 0.25f, 0.8f, 1.0f});
		ImGui::PushFont(boldFont);
		if (ImGui::Button("z", buttonSize))
			values.z = resetValue;
		ImGui::PopStyleColor(3);
		ImGui::PopFont();

		ImGui::SameLine();
		ImGui::DragFloat("##z", &values.z, speed, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth(); 

		ImGui::PopStyleVar();
		ImGui::Columns(1);

		ImGui::PopID();
	}

	void SceneHeirarchyPanel::DrawComponents(Entity entity)
	{

		ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_FramePadding;

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
			if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), treeNodeFlags, "Transform"))
			{
				auto& transformComponent = entity.GetComponent<TransformComponent>();
				// ImGui::DragFloat3("Translation"     , glm::value_ptr(transformComponent.Translation), 0.1f);
				// ImGui::DragFloat3("Rotation", glm::value_ptr(transformComponent.Rotation), 0.1f);
				// ImGui::DragFloat3("Scale", glm::value_ptr(transformComponent.Scale), 0.1f);
				DrawVec3Control("Translation", transformComponent.Translation, 0.0f);
				glm::vec3 rotation = glm::degrees(transformComponent.Rotation);
				DrawVec3Control("Rotation", rotation, 0.0f, 0.5f);
				transformComponent.Rotation = glm::radians(rotation);
				DrawVec3Control("Scale", transformComponent.Scale, 1.0f);

				ImGui::TreePop();
			}
		}

		if(entity.HasComponent<CameraComponent>())
		{
			bool open = ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), treeNodeFlags, "Camera");
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
			bool open = ImGui::TreeNodeEx((void*)typeid(SpriteRendererComponent).hash_code(), treeNodeFlags, "Sprite");
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
					ImGui::Image((void*)spriteComponent.RendererID, ImVec2{ 100, 100 }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
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
					std::string path = FileDialogs::OpenFile("*.png\0*.png\0");
					Ref<Texture2D> tex = AssetManager::CreateTexture(path);
					spriteComponent.Texture = tex;
					spriteComponent.RendererID = tex->GetRendererID();
					spriteComponent.Textured = true;

					m_FileSelector.SetFilter(".png");
					m_FileSelector.Open();
				}

				ImGui::TreePop();
			}
			
		}

		if(entity.HasComponent<NativeScriptComponent>())
		{
			bool open = ImGui::TreeNodeEx((void*)typeid(NativeScriptComponent).hash_code(), treeNodeFlags, "Native Script");
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

		if(entity.HasComponent<ParticleSpawnerComponent>())
		{
			bool open = ImGui::TreeNodeEx((void*)typeid(ParticleSpawnerComponent).hash_code(), treeNodeFlags, "Particle Spawner");
			bool removed = false;
			if (ImGui::BeginPopupContextItem("Particle Spawner Component Context Menu"))
			{
				removed = ImGui::Selectable("Remove Component");
				if (removed)
				{
					entity.RemoveComponent<ParticleSpawnerComponent>();
					// open = false;
				}
				ImGui::EndPopup();
				if(open && removed) ImGui::TreePop();
			}
			if (open && !removed)
			{
				auto& particleSpawnerComponent = entity.GetComponent<ParticleSpawnerComponent>();
				ParticleProps& particle = particleSpawnerComponent.Particle;
				ImGui::PushItemWidth(100.0f);

				ImGui::Text("Random Rotate");
				ImGui::NextColumn();
				ImGui::Checkbox("##Random Rotate", &particle.RandomRotate);
				ImGui::NextColumn();


				DrawVec3Control("Velocity",  particle.Velocity, 1.0f);
				DrawVec3Control("Velocity Variation",  particle.VelocityVariation, 1.0f);

				ImGui::Columns(2);
				ImGui::SetColumnWidth(0, 100.0f);

				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0});
				ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2{ 0, 0 });
				ImGui::Text("Color Begin");
				ImGui::NextColumn();
				ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth()*0.8f);
				ImGui::ColorEdit4("##Color Begin", glm::value_ptr(particle.ColorBegin));
				ImGui::PopItemWidth();
				ImGui::NextColumn();

				ImGui::Text("Color End");
				ImGui::NextColumn();
				ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth()*0.8f);
				ImGui::ColorEdit4("##Color End", glm::value_ptr(particle.ColorEnd));
				ImGui::PopItemWidth();
				ImGui::NextColumn();

				ImGui::Text("Size Begin");
				ImGui::NextColumn();
				ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth()*0.8f);
				ImGui::DragFloat("##Size Begin", &particle.SizeBegin, 0.01f, 0.0f, 1.0f);
				ImGui::PopItemWidth();
				ImGui::NextColumn();

				ImGui::Text("Size End");
				ImGui::NextColumn();
				ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth()*0.8f);
				ImGui::DragFloat("##Size End", &particle.SizeEnd, 0.01f, 0.0f, 1.0f);
				ImGui::PopItemWidth();
				ImGui::NextColumn();

				ImGui::Text("Size Variation");
				ImGui::NextColumn();
				ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth()*0.8f);
				ImGui::DragFloat("##Size Variation", &particle.SizeVariation, 0.01f, 0.0f, 1.0f);
				ImGui::PopItemWidth();
				ImGui::NextColumn();
				// ImGui::DragFloat("Rotation", &particle.Rotation, 0.03f, 0.0f, 5.0f);
				ImGui::Text("LifeTime");
				ImGui::NextColumn();
				ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth()*0.8f);
				ImGui::DragFloat("##LifeTime", &particle.LifeTime, 0.03f, 0.0f, 5.0f);
				ImGui::PopItemWidth();
				ImGui::NextColumn();

				ImGui::Text("Emission Rate");
				ImGui::NextColumn();
				ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth()*0.8f);
				ImGui::DragInt("##Emission Rate", (int*)(&particleSpawnerComponent.EmissionRate), 0.03f, 0, 10);
				ImGui::PopItemWidth();

	
				ImGui::PopStyleVar(2);
				// ImGui::PopItemWidth();
				ImGui::Columns(1);

				ImGui::TreePop();
			} 
		}
		if(entity.HasComponent<TileSetComponent>())
		{
			bool open = ImGui::TreeNodeEx((void*)typeid(TileSetComponent).hash_code(), treeNodeFlags, "Tile Set");
			bool removed = false;
			if (ImGui::BeginPopupContextItem("Tile Set Component Context Menu"))
			{
				removed = ImGui::Selectable("Remove Component");
				if (removed)
				{
					entity.RemoveComponent<TileSetComponent>();
					entity.RemoveComponent<GridComponent>();
					entity.RemoveComponent<TileMapComponent>();
					// open = false;
				}
				ImGui::EndPopup();
				if(open && removed) ImGui::TreePop();
			}
			if (open && !removed)
			{
				auto& tileSetComponent = entity.GetComponent<TileSetComponent>();
				if(ImGui::Button("Use Pallette"))
				{
					m_TilePallette.SetTexture(tileSetComponent.TileSet);
					m_TilePallette.SetTileSize(tileSetComponent.TileWidth, tileSetComponent.TileHeight);
					m_TilePallette.Open();
				}
				ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth() * 0.5f);
				if(ImGui::InputScalar("Tile Width", ImGuiDataType_S32, &tileSetComponent.TileWidth))
				{
					m_TilePallette.SetTileSize(tileSetComponent.TileWidth, tileSetComponent.TileHeight);
				}
				if(ImGui::InputScalar("Tile Height", ImGuiDataType_S32, &tileSetComponent.TileHeight))
				{
					m_TilePallette.SetTileSize(tileSetComponent.TileWidth, tileSetComponent.TileHeight);
				}
				ImGui::PopItemWidth();

				if(m_TilePallette.Active())
				{	
					m_TilePallette.Display();
					int* selection = m_TilePallette.GetSelection();
					uint32_t width = tileSetComponent.TileSet.get()->GetWidth();
					uint32_t height = tileSetComponent.TileSet.get()->GetHeight();

					ImVec2 min = ImVec2((float)(selection[0] * tileSetComponent.TileWidth)/width, 
						1.0f - (float)(selection[1] * tileSetComponent.TileHeight)/height);
					ImVec2 max = ImVec2((float)((selection[0] + 1) * tileSetComponent.TileWidth)/width, 
						1.0f - (float)((selection[1] + 1) * tileSetComponent.TileHeight)/height);

					ImGui::Image((void*)tileSetComponent.TileSet->GetRendererID(),  ImVec2{ (float)tileSetComponent.TileWidth, (float)tileSetComponent.TileHeight}, min, max);
				}
				ImGui::TreePop();
			} 
		}

		if(entity.HasComponent<GridComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(GridComponent).hash_code(), treeNodeFlags, "Grid"))
			{
				auto& gridComponent = entity.GetComponent<GridComponent>();
				ImGui::Checkbox("Active", &gridComponent.Active);
				ImGui::DragFloat("Size", &gridComponent.GridSize, 0.01f);

				ImGui::TreePop();
			}
		}

		if (ImGui::Button("+ Add Component"))
			ImGui::OpenPopup("AddComponent");
		
		if (ImGui::BeginPopup("AddComponent"))
		{
			m_Context->m_Registry.visit([&](const auto component){
				auto type = entt::resolve_type(component);	
				auto typeString = type.prop("Name"_hs).value().try_cast<char const*>();

				if(ImGui::MenuItem(*typeString))
				{
					type.ctor<Entity>().invoke(entity);
				}
			});
			ImGui::EndPopup();
		}
	}
}