#pragma once
#include <RealEngine.h>

#include "Chunks/ChunkManager.h"
#include "FirstPersonCamera.h"
#include "Utils/PerlinTexture.h"
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
		m_DeltaTime += ts;
		m_DeltaTime /= 2.0f;
		m_FPS = 1.0f / m_DeltaTime;
	}

	inline void FrameTimeCounter(RealEngine::Timestep ts) {
		m_FrameTime = ts.GetMilliseconds();
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

	bool m_VSync = true;
	bool m_Wireframe = false;
	bool m_CursorEnabled = false;

	//FPS counter
	float m_DeltaTime = 0.0f;
	float m_FPS = 0.0f;

	//Frame Time Counter
	float m_FrameTime = 0.0f;
};
