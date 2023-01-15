#pragma once
#include <RealEngine.h>

#include "Voxels/ChunkManager.h"

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
	RealEngine::EditorCamera m_EditorCamera;
	ChunkManager m_ChunkManager;

	//FPS counter
	float deltaTime = 0.0f;
	float fps = 0.0f;
};
