#include "EditorLayer.h"

#include "imgui/imgui.h"

#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>


#include "Turtle/Utils/PlatformUtils.h"

#include <chrono>

namespace Turtle {

	// ----------------------- TEMP -----------------------------------------
	struct GlobalMetaFunctionsComponent {};

	void HelloWorld()
	{
		printf("Hello, World\n");
	}

	static void HelloWorld2()
	{
		printf("Hello, World 2\n");
	}

	static void HelloWorld3(uint32_t x, int y)
	{
		printf("Hello, World 3 (%d, %d)\n", x, y);
	}

	int Add(int a, int b)
	{
		return a + b;
	}

	struct Sprite
	{
		int x;
		int y;

		Sprite() : x(0), y(0) {}
		~Sprite() {}

		int Move(int velX, int velY)
		{
			x += velX;
			y += velY;
			return x + y;
		}

		void Draw()
		{
			printf("sprite(%p): x = %d, y = %d\n", this, x, y);
		}
	};

	static Sprite Instance;

	static Sprite& GetSprite()
	{
		return Instance;
	}

	// ---------------------------------------------------------------------

	EditorLayer::EditorLayer()
		: Layer("EditorLayer"), m_CameraController(1280.0f / 720.0f)
	{
	}

	void EditorLayer::OnAttach()
	{
		TURT_PROFILE_FUNCTION();


		//m_CheckerboardTexture = AssetManager::CreateTexture("assets/textures/Checkerboard.png");

		FramebufferSpecification fbSpec;
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_Framebuffer = Framebuffer::Create(fbSpec);

		m_ActiveScene = CreateRef<Scene>();

#if 0
		auto square = m_ActiveScene->CreateEntity("Square Entity");
		square.AddComponent<SpriteRendererComponent>(m_CheckerboardTexture, glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f });

		auto square2 = m_ActiveScene->CreateEntity("Square Entity 2");
		square2.AddComponent<SpriteRendererComponent>(glm::vec4{ 0.6f, 0.2f, 0.5f, 1.0f });
		square2.GetComponent<TransformComponent>().Translation.x = rand() % 10 - 5.0f;

		// m_SquareEntity = square;

		m_CameraEntity = m_ActiveScene->CreateEntity("Camera");
		m_CameraEntity.AddComponent<CameraComponent>();

		class CameraController : public ScriptableEntity
		{
		public:
			void OnCreate()
			{
				// auto& translation = GetComponent<TransformComponent>().Translation;
				// translation.x = rand() % 10  - 5.0f;
			}

			void OnDestroy()
			{

			}

			void OnUpdate(Timestep ts)
			{
				auto& translation = GetComponent<TransformComponent>().Translation;
				float speed = 5.0f;

				if (Input::IsKeyPressed(TURT_KEY_A))
					translation.x -= speed * ts;
				if (Input::IsKeyPressed(TURT_KEY_D))
					translation.x += speed * ts;
				if (Input::IsKeyPressed(TURT_KEY_W))
					translation.y += speed * ts;
				if (Input::IsKeyPressed(TURT_KEY_S))
					translation.y -= speed * ts;
			}
		};

		m_CameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();
#endif
		constexpr char* LUA_SCRIPT = R"(
			-- this is a lua script
			Global.HelloWorld()
			Global.HelloWorld2()
			local c= Global.Add(10, 2)
			Global.HelloWorld3( c, 42)
			local spr = Sprite.new()
			spr:Draw()
			local d = spr:Move(2, 2)
			spr:Draw()
			-- move(4, 4)
			spr:Move(d, d)
			spr:Draw()
			local x = spr.x
			spr.x = 10
			-- move 12, 12
			spr:Move(x, x)
			spr:Draw()
			spr.img = 42
			local im = spr.img	
			print(im)

			local Transfrom = TransformComponent.new()


			function Foo(x, y)
				Global.HelloWorld3( x, y)
			end

