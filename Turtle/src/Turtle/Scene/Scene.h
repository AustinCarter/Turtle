#pragma once

#include <entt.hpp>

#include "Turtle/Core/Timestep.h"
#include <entt.hpp>


namespace Turtle {
	class Entity;
	struct CameraComponent;
	class Scene 
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());
		void DestroyEntity(Entity entity);

		void OnUpdate(Timestep ts);
		void OnViewportResize(uint32_t width, uint32_t height);

		void OnCameraConstruct(entt::registry& registry, entt::entity entity);

		void SerializeScene(const std::string& filepath);
		void DeserializeScene(const std::string& filepath);

	private:
		entt::registry m_Registry;
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

		friend class Entity;
		friend class SceneHeirarchyPanel;

	};
}