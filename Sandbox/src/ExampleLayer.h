#pragma once

#include "RealEngine.h"

class ExampleLayer : public RealEngine::Layer {
public:
	ExampleLayer();
	virtual ~ExampleLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(RealEngine::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(RealEngine::Event& e) override;
private:
	RealEngine::ShaderLibrary m_ShaderLibrary;
	RealEngine::Ref<RealEngine::Shader> m_Shader;
	RealEngine::Ref<RealEngine::VertexArray> m_VertexArray;

	RealEngine::Ref<RealEngine::Shader> m_FlatColorShader;
	RealEngine::Ref<RealEngine::VertexArray> m_SquareVA;

	RealEngine::Ref<RealEngine::Texture2D> m_Texture, m_ChernoLogoTexture;

	RealEngine::OrthographicCameraController m_CameraController;
	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
};

