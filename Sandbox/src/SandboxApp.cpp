#include <Turtle.h>
#include <Turtle/Core/EntryPoint.h>

#include "imgui/imgui.h"

#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>


#include "Sandbox2D.h"


    
class MyLayer : public Turtle::Layer
{
public:
	MyLayer() 
		: Layer("Example"), m_CameraController(1280.0f / 720.0f)
	{
		m_VertexArray = Turtle::VertexArray::Create();


		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		float sqVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		Turtle::Ref<Turtle::VertexBuffer> vertexBuffer;
		vertexBuffer = Turtle::VertexBuffer::Create(vertices, sizeof(vertices));

	
		Turtle::BufferLayout layout = {
			{ Turtle::ShaderDataType::Float3, "a_Position" },
			{ Turtle::ShaderDataType::Float4, "a_Color" }
		};
	
		vertexBuffer->SetLayout(layout);	
		m_VertexArray->AddVertexBuffer(vertexBuffer);


		unsigned int indicies[3] = {0,1,2};
		Turtle::Ref<Turtle::IndexBuffer> indexBuffer;
		indexBuffer = Turtle::IndexBuffer::Create(indicies, sizeof(indicies) / sizeof(uint32_t));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_SquareVA = Turtle::VertexArray::Create();

		Turtle::Ref<Turtle::VertexBuffer> squareVB;
		squareVB = Turtle::VertexBuffer::Create(sqVertices, sizeof(sqVertices));

		Turtle::BufferLayout sqareVBLayout = {
			{ Turtle::ShaderDataType::Float3, "a_Position" },
			{ Turtle::ShaderDataType::Float2, "a_TexCoord" }
		};
	
		squareVB->SetLayout(sqareVBLayout);	
		m_SquareVA->AddVertexBuffer(squareVB);

		unsigned int sqIndicies[6] = {0,1,2,2,3,0};
		Turtle::Ref<Turtle::IndexBuffer> squareIB;
		squareIB = Turtle::IndexBuffer::Create(sqIndicies, sizeof(sqIndicies) / sizeof(uint32_t));
		m_SquareVA->SetIndexBuffer(squareIB);


		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;
			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}

		)";

		m_Shader = Turtle::Shader::Create("VertexPosColorTriangle", vertexSrc, fragmentSrc);

		std::string flatColorVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
			}
		)";

		std::string flatColorFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			uniform vec3 u_Color;

			void main()
			{
				color = vec4(u_Color, 1.0);
			}

		)";


		m_FlatColorShader = Turtle::Shader::Create("FlatColor", flatColorVertexSrc, flatColorFragmentSrc);

		auto textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");

		m_Texture = Turtle::Texture2D::Create("assets/textures/Checkerboard.png");
		m_logoTexture = Turtle::Texture2D::Create("assets/textures/ChernoLogo.png");

		textureShader->Bind();
		textureShader->SetInt("u_Texture", 0);
	}

	void OnUpdate(Turtle::Timestep ts) override
	{
		// Update
		m_CameraController.OnUpdate(ts);

		// Render
		Turtle::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 0.1f});
		Turtle::RenderCommand::Clear();

		Turtle::Renderer::BeginScene(m_CameraController.GetCamera());

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		m_FlatColorShader->Bind();
		m_FlatColorShader->SetFloat3("u_Color", m_SquareColor);


		for (int y = 0; y < 20; y++)
		{
			for (int x = 0; x < 20; x++)
			{
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Turtle::Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);
			}
		}

		auto textureShader = m_ShaderLibrary.Get("Texture");

		m_Texture->Bind();
		Turtle::Renderer::Submit(textureShader, m_SquareVA,  glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		m_logoTexture->Bind();
		Turtle::Renderer::Submit(textureShader, m_SquareVA,  glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		// Triangle 
		// Turtle::Renderer::Submit(m_Shader, m_VertexArray);

		Turtle::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override 
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	void OnEvent(Turtle::Event& event) override
	{
		m_CameraController.OnEvent(event);
	}
private:
	Turtle::ShaderLibrary m_ShaderLibrary;
	Turtle::Ref<Turtle::VertexArray> m_VertexArray;
	Turtle::Ref<Turtle::Shader> m_Shader;
	Turtle::Ref<Turtle::Shader> m_FlatColorShader, m_TextureShader;
	Turtle::Ref<Turtle::VertexArray> m_SquareVA;

	Turtle::Ref<Turtle::Texture2D> m_Texture, m_logoTexture;

	Turtle::OrthographicCameraController m_CameraController;
	float m_CameraMoveSpeed = 2.5f;
	float m_CameraRotation = 0.0f;
	float m_CameraRotationSpeed = 180.0f;

	glm::vec3 m_SquareColor = {0.2f, 0.3f, 0.8f};
};

class Sandbox : public Turtle::Application
{
public:
	Sandbox()
	{
		// PushLayer(new MyLayer());
		PushLayer(new Sandbox2D());
	}

	~Sandbox()
	{

	}

};

Turtle::Application* Turtle::CreateApplication()
{
	return new Sandbox();
}