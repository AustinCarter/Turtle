#pragma once

#include "Turtle/Core/Core.h"
#include "Turtle/Events/Event.h"
#include "Turtle/Events/ApplicationEvent.h"
#include "Turtle/Events/MouseEvent.h"
#include "Turtle/Core/Window.h"
#include "Turtle/Core/LayerStack.h"

#include "Turtle/ImGui/ImGuiLayer.h"

#include "Turtle/Core/Timestep.h"


namespace Turtle {

	class Application
	{
	public:
		Application(const std::string& name = "Turtle App");
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);
		
		void PushLayer(Layer *layer);
		void PushOverlay(Layer *layer);

		inline static Application& Get() {return *s_Instance;}
		inline Window& GetWindow() {return *m_Window;}

		void Close();

		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;

		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;

		float m_LastFrameTime = 0.0f;
	private:
		static Application* s_Instance;
	};

	// To be defined in CLIENT
	Application* CreateApplication();

}