#include "EditorLayer.h"

#include "imgui/imgui.h"

#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>

#include <chrono>

namespace Turtle {

	EditorLayer::EditorLayer()
		: Layer("EditorLayer"), m_CameraController(1280.0f / 720.0f)
	{
	}

	void EditorLayer::OnAttach()
	{
		TURT_PROFILE_FUNCTION();
			
		// m_CheckerboardTexture = Texture2D::Create("assets/textures/Checkerboard.png");
		m_CheckerboardTexture = AssetManager::CreateTexture("assets/textures/Checkerboard.png");

		FramebufferSpecification fbSpec;
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_Framebuffer = Framebuffer::Create(fbSpec);

		m_ActiveScene = CreateRef<Scene>();

		auto square = m_ActiveScene->CreateEntity("Square Entity");
		square.AddComponent<SpriteRendererComponent>(m_CheckerboardTexture.get()->GetAssetID(), glm::vec4{1.0f, 1.0f, 1.0f, 1.0f});

		auto square2 = m_ActiveScene->CreateEntity("Square Entity 2");
		square2.AddComponent<SpriteRendererComponent>(glm::vec4{0.6f, 0.2f, 0.5f, 1.0f});
		square2.GetComponent<TransformComponent>().Transform[3][0] = rand() % 10  - 5.0f;

		m_SquareEntity = square;

		m_CameraEntity = m_ActiveScene->CreateEntity("Camera A");
		m_CameraEntity.AddComponent<CameraComponent>();

		class CameraController : public ScriptableEntity
		{
		public:
			void OnCreate()
			{
				auto& transform = GetComponent<TransformComponent>().Transform;
				// transform[3][0] = rand() % 10  - 5.0f;
			}

			void OnDestroy()
			{

			}

			void OnUpdate(Timestep ts)
			{
				auto& transform = GetComponent<TransformComponent>().Transform;
				float speed = 5.0f;

				if (Input::IsKeyPressed(TURT_KEY_A))
					transform[3][0] -= speed * ts;
				if (Input::IsKeyPressed(TURT_KEY_D))
					transform[3][0] += speed * ts;
				if (Input::IsKeyPressed(TURT_KEY_W))
					transform[3][1] += speed * ts;
				if (Input::IsKeyPressed(TURT_KEY_S))
					transform[3][1] -= speed * ts;
			}
		};

		m_CameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();

		m_SceneHeirarchy.SetContext(m_ActiveScene);
	}

	void EditorLayer::OnDetach()
	{
		TURT_PROFILE_FUNCTION();
	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		TURT_PROFILE_FUNCTION();

		if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);

			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}

		if(m_ViewportFocused)
			m_CameraController.OnUpdate(ts);
		

		Renderer2D::ResetStats();
		// Render
		
		m_Framebuffer->Bind();
		RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 0.1f});
		RenderCommand::Clear();
	
		m_ActiveScene->OnUpdate(ts);	

		m_Framebuffer->Unbind();
	
	}

	void EditorLayer::OnImGuiRender()
	{
		TURT_PROFILE_FUNCTION();

		static bool dockingEnabled = true;
		static bool opt_fullscreen_persistant = true;
	    bool opt_fullscreen = opt_fullscreen_persistant;
	    static ImGuiDockNodeFlags opt_flags = ImGuiDockNodeFlags_None;

	    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	    // because it would be confusing to have two docking targets within each others.
	    //  ImGuiWindowFlags_MenuBar to re-enable menu bar 
	    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
	    if (opt_fullscreen)
	    {
	        ImGuiViewport* viewport = ImGui::GetMainViewport();
	        ImGui::SetNextWindowPos(viewport->Pos);
	        ImGui::SetNextWindowSize(viewport->Size);
	        ImGui::SetNextWindowViewport(viewport->ID);
	        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	    }

	    // When using ImGuiDockNodeFlags_PassthruDockspace, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
	    if (opt_flags & ImGuiDockNodeFlags_PassthruDockspace)
	        window_flags |= ImGuiWindowFlags_NoBackground;

	    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	    ImGui::Begin("DockSpace Demo", &dockingEnabled, window_flags);
	    ImGui::PopStyleVar();

	    if (opt_fullscreen)
	        ImGui::PopStyleVar(2);

	    // Dockspace
	    ImGuiIO& io = ImGui::GetIO();
	    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	    {
	        ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
	        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), opt_flags);
	    }

	    if (ImGui::BeginMenuBar())
	    {
	        if (ImGui::BeginMenu("File"))
	        {
	            // Disabling fullscreen would allow the window to be moved to the front of other windows, 
	            // which we can't undo at the moment without finer window depth/z control.
	            //ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

	            if (ImGui::MenuItem("Exit")) Application::Get().Close();
	            ImGui::EndMenu();
	        }

	        ImGui::EndMenuBar();
	    }

	    m_SceneHeirarchy.OnImGuiRender();

		ImGui::Begin("Info");

		auto renderStats = Renderer2D::GetStats();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", renderStats.DrawCalls);
		ImGui::Text("Quads: %d", renderStats.QuadCount);
		ImGui::Text("Vertices: %d", renderStats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", renderStats.GetTotalIndexCount());
		auto assetStats = AssetManager::GetStats();
		ImGui::Text("AssetManager Stats:");
		ImGui::Text("Textures Loaded: %d", assetStats.TexturesLoaded);
		if(ImGui::Button("AssetManager::Unload()"))
			AssetManager::Unload();

		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
		ImGui::Begin("Viewport");

		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->SetBlockEvents(!m_ViewportFocused || !m_ViewportHovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
		ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		ImGui::End();
		ImGui::PopStyleVar();

	    ImGui::End();

	}

	void EditorLayer::OnEvent(Event& event)
	{
		m_CameraController.OnEvent(event);
	}

}