			function Bar()
				print("Im The Bar")
			end

			function Render()
				spri = Global.GetSprite()
				spri.x = 420

			end
		)";

		entt::meta<GlobalMetaFunctionsComponent>().type("Global"_hs)
			.func<&HelloWorld>("HelloWorld"_hs).prop("Name"_hs, "HelloWorld")
			.func<&HelloWorld2>("HelloWorld2"_hs).prop("Name"_hs, "HelloWorld2")
			.func<&HelloWorld3>("HelloWorld3"_hs).prop("Name"_hs, "HelloWorld3")
			.func<&Add>("Add"_hs).prop("Name"_hs, "Add");


		entt::meta<Sprite>().type("Sprite"_hs)
			.prop("Name"_hs, "Sprite")
			.ctor<>()
			.func<&Sprite::Move>("Move"_hs).prop("Name"_hs, "Move")
			.func<&Sprite::Draw>("Draw"_hs).prop("Name"_hs, "Draw")
			.func<&GetSprite, entt::as_ref_t>("GetSprite"_hs).prop("Name"_hs, "GetSprite")
			.data<&Sprite::x>("x"_hs)
			.data<&Sprite::y>("y"_hs);

		LuaScript script;
		script.LoadScript(LUA_SCRIPT);
		script.ExecuteScript();

		int x = 1;
		int y = 2;

		script.CallScriptFunction("Foo", x, y);
		script.CallScriptFunction("Bar");
		Sprite sprite;
		script.CallScriptFunction("Render");
		script.CallScriptFunction("Render");
		sprite.Draw();
		sprite.x += 10;
		script.CallScriptFunction("Render");
		Instance.Draw();

		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
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
	    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
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
	    if (opt_flags & ImGuiDockNodeFlags_PassthruCentralNode)
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
				//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);1
				if (ImGui::MenuItem("New", "Ctrl+N"))
					NewScene();
				if(ImGui::MenuItem("Open...", "Ctrl+O"))
					OpenSceneFile();
				if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
					SaveSceneFileAs();
				

				if (ImGui::MenuItem("Exit")) Application::Get().Close();
				ImGui::EndMenu();
			}

	        ImGui::EndMenuBar();
	    }

	    m_SceneHierarchyPanel.OnImGuiRender();

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

		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<KeyPressedEvent>(TURT_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& event)
	{
		// Shortcuts
		if (event.GetRepeatCount() > 0)
			return false;

		bool control = Input::IsKeyPressed(TURT_KEY_LEFT_CONTROL) || Input::IsKeyPressed(TURT_KEY_RIGHT_CONTROL);
		bool shift = Input::IsKeyPressed(TURT_KEY_LEFT_SHIFT) || Input::IsKeyPressed(TURT_KEY_RIGHT_SHIFT);
		switch (event.GetKeyCode())
		{
			case TURT_KEY_N:
			{
				if (control)
					NewScene();

				break;
			}
			case TURT_KEY_O:
			{
				if (control)
					OpenSceneFile();

				break;
			}
			case TURT_KEY_S:
			{
				if (control && shift)
					SaveSceneFileAs();

				break;
			}
		}
	}

	void EditorLayer::OpenSceneFile()
	{
		std::string filepath = FileDialogs::OpenFile("Turtle Scene (*.turt)\0*.turt\0");
		if (!filepath.empty())
		{
			m_ActiveScene = CreateRef<Scene>();
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_SceneHierarchyPanel.SetContext(m_ActiveScene);
			
			m_ActiveScene->DeserializeScene(filepath);
		}
	}

	void EditorLayer::SaveSceneFileAs()
	{
		std::string filepath = FileDialogs::OpenFile("");
		if (!filepath.empty())
		{
			m_ActiveScene->SerializeScene(filepath);
		}
	}

	void EditorLayer::NewScene()
	{
		m_ActiveScene = CreateRef<Scene>();
		m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}

}