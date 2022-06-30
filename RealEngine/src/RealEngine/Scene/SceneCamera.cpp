#include "repch.h"
#include "SceneCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace RealEngine {
	SceneCamera::SceneCamera() {
		RecalculateProjection();
	}

	void SceneCamera::SetOrthographic(float size, float nearClip, float farClip) {
		m_ProjectionType = ProjectionType::Orthographic;
		m_OrthgraphicSize = size;
		m_OrthgraphicNear = nearClip;
		m_OrthgraphicFar = farClip;

		RecalculateProjection();
	}

	void SceneCamera::SetPerspective(float verticalFOV, float nearClip, float farClip) {
		m_ProjectionType = ProjectionType::Perspective;
		m_PerspectiveFOV = verticalFOV;
		m_PerspectiveNear = nearClip;
		m_PerspectiveFar = farClip;

		RecalculateProjection();
	}

	void SceneCamera::SetViewportSize(uint32_t width, uint32_t height) {
		m_AspectRatio = (float)width / (float)height;
		
		RecalculateProjection();
	}

	void SceneCamera::RecalculateProjection() {
		if (m_ProjectionType == ProjectionType::Perspective) {
			m_Projection = glm::perspective(m_PerspectiveFOV, m_AspectRatio, m_PerspectiveNear, m_PerspectiveFar);
		}
		else {
			float orthLeft = -m_OrthgraphicSize * m_AspectRatio * 0.5f;
			float orthRight = m_OrthgraphicSize * m_AspectRatio * 0.5f;
			float orthBottom = -m_OrthgraphicSize * 0.5f;
			float orthTop = m_OrthgraphicSize * 0.5f;

			m_Projection = glm::ortho(orthLeft, orthRight, orthTop, orthBottom, m_OrthgraphicNear, m_OrthgraphicFar);
		}
	}
}