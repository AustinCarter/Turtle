#pragma once

#include <Turtle.h>

#include "Panels/SceneHeirarchyPanel.h"
#include "Panels/FileSelector.h"

namespace Turtle {

	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;
		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& event) override;
	private:
		Ref<VertexArray> m_SquareVA;
		OrthographicCameraController m_CameraController;
		Ref<Shader> m_FlatColorShader;
		Ref<Framebuffer> m_Framebuffer;

		Ref<Scene> m_ActiveScene;
		Entity m_SquareEntity;
		Entity m_CameraEntity;
		// Entity m_SecondCamera;

		bool m_PrimaryCamera = true;

		Ref<Texture2D> m_CheckerboardTexture;

		glm::vec4 m_SquareColor = {1.0f, 1.0f, 1.0f, 1.0f};

		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;

		SceneHeirarchyPanel m_SceneHeirarchy;

		FileSelector m_FileSelector; 
	};

}

