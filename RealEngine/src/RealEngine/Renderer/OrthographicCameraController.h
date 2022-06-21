#pragma once
#include "RealEngine/Renderer/OrthographicCamera.h"
#include "RealEngine/Core/TimeStep.h"

#include "RealEngine/Events/ApplicationEvent.h"
#include "RealEngine/Events/MouseEvent.h"

namespace RealEngine {
	class OrthographicCameraController {
	public:
		OrthographicCameraController(float aspectRatio);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		void OnResize(float width, float height);

		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }

		float GetZoomLevel() const { return m_ZoomLevel; }
		void SetZoomLevel(float level) { m_ZoomLevel = level; }
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnMouseMoved(MouseMovedEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);

		float m_AspectRatio;
		float m_ZoomLevel = 2.0f;

		bool m_FirstMouseMove = true;
		float m_MouseX, m_MouseY;

		float m_MouseSensitivity = 0.1f;
		float m_Yaw = -90.0f;
		float m_Pitch = 0.0f;

		OrthographicCamera m_Camera;

		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_CameraMoveSpeed = 5.0f;
	};
}