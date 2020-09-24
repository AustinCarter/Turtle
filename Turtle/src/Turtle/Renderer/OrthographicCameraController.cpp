#include "turtpch.h"
#include "OrthographicCameraController.h"

#include "Turtle/Core/Input.h"
#include "Turtle/Core/KeyCodes.h"

namespace Turtle {
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		: m_AspectRatio(aspectRatio),
		m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel)
		, m_RotationEnabled(rotation)
	{

	} 

	void OrthographicCameraController::OnUpdate(Timestep ts)
	{
		TURT_PROFILE_FUNCTION();
		
		if(Input::IsKeyPressed(TURT_KEY_A))
			m_CameraPosition.x -= m_CameraTranslationSpeed * ts.GetSeconds();
		else if(Input::IsKeyPressed(TURT_KEY_D))
			m_CameraPosition.x += m_CameraTranslationSpeed * ts.GetSeconds();
		if(Input::IsKeyPressed(TURT_KEY_W))
			m_CameraPosition.y += m_CameraTranslationSpeed * ts.GetSeconds();
		else if(Input::IsKeyPressed(TURT_KEY_S))
			m_CameraPosition.y -= m_CameraTranslationSpeed * ts.GetSeconds();

		if(m_RotationEnabled)
		{
			if(Input::IsKeyPressed(TURT_KEY_Q))
				m_CameraRotation += m_CameraRotationSpeed * ts.GetSeconds();
			else if(Input::IsKeyPressed(TURT_KEY_E))
				m_CameraRotation -= m_CameraRotationSpeed * ts.GetSeconds();

			m_Camera.SetRotation(m_CameraRotation);
		}

		m_Camera.SetPosition(m_CameraPosition);

		m_CameraTranslationSpeed = m_ZoomLevel;
	}

	void OrthographicCameraController::OnEvent(Event& e)
	{
		TURT_PROFILE_FUNCTION();
		
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(TURT_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(TURT_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		TURT_PROFILE_FUNCTION();
		
		m_ZoomLevel -= e.GetYOffset() * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		TURT_PROFILE_FUNCTION();
		
		/****** Scale what is on screen on resize******/
		OnResize((float)e.GetWidth(), (float)e.GetHeight());
		return false;


		/****** Show more/less in view port on resize ******/
		/*float yScale = e.GetHeight() / 720.0f;
		m_AspectRatio = yScale * (float)e.GetWidth() / e.GetHeight();
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -yScale * m_ZoomLevel, yScale * m_ZoomLevel);
		return false;*/
	}

	void OrthographicCameraController::OnResize(float width, float height)
	{
		TURT_PROFILE_FUNCTION();

		m_AspectRatio = width / height;
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);

	}

	/*void OrthographicCameraController::CalculateView()
	{
		m_Bounds = { -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel};
		m_Camera.SetProjection(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top);
	}*/
}