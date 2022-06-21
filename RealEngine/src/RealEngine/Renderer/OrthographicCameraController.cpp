#include "repch.h"
#include "OrthographicCameraController.h"

#include "RealEngine/Core/Input.h"
#include "RealEngine/Core/KeyCodes.h"

namespace RealEngine {
	OrthographicCameraController::OrthographicCameraController(float aspectRatio) 
		: m_AspectRatio(aspectRatio), m_Camera(m_AspectRatio, m_ZoomLevel){
		m_Camera.UpdateCameraVectors(m_Yaw, m_Pitch);
	}
	
	void OrthographicCameraController::OnUpdate(Timestep ts) {
		RE_PROFILE_FUNCTION();

		float velocity = m_CameraMoveSpeed * ts;
		if (Input::IsKeyPressed(RE_KEY_A)) {
			m_CameraPosition -= m_Camera.GetRightVector() * velocity;
		}
		else if (Input::IsKeyPressed(RE_KEY_D)) {
			m_CameraPosition += m_Camera.GetRightVector() * velocity;
		}

		if (Input::IsKeyPressed(RE_KEY_W)) {
			m_CameraPosition += m_Camera.GetFrontVector() * velocity;
		}
		else if (Input::IsKeyPressed(RE_KEY_S)) {
			m_CameraPosition -= m_Camera.GetFrontVector() * velocity;
		}

		if (Input::IsKeyPressed(RE_KEY_SPACE)) {
			m_CameraPosition += m_Camera.GetWorldUpVector() * velocity;
		}
		else if (Input::IsKeyPressed(RE_KEY_LEFT_SHIFT) || Input::IsKeyPressed(RE_KEY_RIGHT_SHIFT)) {
			m_CameraPosition -= m_Camera.GetWorldUpVector() * velocity;
		}

		m_Camera.SetPosition(m_CameraPosition);
	}
	
	void OrthographicCameraController::OnEvent(Event& e) {
		RE_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(RE_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<MouseMovedEvent>(RE_BIND_EVENT_FN(OrthographicCameraController::OnMouseMoved));
		dispatcher.Dispatch<WindowResizeEvent>(RE_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}

	void OrthographicCameraController::OnResize(float width, float height) {
		m_AspectRatio = width / height;
		m_Camera.SetProjection(m_AspectRatio, m_ZoomLevel);
	}
	
	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e) {
		RE_PROFILE_FUNCTION();
		
		m_ZoomLevel -= e.GetYOffset() * 0.25f;

		if (m_ZoomLevel < 1.0f)
			m_ZoomLevel = 1.0f;
		if (m_ZoomLevel > 3.0f)
			m_ZoomLevel = 3.0f;

		m_Camera.SetProjection(m_AspectRatio, m_ZoomLevel);
		return false;
	}

	bool OrthographicCameraController::OnMouseMoved(MouseMovedEvent& e) {
		RE_PROFILE_FUNCTION();
		
		if (m_FirstMouseMove) {
			m_FirstMouseMove = false;
			m_MouseX = e.GetX();
			m_MouseY = e.GetY();
		}
		else {
			m_Yaw += (e.GetX() - m_MouseX) * m_MouseSensitivity;
			m_Pitch += (m_MouseY - e.GetY()) * m_MouseSensitivity;

			if (m_Pitch > 89.0f)
				m_Pitch = 89.0f;
			if (m_Pitch < -89.0f)
				m_Pitch = -89.0f;

			m_MouseX = e.GetX();
			m_MouseY = e.GetY();

			m_Camera.UpdateCameraVectors(m_Yaw, m_Pitch);
		}
		return false;
	}
	
	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e) {
		RE_PROFILE_FUNCTION();

		OnResize((float)e.GetWidth(), (float)e.GetHeight());
		return false;
	}
}