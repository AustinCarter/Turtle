#include "turtpch.h"

#include "Turtle/Scene/Components.h"
#include "Turtle/Utils/IMGuiUILib.h"
#include <imgui.h>
#include <imgui_internal.h>

#include "Turtle/Utils/PlatformUtils.h"


#include <glm/gtc/type_ptr.hpp>

namespace Turtle{

	//NOTE: Tag and Transform UI is being drawn in the SceneHeirarchy Panel at the moment to ensure they are shown at the top
	void TagComponent::DrawUI(Entity entity){}
	void TransformComponent::DrawUI(Entity entity){}

	void SpriteRendererComponent::DrawUI(Entity entity)
	{
		ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_FramePadding;

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

			if (Textured)
			{
				ImGui::Image((void*)spriteComponent.RendererID, ImVec2{ 100, 100 }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
				ImGui::SameLine();

				if(ImGui::Button("Remove Texture"))
					spriteComponent.Textured = false;
				
			}
			
			else if(ImGui::Button("Add Texture"))
			{
				std::string path = FileDialogs::OpenFile("*.png\0*.png\0");
				spriteComponent.Texture = AssetManager::CreateTexture(path);
				spriteComponent.RendererID = spriteComponent.Texture->GetRendererID();
				spriteComponent.Textured = true;
			}

			ImGui::TreePop();
		}
	}

	void CameraComponent::DrawUI(Entity entity)
	{
		ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_FramePadding;
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

	void NativeScriptComponent::DrawUI(Entity entity)
	{
		ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_FramePadding;
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
			// auto& nativeScriptComponent = entity.GetComponent<NativeScriptComponent>();
			const char* BoundString = Bound ? "" : "No script bound";	
			ImGui::Button(BoundString);
			ImGui::TreePop();
		} 
	}

