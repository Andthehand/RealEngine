#pragma once
#include "RealEngine.h"

class Sandbox2D : public RealEngine::Layer{
public:
	Sandbox2D();
	~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnImGuiRender() override;
	virtual void OnUpdate(RealEngine::Timestep ts) override;
	virtual void OnEvent(RealEngine::Event& event) override;
private:
	RealEngine::OrthographicCameraController m_CameraController;

	//Temp
	RealEngine::Ref<RealEngine::VertexArray> m_VertexArray;
	RealEngine::Ref<RealEngine::Shader> m_SquareShader;

	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
};