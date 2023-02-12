#pragma once
#include <RealEngine.h>

#include "Voxels/ChunkManager.h"
#include "FirstPersonCamera.h"
#include "PerlinTexture.h"
#include "Constants.h"

#include "Test.h"

//#define TESTING

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

private:
	RealEngine::Ref<RealEngine::Shader> m_ChunkShader;
	FirstPersonCamera m_Camera;
	ChunkManager m_ChunkManager;

	#ifdef TESTING
		Test m_Test;
		RealEngine::Ref<RealEngine::Shader> m_TestShader;
	#endif

	//This is all for PerlinTexture
	int m_Width = 500;
	int m_Height = 500;
	float m_Precition = Constants::FREQUENCY;
	PerlinTexture m_Texture;

	bool m_Wireframe = false;
	bool m_CursorEnabled = false;

	//FPS counter
	float deltaTime = 0.0f;
	float fps = 0.0f;
};
