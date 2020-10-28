#include "SceneHeirarchyPanel.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include <glm/gtc/type_ptr.hpp>

#include "Turtle/Scene/Components.h"
#include "Turtle/Core/AssetManager.h"
#include "Turtle/Utils/PlatformUtils.h"

#include "Turtle/Utils/ImGuiUILib.h"
	

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

		if (ImGui::IsMouseClicked(0) && ImGui::IsWindowHovered())
		 	m_SelectionContext = {};

		if(ImGui::BeginPopupContextWindow(0, 1, false))
		{
			if (ImGui::MenuItem("Create Entity"))
				m_Context->CreateEntity();

			ImGui::EndPopup();
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

	void SceneHeirarchyPanel::DrawComponents(Entity entity)
	{
		ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_FramePadding;
	
		//NOTE: Tag and Transform are currently being drawn here instead of in their ui functions as all entities should have 
		//       these components and it is desierable to ensure that they are always drawn first
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
				DrawVec3Control("Translation", transformComponent.Translation, 0.0f);
				glm::vec3 rotation = glm::degrees(transformComponent.Rotation);
				DrawVec3Control("Rotation", rotation, 0.0f, 0.5f);
				transformComponent.Rotation = glm::radians(rotation);
				DrawVec3Control("Scale", transformComponent.Scale, 1.0f);

				ImGui::TreePop();
			}
		}

		m_Context->m_Registry.visit(entity, [&](const auto component){
			auto type = entt::resolve_type(component);	
			auto instance = type.ctor<decltype(m_Context->m_Registry), entt::entity>().invoke(std::ref(m_Context->m_Registry), (entt::entity)entity);
			type.func("DrawUI"_hs).invoke(instance, entity);
		});

		if (ImGui::Button("+ Add Component"))
			ImGui::OpenPopup("AddComponent");
		if (ImGui::BeginPopup("AddComponent"))
		{			
			entt::meta_range<entt::meta_type> range{ entt::internal::meta_context::local() };
			for (auto& typeID : range)
			{
				auto type = entt::resolve_type(typeID.type_id());
				if(type)
				{
					entt::meta_prop isComponent = type.prop("Component"_hs);
					if(isComponent)
					{
						auto typeString = type.prop("Name"_hs).value().try_cast<char const*>();
					
						if(ImGui::MenuItem(*typeString))
						{
							type.ctor<Entity>().invoke(entity);
						}
					}
				}
			}

			ImGui::EndPopup();
		}
	}
}