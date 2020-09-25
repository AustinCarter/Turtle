#pragma once


#include "Turtle/Scene/Entity.h"
#include "Turtle/Scene/Scene.h"

namespace Turtle {

	class ComponentAddPanel
	{
	public:
		ComponentAddPanel() = default;

		bool OnImGuiRender();
		void ResetState();

		void OnCameraAdd(CameraComponent cameraComponent);

		void InitState(Entity entity,  const Ref<Scene>& scene) 
			{ m_Entity = entity; m_Context = scene;}
	private:
		bool m_ComponentCreated = false;
		int m_ComponentType = -1;

		Ref<Scene> m_Context;

		Entity m_Entity; 
	};
}