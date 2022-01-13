#pragma once
#include "RealEngine/Renderer/OrthographicCamera.h"
#include "RealEngine/Core/TimeStep.h"

#include "RealEngine/Events/ApplicationEvent.h"
#include "RealEngine/Events/MouseEvent.h"

namespace RealEngine {
	class OrthographicCameraController {
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);

		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);

		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;

		OrthographicCamera m_Camera;

		bool m_Rotation;

		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_CameraRotation = 0.0f;
		float m_CameraMoveSpeed = 2.5f, m_CameraRotationSpeed = 180.0f;
	};
}