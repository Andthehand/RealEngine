#pragma once
#include <RealEngine.h>

#include "Constants.h"

class FirstPersonCamera : public RealEngine::Camera {
public:
	// constructor with vectors
	FirstPersonCamera(glm::vec3 position, float aspectRatio, float fov)
		: m_Position(position), m_FOV(fov), m_AspectRatio(aspectRatio) {
		m_MousePos = RealEngine::Input::GetMousePosition();

		UpdateProjectionMatrix();
		UpdateCameraVectors();
	}

	void OnUpdate(RealEngine::Timestep ts) {
		float velocity = Constants::CAMERA_SPEED * ts;

		if(RealEngine::Input::IsKeyPressed(RealEngine::Key::W))
			m_Position += m_WorldFront * velocity;
		if (RealEngine::Input::IsKeyPressed(RealEngine::Key::S))
			m_Position -= m_WorldFront * velocity;

		if (RealEngine::Input::IsKeyPressed(RealEngine::Key::A))
			m_Position -= m_Right * velocity;
		if (RealEngine::Input::IsKeyPressed(RealEngine::Key::D))
			m_Position += m_Right * velocity;

		if (RealEngine::Input::IsKeyPressed(RealEngine::Key::Space))
			m_Position += s_WorldUp * velocity;
		if (RealEngine::Input::IsKeyPressed(RealEngine::Key::LeftShift) || RealEngine::Input::IsKeyPressed(RealEngine::Key::RightShift))
			m_Position -= s_WorldUp * velocity;

		UpdateViewMatrix();
	}

	void OnEvent(RealEngine::Event& e) {
		RealEngine::EventDispatcher dispatcher(e);
		dispatcher.Dispatch<RealEngine::MouseMovedEvent>(RE_BIND_EVENT_FN(FirstPersonCamera::OnMouseMoved));
		dispatcher.Dispatch<RealEngine::WindowResizeEvent>(RE_BIND_EVENT_FN(FirstPersonCamera::OnWindowResize));
	}

	glm::mat4 GetViewProjection() const { return m_Projection * m_ViewMatrix; }
	glm::vec3 GetForwardDirection() const { return m_Front; }

	bool OnMouseMoved(RealEngine::MouseMovedEvent& e) {
		float xoffset = (e.GetX() - m_MousePos.x) * Constants::MOUSE_SENSITIVITY;
		float yoffset = (m_MousePos.y - e.GetY()) * Constants::MOUSE_SENSITIVITY;

		m_MousePos = { e.GetX(), e.GetY() };

		//Constrain yaw to 360
		m_Yaw = glm::mod(m_Yaw + xoffset, 360.0f);
		m_Pitch += yoffset;

		// make sure that when pitch is out of bounds, screen doesn't get flipped
		if (m_Pitch > 89.0f)
			m_Pitch = 89.0f;
		if (m_Pitch < -89.0f)
			m_Pitch = -89.0f;

		// update m_Front, m_Right and m_Up Vectors using the updated Euler angles
		UpdateCameraVectors();

		return false;
	}

	bool OnWindowResize(RealEngine::WindowResizeEvent& e) {
		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		UpdateProjectionMatrix();
		return false;
	}

	const glm::vec3& GetPosition() const { return m_Position; }
	void GetPosition(const glm::vec3& pos) {
		m_Position = pos;
		UpdateViewMatrix();
	}

	void SetFOV(float fov) {
		m_FOV = fov;
		UpdateProjectionMatrix();
	}
private:
	// calculates the front vector from the Camera's (updated) Euler Angles
	void UpdateCameraVectors() {
		// calculate the new m_Front vector
		float a = cos(glm::radians(m_Yaw + 90.0f)) / 5.0f;
		float b = sin(glm::radians(m_Yaw + 90.0f)) / 5.0f;

		m_WorldFront.x = cos(glm::radians(m_Yaw));
		m_WorldFront.z = sin(glm::radians(m_Yaw));
		m_WorldFront = glm::normalize(m_WorldFront);

		glm::vec3 front;
		front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		front.y = sin(glm::radians(m_Pitch));
		front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		m_Front = glm::normalize(front);
		// also re-calculate the m_Right and m_Up vector
		m_Right = glm::normalize(glm::cross(m_Front, s_WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		m_Up = glm::normalize(glm::cross(m_Right, m_Front));

		UpdateViewMatrix();
	}

	void UpdateViewMatrix() {
		m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
	}

	void UpdateProjectionMatrix() {
		m_Projection = glm::perspective(glm::radians(m_FOV), m_AspectRatio, Constants::NEAR_Clip, Constants::FAR_Clip);
	}
private:
	glm::mat4 m_ViewMatrix;

	// camera Attributes
	glm::vec3 m_Position = glm::vec3(0.0f, 0.0f, 0.0f);
	//m_WorldFront is used for moving
	glm::vec3 m_WorldFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 m_Front = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 m_Up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 m_Right;
	
	glm::vec2 m_MousePos;
	float m_Yaw = -90.0f;
	float m_Pitch = 0.0f;

	float m_FOV = 90.0f;
	float m_AspectRatio;

	inline static const glm::vec3 s_WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
};
