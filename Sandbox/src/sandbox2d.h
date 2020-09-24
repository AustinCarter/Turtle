#pragma once

#include <Turtle.h>

class Sandbox2D : public Turtle::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;
	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(Turtle::Timestep ts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Turtle::Event& event) override;
private:
	Turtle::Ref<Turtle::VertexArray> m_SquareVA;
	Turtle::OrthographicCameraController m_CameraController;
	Turtle::Ref<Turtle::Shader> m_FlatColorShader;

	Turtle::Ref<Turtle::Texture2D> m_CheckerboardTexture;
	Turtle::Ref<Turtle::Texture2D> m_SpriteSheet;
	Turtle::Ref<Turtle::SubTexture2D> m_StairSprite;
	Turtle::Ref<Turtle::SubTexture2D> m_BarrelSprite;
	Turtle::Ref<Turtle::SubTexture2D> m_TreeSprite;

	glm::vec4 m_SquareColor = {1.0f, 1.0f, 1.0f, 1.0f};
};