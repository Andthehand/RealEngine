#pragma once
#include "RealEngine/Renderer/Camera.h"

namespace RealEngine {
	class SceneCamera : public Camera {
	public:
		enum class ProjectionType { Perspective = 0, Orthographic = 1 };
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		void SetOrthographic(float size, float nearClip, float farClip);
		void SetPerspective(float verticalFOV, float nearClip, float farClip);
	
		void SetViewportSize(uint32_t width, uint32_t height);

		float GetPerspectiveVerticalFov() const { return m_PerspectiveFOV; }
		void SetPerspectiveVerticalFov(float verticalFOV) { m_PerspectiveFOV = verticalFOV; RecalculateProjection(); }
		float GetPerspectiveNearClip() const { return m_PerspectiveNear; }
		void SetPerspectiveNearClip(float nearClip) { m_PerspectiveNear = nearClip; RecalculateProjection(); }
		float GetPerspectiveFarClip() const { return m_PerspectiveFar; }
		void SetPerspectiveFarClip(float farClip) { m_PerspectiveFar = farClip; RecalculateProjection(); }

		float GetOrthographicSize() const { return m_OrthgraphicSize; }
		void SetOrthographicSize(float size) { m_OrthgraphicSize = size; RecalculateProjection(); }
		float GetOrthgraphicNearClip() const { return m_OrthgraphicNear; }
		void SetOrthgraphicNearClip(float nearClip) { m_OrthgraphicNear = nearClip; RecalculateProjection(); }
		float GetOrthgraphicFarClip() const { return m_OrthgraphicFar; }
		void SetOrthgraphicFarClip(float farClip) { m_OrthgraphicFar = farClip; RecalculateProjection(); }

		ProjectionType GetProjectionType() const { return m_ProjectionType; }
		void SetProjectionType(ProjectionType type) { m_ProjectionType = type; RecalculateProjection(); }
	private:
		void RecalculateProjection();
	private:
		ProjectionType m_ProjectionType = ProjectionType::Orthographic;
		
		float m_PerspectiveFOV = glm::radians(45.0f);;
		float m_PerspectiveNear = 0.01f, m_PerspectiveFar = 1000.0f;
		
		float m_OrthgraphicSize = 10.0f;
		float m_OrthgraphicNear = -1.0f, m_OrthgraphicFar = 1.0f;
	
		float m_AspectRatio = 0.0f;
	};
}