#include "MainLayer.h"

MainLayer::MainLayer() : m_EditorCamera(90.0f, RealEngine::Application::Get().GetWindow().GetWidth() / RealEngine::Application::Get().GetWindow().GetHeight(), 0.1f, 1000.0f) {
	m_ChunkShader = RealEngine::Shader::Create("assets/shaders/Chunk.glsl");
	
	RealEngine::RenderCommand::SetWireFrame(true);
}

void MainLayer::OnUpdate(RealEngine::Timestep ts) {
	RealEngine::RenderCommand::SetClearColor({ 0.1, 0.1, 0.1, 1 });
	RealEngine::RenderCommand::Clear();

	m_ChunkShader->Bind();
	m_EditorCamera.OnUpdate(ts);
	m_ChunkShader->SetMat4("u_ViewProjection", m_EditorCamera.GetViewProjection());
	
	m_ChunkManager.Render();
}

void MainLayer::OnEvent(RealEngine::Event& event) {
	m_EditorCamera.OnEvent(event);
}
