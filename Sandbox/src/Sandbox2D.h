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

	RealEngine::Ref<RealEngine::Texture2D> m_Texture;
	RealEngine::Ref<RealEngine::Texture2D> m_SpriteSheet;
	RealEngine::Ref<RealEngine::SubTexture2D> m_GrassTexture;
	RealEngine::Ref<RealEngine::Framebuffer> m_Framebuffer;

	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };

	//FPS counter
	float deltaTime = 0.0f;
	float fps = 0.0f;
};