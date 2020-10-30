#include "turtpch.h"

#include "Turtle/Scene/Components.h"
#include "Turtle/Utils/IMGuiUILib.h"
#include <imgui.h>
#include <imgui_internal.h>

#include "Turtle/Utils/PlatformUtils.h"

#include <filesystem>


#include <glm/gtc/type_ptr.hpp>

namespace Turtle{

	template<typename T, typename UIFunction>
	static void DrawComponent(const std::string& name, Entity entity, UIFunction uiFunction)
	{
		ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_FramePadding;

		bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
		bool removed = false;
		if (ImGui::BeginPopupContextItem("Component Context Menu"))
		{
			removed = ImGui::Selectable("Remove Component");
			if (removed)
			{
				entity.RemoveComponent<T>();
				// open = false;
			}
			ImGui::EndPopup();
			if(open && removed) ImGui::TreePop();
		}
		if (open && !removed)
		{
			auto& component = entity.GetComponent<T>();
			uiFunction(component);
			ImGui::TreePop();
		}
	}

	//NOTE: Tag and Transform UI is being drawn in the SceneHeirarchy Panel at the moment to ensure they are shown at the top
	void TagComponent::DrawUI(Entity entity){}
	void TransformComponent::DrawUI(Entity entity){}

	void SpriteRendererComponent::DrawUI(Entity entity)
	{
		DrawComponent<SpriteRendererComponent>("Sprite", entity, [](auto& component)
		{
			auto& color = component.Color; 
			ImGui::ColorEdit4("Color", glm::value_ptr(color));

			if (component.Textured)
			{
				ImGui::Image((void*)component.RendererID, ImVec2{ 100, 100 }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
				ImGui::SameLine();

				if(ImGui::Button("Remove Texture"))
					component.Textured = false;
				
			}
			
			else if(ImGui::Button("Add Texture"))
			{
				std::string path = FileDialogs::OpenFile("*.png\0*.png\0");
				if(!path.empty())
				{
					component.Texture = AssetManager::CreateTexture(path);
					component.RendererID = component.Texture->GetRendererID();
					component.Textured = true;
				}
			}
		});
	}

	void CameraComponent::DrawUI(Entity entity)
	{
		DrawComponent<CameraComponent>("Camera", entity, [](auto& component)
		{
			auto& camera = component.Camera;

			ImGui::Checkbox("Primary", &component.Primary);

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


				ImGui::Checkbox("Fixed Aspect Ratio", &component.FixedAspectRatio);	
			}
		});
	}

