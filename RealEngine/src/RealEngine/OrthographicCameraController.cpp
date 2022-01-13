#include "repch.h"
#include "OrthographicCameraController.h"

#include "RealEngine/Input.h"
#include "RealEngine/KeyCodes.h"

namespace RealEngine {
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation) 
		: m_AspectRatio(aspectRatio), m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel), m_Rotation(rotation) {
	}
	
	void OrthographicCameraController::OnUpdate(Timestep ts) {
		if (RealEngine::Input::IsKeyPressed(RE_KEY_W)) {
			m_CameraPosition.y += m_CameraMoveSpeed * ts;
		}
		if (RealEngine::Input::IsKeyPressed(RE_KEY_A)) {
			m_CameraPosition.x -= m_CameraMoveSpeed * ts;
		}
		if (RealEngine::Input::IsKeyPressed(RE_KEY_S)) {
			m_CameraPosition.y -= m_CameraMoveSpeed * ts;
		}
		if (RealEngine::Input::IsKeyPressed(RE_KEY_D)) {
			m_CameraPosition.x += m_CameraMoveSpeed * ts;
		}

		if (m_Rotation) {
			if (RealEngine::Input::IsKeyPressed(RE_KEY_Q))
				m_CameraRotation += m_CameraRotationSpeed * ts;
			if(RealEngine::Input::IsKeyPressed(RE_KEY_E))
				m_CameraRotation -= m_CameraRotationSpeed * ts;

			m_Camera.SetRotation(m_CameraRotation);
		}

		m_Camera.SetPosition(m_CameraPosition);
	}
	
	void OrthographicCameraController::OnEvent(Event& e) {
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(RE_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(RE_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}
	
	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e) {
		
		m_ZoomLevel -= e.GetYOffset() * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}
	
	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e) {
		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}
}