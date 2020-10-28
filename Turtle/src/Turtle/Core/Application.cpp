#include "turtpch.h"
#include "Turtle/Core/Application.h"

#include "Turtle/Core/Log.h"

#include "Turtle/Core/input.h"
#include "Turtle/Renderer/Renderer.h"
#include "Turtle/Core/AssetManager.h"
#include "Turtle/Core/Random.h"

#include "Turtle/Core/MetaRegistry.h"


#include "glfw/glfw3.h"

namespace Turtle {

	Application* Application::s_Instance = nullptr;

	
	Application::Application(const std::string& name)
	{
		TURT_PROFILE_FUNCTION();

		TURT_CORE_ASSERT(!s_Instance, "Application already exists.");
		s_Instance = this;


		m_Window = Window::Create(WindowProps(name));
		m_Window->SetEventCallback(TURT_BIND_EVENT_FN(Application::OnEvent));

		InitMetaRegistry();
		//initiate sub-systems
		AssetManager::Init();
		Renderer::Init();
		Random::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{
		TURT_PROFILE_FUNCTION();
		//shutdown sub=systems
		// Renderer::Shutdown();
		// AssetManager::Shutdown();
	}

	void Application::OnEvent(Event& e)
	{
		TURT_PROFILE_FUNCTION();
		
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(TURT_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(TURT_BIND_EVENT_FN(Application::OnWindowResized));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			if (e.Handled)
				break;
			(*--it)->OnEvent(e);
		}

		//TURT_CORE_TRACE("{0}",e);
	}

	void Application::PushLayer(Layer *layer)
	{
		TURT_PROFILE_FUNCTION();
		
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer *layer)
	{
		TURT_PROFILE_FUNCTION();
		
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::Run()
	{
		TURT_PROFILE_FUNCTION();
		
		while (m_Running)
		{

			TURT_PROFILE_SCOPE("Run Loop");
		
			float time = (float)glfwGetTime(); // Platform::GetTime();
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if(!m_Minimized)
			{
				{
					TURT_PROFILE_SCOPE("LayerStack updates");
					for (Layer *layer : m_LayerStack)
						layer->OnUpdate(timestep);	
				}
				
				{
					TURT_PROFILE_SCOPE("LayerStack ImGuiRenders");

					m_ImGuiLayer->Begin();
					for (Layer *layer : m_LayerStack)
						layer->OnImGuiRender();
					m_ImGuiLayer->End();
				}
			}
			

			m_Window->OnUpdate();

		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResized(WindowResizeEvent& e)
	{
		TURT_PROFILE_FUNCTION();
		
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}
		m_Minimized = false;

		Renderer::OnWindowResized(e.GetWidth(), e.GetHeight());

		return false;
	}

	void Application::Close()
	{
		m_Running = false;
	}

}