#pragma once
#include <RealEngine.h>

using namespace RealEngine;

class GameLayer : public Layer{
public:
	GameLayer();
	~GameLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnImGuiRender() override;
	virtual void OnUpdate(Timestep ts) override;
	virtual void OnEvent(Event& event) override;

private:
	OrthographicCameraController m_CameraController;

	float height = 10, width = 10;

	Ref<Shader> m_SquareShader;
	Ref<VertexArray> m_SquareVA;

	Ref<Texture2D> m_Snake, m_Body, m_Empty;

	//Temp
	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
};