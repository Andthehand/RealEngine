#include "MainLayer.h"

#include "imgui/imgui.h"

MainLayer::MainLayer() : m_EditorCamera(90.0f, (float)(RealEngine::Application::Get().GetWindow().GetWidth() / RealEngine::Application::Get().GetWindow().GetHeight()), 0.1f, 1000.0f),
						m_ChunkManager((glm::ivec3)m_EditorCamera.GetPosition()) {
	m_ChunkShader = RealEngine::Shader::Create("assets/shaders/Chunk.glsl");
	
	RealEngine::RenderCommand::SetWireFrame(true);
}

void MainLayer::OnUpdate(RealEngine::Timestep ts) {
	FPSCounter(ts);

	RealEngine::RenderCommand::SetClearColor({ 0.1, 0.1, 0.1, 1 });
	RealEngine::RenderCommand::Clear();

	m_ChunkShader->Bind();
	m_EditorCamera.OnUpdate(ts);
	m_ChunkShader->SetMat4("u_ViewProjection", m_EditorCamera.GetViewProjection());
	
	m_ChunkManager.Render(m_EditorCamera);
}

void MainLayer::OnImGuiRender() {
	ImGui::Begin("Test Variables");

	ImGui::Text("FPS: %f", fps);

	ImGui::Text("Camera Pos: %f, %f, %f", m_EditorCamera.GetPosition().x, m_EditorCamera.GetPosition().z, m_EditorCamera.GetPosition().y);
	m_ChunkManager.OnImGuiRender();

	ImGui::End();
}

void MainLayer::OnEvent(RealEngine::Event& event) {
	m_EditorCamera.OnEvent(event);

	RealEngine::EventDispatcher dispatcher(event);
	dispatcher.Dispatch<RealEngine::KeyPressedEvent>(RE_BIND_EVENT_FN(MainLayer::OnKeyPressed));
}

bool MainLayer::OnKeyPressed(RealEngine::KeyPressedEvent& e) {
	return false;
}
