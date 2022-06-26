#pragma once
#include "RealEngine/Renderer/Camera.h"

namespace RealEngine {
	class SceneCamera : public Camera {
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		void SetOrthographic(float size, float nearClip, float farClip);
	
		void SetViewportSize(uint32_t width, uint32_t height);

		float GetOrthographicSize() const { return m_OrthgraphicSize; }
		float SetOrthographicSize(float size) { m_OrthgraphicSize = size; RecalculateProjection(); }
	private:
		void RecalculateProjection();
	private:
		float m_OrthgraphicSize = 10.0f;
		float m_OrthgraphicNear = -1.0f, m_OrthgraphicFar = 1.0f;
	
		float m_AspectRatio = 0.0f;
	};
}