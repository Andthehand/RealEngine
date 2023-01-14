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
	RealEngine::Ref<RealEngine::Shader> m_ChunkShader;
	RealEngine::EditorCamera m_EditorCamera;
	ChunkManager m_ChunkManager;
};
