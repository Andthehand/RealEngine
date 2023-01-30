#pragma once
#include <RealEngine.h>

#include "Voxels/ChunkManager.h"
#include "PerlinTexture.h"
#include "Constants.h"

#include "Test.h"

class MainLayer : public RealEngine::Layer {
public:
	MainLayer();
	virtual void OnUpdate(RealEngine::Timestep ts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(RealEngine::Event& event) override;

	bool OnKeyPressed(RealEngine::KeyPressedEvent& e);
private:
	inline void FPSCounter(RealEngine::Timestep ts) {
		deltaTime += ts;
		deltaTime /= 2.0f;
		fps = 1.0f / deltaTime;
	}

	inline void TestFunction() {
		m_TestShader->Bind();
		m_TestShader->SetMat4("u_ViewProjection", m_EditorCamera.GetViewProjection());
		m_Test.Render();
	}

private:
	RealEngine::Ref<RealEngine::Shader> m_ChunkShader, m_TestShader;
	RealEngine::EditorCamera m_EditorCamera;
	ChunkManager m_ChunkManager;

	Test m_Test;

	int m_Width = 500;
	int m_Height = 500;
	float m_Precition = Constants::NOISE_STEP;
	PerlinTexture m_Texture;

	bool m_Wireframe = false;

	//FPS counter
	float deltaTime = 0.0f;
	float fps = 0.0f;
};
