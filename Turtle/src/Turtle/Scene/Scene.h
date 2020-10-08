#pragma once

#include <entt.hpp>

#include "Turtle/Core/Timestep.h"


namespace Turtle {
	class Entity;
	struct CameraComponent;
	class Scene 
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());
		void DestoryEntity();

		void OnUpdate(Timestep ts);
		void OnViewportResize(uint32_t width, uint32_t height);

		void OnCameraAdd(CameraComponent& cameraComponent);

	private:
		entt::registry m_Registry;
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

		friend class Entity;
		friend class SceneHeirarchyPanel;

	};
}