#include "repch.h"
#include "OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace RealEngine {
	OrthographicCamera::OrthographicCamera(float aspectRatio, float zoom = 45.0f)
		: m_ProjectionMatrix(glm::perspective(zoom, aspectRatio, 0.1f, 100.0f)), m_ViewMatrix(1.0f) {
		RE_PROFILE_FUNCTION();
		
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::SetProjection(float aspectRatio, float zoom) {
		RE_PROFILE_FUNCTION();
		
		m_ProjectionMatrix = glm::perspective(zoom, aspectRatio, 0.1f, 100.0f);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::RecalculateViewMatrix() {
		RE_PROFILE_FUNCTION();
		
		m_ViewMatrix = glm::lookAt(m_Position, m_Position + Front, Up);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}


}
