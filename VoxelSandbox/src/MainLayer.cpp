#include "MainLayer.h"

#include <imgui/imgui.h>

MainLayer::MainLayer() : m_EditorCamera(70.0f, (float)(RealEngine::Application::Get().GetWindow().GetWidth() / RealEngine::Application::Get().GetWindow().GetHeight()), 0.1f, 1000.0f),
						m_ChunkManager(m_EditorCamera.GetPosition()), m_Texture(m_Width, m_Height){
	m_ChunkShader = RealEngine::Shader::Create("assets/shaders/Chunk.glsl");

#ifdef TESTING
	m_TestShader = RealEngine::Shader::Create("assets/shaders/Test.glsl");
#endif

	//RealEngine::Application::Get().GetWindow().SetVSync(false);
}

void MainLayer::OnUpdate(RealEngine::Timestep ts) {
	FPSCounter(ts);

	RealEngine::RenderCommand::SetClearColor({ 0.1, 0.1, 0.1, 1 });
	RealEngine::RenderCommand::Clear();

	m_EditorCamera.OnUpdate(ts);
	m_ChunkShader->Bind();
	m_ChunkShader->SetMat4("u_ViewProjection", m_EditorCamera.GetViewProjection());
	
#ifndef TESTING
	m_ChunkManager.Render(m_EditorCamera);
#endif


#ifdef TESTING
	m_TestShader->Bind();
	m_TestShader->SetMat4("u_ViewProjection", m_EditorCamera.GetViewProjection());
	m_Test.Render(m_TestShader);
#endif
}

void MainLayer::OnImGuiRender() {
	ImGui::Begin("Test Variables");

	ImGui::Text("FPS: %f", fps);

	ImGui::Text("Camera Facing: %f, %f, %f", m_EditorCamera.GetForwardDirection().x, m_EditorCamera.GetForwardDirection().y, m_EditorCamera.GetForwardDirection().z);
	ImGui::Text("Camera Pos: %f, %f, %f", m_EditorCamera.GetPosition().x, m_EditorCamera.GetPosition().y, m_EditorCamera.GetPosition().z);
	m_ChunkManager.OnImGuiRender();
	if(ImGui::Button("Wireframe")) { 
		m_Wireframe = !m_Wireframe;
		RealEngine::RenderCommand::SetWireFrame(m_Wireframe); 
	}
	ImGui::End();

	ImGui::Begin("Perlin Noise");
	if (ImGui::InputInt("Width", &m_Width)) { m_Texture.Regenerate((uint32_t)m_Width, (uint32_t)m_Height, m_Precition); }
	ImGui::SameLine();
	if (ImGui::InputInt("Height", &m_Height)) { m_Texture.Regenerate((uint32_t)m_Width, (uint32_t)m_Height, m_Precition); }
	if (ImGui::InputFloat("Precition", &m_Precition)) { m_Texture.Regenerate(m_Width, m_Height, m_Precition); }
	ImGui::Image((void*)(intptr_t)m_Texture.GetTexture()->GetRendererID(), ImVec2((float)m_Width, (float)m_Height));

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
