#pragma once
#include <RealEngine.h>

using namespace RealEngine;

class MainLayer : public Layer {
public:
	MainLayer(Application* app, int width, int height);

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(Timestep ts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Event& event) override;
private:
	bool OnKeyPressedEvent(KeyPressedEvent& e);

	inline void FPSCounter(Timestep ts) {
		deltaTime += ts;
		deltaTime /= 2.0f;
		fps = 1.0f / deltaTime;
	}
private:
	Application* m_App;

	RealEngine::Ref<RealEngine::Texture2D> m_SpriteSheet;
	RealEngine::Ref<RealEngine::SubTexture2D> m_GrassTexture;

	OrthographicCameraController m_CameraController;

	glm::vec3 m_Color = {1.0f, 0.0f, 0.0f};

	bool m_Wireframe = false;
	bool m_MouseEnabled = true;

	//FPS counter
	float deltaTime = 0.0f;
	float fps = 0.0f;
};