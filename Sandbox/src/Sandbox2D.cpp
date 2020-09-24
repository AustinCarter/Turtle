#include "Sandbox2D.h"
#include "imgui/imgui.h"

#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>

#include <chrono>

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f)
{
}

void Sandbox2D::OnAttach()
{
	TURT_PROFILE_FUNCTION();
		
	m_CheckerboardTexture = Turtle::Texture2D::Create("assets/textures/Checkerboard.png");
	m_SpriteSheet = Turtle::Texture2D::Create("assets/textures/RPGpack_sheet_2X.png");

	m_StairSprite = Turtle::SubTexture2D::CreateFromCoords(m_SpriteSheet, {7, 6}, {128, 128});
	m_BarrelSprite = Turtle::SubTexture2D::CreateFromCoords(m_SpriteSheet, {8, 2}, {128, 128});
	m_TreeSprite = Turtle::SubTexture2D::CreateFromCoords(m_SpriteSheet, {2, 1}, {128, 128}, {1, 2});
}

void Sandbox2D::OnDetach()
{
	TURT_PROFILE_FUNCTION();
}

void Sandbox2D::OnUpdate(Turtle::Timestep ts)
{
	TURT_PROFILE_FUNCTION();

	m_CameraController.OnUpdate(ts);
	

	Turtle::Renderer2D::ResetStats();
	// Render
	{
		TURT_PROFILE_SCOPE("Renderer Prep");
		Turtle::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 0.1f});
		Turtle::RenderCommand::Clear();
	}

	{
		static float rotation = 0.0f;
		rotation += 20.0f * ts;

		TURT_PROFILE_SCOPE("Render Draw Calls");
		Turtle::Renderer2D::BeginScene(m_CameraController.GetCamera());
		Turtle::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.3f, 0.2f, 1.0f });
		Turtle::Renderer2D::DrawRotatedQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, glm::radians(rotation), { 0.2f, 0.3f, 0.8f, 1.0f });
		Turtle::Renderer2D::DrawQuad({-0.0f, -0.0f, -0.1f}, {20.0f, 20.0f}, m_CheckerboardTexture, m_SquareColor);
		Turtle::Renderer2D::DrawRotatedQuad({-0.5f, -0.5f, 0.0f}, {1.0f, 1.0f}, glm::radians(rotation), m_CheckerboardTexture);	
		Turtle::Renderer2D::EndScene();

		Turtle::Renderer2D::BeginScene(m_CameraController.GetCamera());
		for(float y = -5.0f; y < 5.0f; y += 0.5f)
		{
			for (float x = -5.0f; x < 5.0f; x += 0.5f)
			{
				glm::vec4 color = {(x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.75f };
				Turtle::Renderer2D::DrawQuad({ x, y}, { 0.45f, 0.45f }, color);
			}
		}
		Turtle::Renderer2D::EndScene();
		// Turtle::Renderer2D::BeginScene(m_CameraController.GetCamera());

		// Turtle::Renderer2D::DrawQuad({0.0f, 0.0f}, {1.0f, 1.0f}, m_StairSprite);
		// Turtle::Renderer2D::DrawQuad({1.0f, 0.0f}, {1.0f, 1.0f}, m_BarrelSprite);
		// Turtle::Renderer2D::DrawQuad({-1.0f, 0.0f}, {1.0f, 2.0f}, m_TreeSprite);

		// Turtle::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiRender()
{
	TURT_PROFILE_FUNCTION();

	ImGui::Begin("Settings");

	auto stats = Turtle::Renderer2D::GetStats();
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("DrawCalls: %d", stats.DrawCalls);
	ImGui::Text("Quads: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();

}

void Sandbox2D::OnEvent(Turtle::Event& event)
{
	m_CameraController.OnEvent(event);
}