	void ParticleSpawnerComponent::DrawUI(Entity entity)
	{
		ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_FramePadding;
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

	void TileSetComponent::DrawUI(Entity entity)
	{
		ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_FramePadding;
		bool open = ImGui::TreeNodeEx((void*)typeid(TileSetComponent).hash_code(), treeNodeFlags, "Tile Set");
		bool removed = false;
		if (ImGui::BeginPopupContextItem("Tile Set Component Context Menu"))
		{
			removed = ImGui::Selectable("Remove Component");
			if (removed)
			{
				entity.RemoveComponent<TileSetComponent>();
			}
			ImGui::EndPopup();
			if(open && removed) ImGui::TreePop();
		}
		if (open && !removed)
		{
			auto& tileSetComponent = entity.GetComponent<TileSetComponent>();
			
			ImGui::Checkbox("Active", &tileSetComponent.DisplayPallette);

			if(tileSetComponent.DisplayPallette)
			{
				ImGui::Begin("Tile Pallette"); 
				ImGuiIO& io = ImGui::GetIO();
				ImGui::BeginChild("Tile Set", ImVec2{ ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y - 30 }, false, ImGuiWindowFlags_HorizontalScrollbar);
				ImVec2 p = ImGui::GetCursorScreenPos();
				ImGui::Image((void*)tileSetComponent.TileSet->GetRendererID(), ImVec2{ (float)tileSetComponent.TileSet->GetWidth(), (float)tileSetComponent.TileSet->GetHeight()},ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
				
				if(!tileSetComponent.TileWidth) tileSetComponent.TileWidth = tileSetComponent.TileSet->GetWidth();
				if(!tileSetComponent.TileHeight) tileSetComponent.TileHeight = tileSetComponent.TileSet->GetHeight();

				for(int x = 1; x < tileSetComponent.TileSet->GetWidth()/tileSetComponent.TileWidth; x++)
				{
					ImGui::GetWindowDrawList()->AddLine(ImVec2(p.x + tileSetComponent.TileWidth * x , p.y), ImVec2(p.x + tileSetComponent.TileWidth * x, p.y + tileSetComponent.TileSet->GetHeight()), IM_COL32(0, 0, 0, 255), 1.0f);
				}
				for(int y = 1; y < tileSetComponent.TileSet->GetHeight()/tileSetComponent.TileHeight; y++)
				{
					ImGui::GetWindowDrawList()->AddLine(ImVec2(p.x, p.y + tileSetComponent.TileHeight * y), ImVec2(p.x +  tileSetComponent.TileSet->GetWidth(), p.y + tileSetComponent.TileHeight * y), IM_COL32(0, 0, 0, 255), 1.0f);
				}

				if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0))
				{
					tileSetComponent.SelectedX = (int)(io.MousePos.x - p.x) / tileSetComponent.TileWidth;
					tileSetComponent.SelectedY = (int)(io.MousePos.y - p.y) / tileSetComponent.TileHeight;
					// TURT_WARN("selected: {0}, {1}", (int)(io.MousePos.x - p.x)/128, (int)(io.MousePos.y - p.y)/128);
				}

				int xPos = tileSetComponent.TileWidth * SelectedX;
				int yPos = tileSetComponent.TileHeight * tileSetComponent.SelectedY;
				ImGui::GetWindowDrawList()->AddLine(ImVec2(p.x + xPos , p.y + yPos), ImVec2(p.x + xPos + tileSetComponent.TileWidth, p.y + yPos), IM_COL32(255, 0, 0, 255), 3.0f);
				ImGui::GetWindowDrawList()->AddLine(ImVec2(p.x + xPos + tileSetComponent.TileWidth , p.y + yPos), ImVec2(p.x + xPos + tileSetComponent.TileWidth, p.y + yPos + tileSetComponent.TileHeight), IM_COL32(255, 0, 0, 255), 3.0f);
				ImGui::GetWindowDrawList()->AddLine(ImVec2(p.x + xPos , p.y + yPos ), ImVec2(p.x + xPos, p.y + yPos + tileSetComponent.TileHeight), IM_COL32(255, 0, 0, 255), 3.0f);
				ImGui::GetWindowDrawList()->AddLine(ImVec2(p.x + xPos , p.y + yPos + tileSetComponent.TileHeight), ImVec2(p.x + xPos + tileSetComponent.TileWidth, p.y + yPos + tileSetComponent.TileHeight), IM_COL32(255, 0, 0, 255), 3.0f);

				ImGui::EndChild();
				if(ImGui::Button("Close"))
					tileSetComponent.DisplayPallette = false;
				ImGui::End();

				uint32_t TileSetWidth = tileSetComponent.TileSet->GetWidth();
				uint32_t TileSetHeight = tileSetComponent.TileSet->GetHeight();

				ImVec2 min = ImVec2( (float)(SelectedX * tileSetComponent.TileWidth)/TileSetWidth, 
					1.0f - (float)(tileSetComponent.SelectedY * tileSetComponent.TileHeight)/TileSetHeight);
				ImVec2 max = ImVec2( (float)((SelectedX + 1) * tileSetComponent.TileWidth)/TileSetWidth, 
					1.0f - (float)((tileSetComponent.SelectedY + 1) * tileSetComponent.TileHeight)/TileSetHeight);

				ImGui::Image((void*)tileSetComponent.TileSet->GetRendererID(),  ImVec2{ (float)tileSetComponent.TileWidth, (float)tileSetComponent.TileHeight}, min, max);
			}

			ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth() * 0.5f);
			ImGui::InputScalar("Tile Width", ImGuiDataType_S32, &tileSetComponent.TileWidth);
			ImGui::InputScalar("Tile Height", ImGuiDataType_S32, &tileSetComponent.TileHeight);
			ImGui::PopItemWidth();

			ImGui::TreePop();
		} 
	}

	void GridComponent::DrawUI(Entity entity)
	{
		ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_FramePadding;

		if (ImGui::TreeNodeEx((void*)typeid(GridComponent).hash_code(), treeNodeFlags, "Grid"))
		{
			auto& gridComponent = entity.GetComponent<GridComponent>();
			ImGui::Checkbox("Active", &gridComponent.Active);
			ImGui::DragFloat("Size", &gridComponent.GridSize, 0.01f);

			ImGui::TreePop();
		}
	}

	void TileMapComponent::DrawUI(Entity entity)
	{
		ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_FramePadding;
		if (ImGui::TreeNodeEx((void*)typeid(TileMapComponent).hash_code(), treeNodeFlags, "Tile Map"))
		{

			ImGui::TreePop();
		}

	}
}