	void ScriptComponent::DrawUI(Entity entity)
	{
		DrawComponent<ScriptComponent>("Script", entity, [](auto& component)
		{
			if(component.Script)
			{
				ImGui::Text("Current Script %s", component.Script->GetFilepath().c_str());
				// table is in the stack at index 't'
				lua_State* L = component.Script->GetState();
				lua_getglobal(L, "Props");
				int top = lua_gettop(L);  // first key
				lua_pushnil(L);
				if (!lua_isnil(L, top))
				{
					ImGui::Columns(2);
					ImGui::SetColumnWidth(0, 150.0f);
					while (lua_next(L, top) != 0)
					{
						// uses 'key' (at index -2) and 'value' (at index -1)
						

						const char* fieldName = lua_tostring(L, -2);
						float value = lua_tonumber(L, -1);

						ImGui::Text(fieldName);
						ImGui::NextColumn();
						ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth()*0.8f);
						ImGui::PushID(fieldName);
						ImGui::DragFloat("", &value, 0.1f);
						ImGui::PopID();
						ImGui::PopItemWidth();
						ImGui::NextColumn();

						lua_pushnumber(L, value);
						lua_setfield(L, top, fieldName);
						lua_pop(L, 1);
					}
					ImGui::Columns(1);
				}
			}
			if(ImGui::Button("Set Script"))
			{
				component.Script = CreateRef<LuaScript>();
				std::string filepath = FileDialogs::OpenFile("*.lua\0*.lua\0");
				if(component.Script->LoadScriptFromFile(filepath) != LUA_OK)
					component.Script->LogError();
				component.Script->ExecuteScript();
			}
			
		});
	}

	void ParticleSpawnerComponent::DrawUI(Entity entity)
	{

		DrawComponent<ParticleSpawnerComponent>("Particle Spawner", entity, [](auto& component)
		{
			ParticleProps& particle = component.Particle;

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
			ImGui::DragInt("##Emission Rate", (int*)(&component.EmissionRate), 0.03f, 0, 10);
			ImGui::PopItemWidth();


			ImGui::PopStyleVar(2);
			// ImGui::PopItemWidth();
			ImGui::Columns(1);
		});

	}

	void TileSetComponent::DrawUI(Entity entity)
	{

		DrawComponent<TileSetComponent>("Tile Set", entity, [&](auto& component)
		{
			ImGui::Checkbox("Active", &component.DisplayPallette);

			if(component.DisplayPallette)
			{
				ImGui::Begin("Tile Pallette"); 
				ImGuiIO& io = ImGui::GetIO();
				ImGui::BeginChild("Tile Set", ImVec2{ ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y - 30 }, false, ImGuiWindowFlags_HorizontalScrollbar);
				ImVec2 p = ImGui::GetCursorScreenPos();
				ImGui::Image((void*)component.TileSet->GetRendererID(), ImVec2{ (float)component.TileSet->GetWidth(), (float)component.TileSet->GetHeight()},ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
				
				if(!component.TileWidth) component.TileWidth = component.TileSet->GetWidth();
				if(!component.TileHeight) component.TileHeight = component.TileSet->GetHeight();

				for(int x = 1; x < component.TileSet->GetWidth()/component.TileWidth; x++)
				{
					ImGui::GetWindowDrawList()->AddLine(ImVec2(p.x + component.TileWidth * x , p.y), ImVec2(p.x + component.TileWidth * x, p.y + component.TileSet->GetHeight()), IM_COL32(0, 0, 0, 255), 1.0f);
				}
				for(int y = 1; y < component.TileSet->GetHeight()/component.TileHeight; y++)
				{
					ImGui::GetWindowDrawList()->AddLine(ImVec2(p.x, p.y + component.TileHeight * y), ImVec2(p.x +  component.TileSet->GetWidth(), p.y + component.TileHeight * y), IM_COL32(0, 0, 0, 255), 1.0f);
				}

				if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0))
				{
					component.SelectedX = (int)(io.MousePos.x - p.x) / component.TileWidth;
					component.SelectedY = (int)(io.MousePos.y - p.y) / component.TileHeight;
					// TURT_WARN("selected: {0}, {1}", (int)(io.MousePos.x - p.x)/128, (int)(io.MousePos.y - p.y)/128);
				}

				int xPos = component.TileWidth * SelectedX;
				int yPos = component.TileHeight * component.SelectedY;
				ImGui::GetWindowDrawList()->AddLine(ImVec2(p.x + xPos , p.y + yPos), ImVec2(p.x + xPos + component.TileWidth, p.y + yPos), IM_COL32(255, 0, 0, 255), 3.0f);
				ImGui::GetWindowDrawList()->AddLine(ImVec2(p.x + xPos + component.TileWidth , p.y + yPos), ImVec2(p.x + xPos + component.TileWidth, p.y + yPos + component.TileHeight), IM_COL32(255, 0, 0, 255), 3.0f);
				ImGui::GetWindowDrawList()->AddLine(ImVec2(p.x + xPos , p.y + yPos ), ImVec2(p.x + xPos, p.y + yPos + component.TileHeight), IM_COL32(255, 0, 0, 255), 3.0f);
				ImGui::GetWindowDrawList()->AddLine(ImVec2(p.x + xPos , p.y + yPos + component.TileHeight), ImVec2(p.x + xPos + component.TileWidth, p.y + yPos + component.TileHeight), IM_COL32(255, 0, 0, 255), 3.0f);

				ImGui::EndChild();
				if(ImGui::Button("Close"))
					component.DisplayPallette = false;
				ImGui::End();

				uint32_t TileSetWidth = component.TileSet->GetWidth();
				uint32_t TileSetHeight = component.TileSet->GetHeight();

				ImVec2 min = ImVec2( (float)(SelectedX * component.TileWidth)/TileSetWidth, 
					1.0f - (float)(component.SelectedY * component.TileHeight)/TileSetHeight);
				ImVec2 max = ImVec2( (float)((SelectedX + 1) * component.TileWidth)/TileSetWidth, 
					1.0f - (float)((component.SelectedY + 1) * component.TileHeight)/TileSetHeight);

				ImGui::Image((void*)component.TileSet->GetRendererID(),  ImVec2{ (float)component.TileWidth, (float)component.TileHeight}, min, max);
			}

			ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth() * 0.5f);
			ImGui::InputScalar("Tile Width", ImGuiDataType_S32, &component.TileWidth);
			ImGui::InputScalar("Tile Height", ImGuiDataType_S32, &component.TileHeight);
			ImGui::PopItemWidth();
		});		
	}

	void GridComponent::DrawUI(Entity entity)
	{
		DrawComponent<GridComponent>("Grid", entity, [](auto& component)
		{
			ImGui::Checkbox("Active", &component.Active);
			ImGui::DragFloat("Size", &component.GridSize, 0.01f);
		});		
	}

	void TileMapComponent::DrawUI(Entity entity)
	{
		DrawComponent<TileMapComponent>("Tile Map", entity, [](auto& component)
		{
		});	
	}
}
