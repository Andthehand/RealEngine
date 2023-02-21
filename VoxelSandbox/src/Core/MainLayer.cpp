#include "MainLayer.h"

#include <imgui/imgui.h>

MainLayer::MainLayer() 
	: m_Camera( Constants::CAMERA_POSITION, (float)RealEngine::Application::Get().GetWindow().GetWidth() / (float)RealEngine::Application::Get().GetWindow().GetHeight(), 70.0f),
		m_ChunkManager(m_Camera.GetPosition()), m_Texture(m_Width, m_Height){
	
	m_ChunkShader = RealEngine::Shader::Create("assets/shaders/Chunk.glsl");

#ifdef TESTING
	m_TestShader = RealEngine::Shader::Create("assets/shaders/Test.glsl");
#endif

	RealEngine::Application::Get().GetWindow().SetCursorEnabled(m_CursorEnabled);
}

void MainLayer::OnUpdate(RealEngine::Timestep ts) {
	FPSCounter(ts);
	FrameTimeCounter(ts);

	RealEngine::RenderCommand::SetClearColor({ 0.1, 0.1, 0.1, 1 });
	RealEngine::RenderCommand::Clear();

	m_Camera.OnUpdate(ts);
	m_ChunkShader->Bind();
	m_ChunkShader->SetMat4("u_ViewProjection", m_Camera.GetViewProjection());
	
#ifndef TESTING
	m_ChunkManager.Render(m_Camera);
#endif


#ifdef TESTING
	m_TestShader->Bind();
	m_TestShader->SetMat4("u_ViewProjection", m_EditorCamera.GetViewProjection());
	m_Test.Render(m_TestShader);
#endif
}

void MainLayer::OnImGuiRender() {
	ImGui::Begin("General Stats");

	ImGui::Text("FPS: %.0f", m_FPS);
	ImGui::Text("Frame Time: %.2f", m_FrameTime);

	ImGui::Text("Camera Facing: %f, %f, %f", m_Camera.GetForwardDirection().x, m_Camera.GetForwardDirection().y, m_Camera.GetForwardDirection().z);
	ImGui::Text("Camera Pos: %f, %f, %f", m_Camera.GetPosition().x, m_Camera.GetPosition().y, m_Camera.GetPosition().z);
	if (ImGui::Button("VSync")) {
		m_VSync = !m_VSync;
		RealEngine::Application::Get().GetWindow().SetVSync(m_VSync);
	}
	if(ImGui::Button("Wireframe")) { 
		m_Wireframe = !m_Wireframe;
		RealEngine::RenderCommand::SetWireFrame(m_Wireframe); 
	}
	ImGui::InputFloat("Camera Speed", &Constants::CAMERA_SPEED);
	ImGui::End();

	m_ChunkManager.OnImGuiRender();

	ImGui::Begin("Perlin Noise");
	if (ImGui::InputInt("Width", &m_Width)) { m_Texture.Regenerate((uint32_t)m_Width, (uint32_t)m_Height, m_Precition); }
	ImGui::SameLine();
	if (ImGui::InputInt("Height", &m_Height)) { m_Texture.Regenerate((uint32_t)m_Width, (uint32_t)m_Height, m_Precition); }
	if (ImGui::InputFloat("Precition", &m_Precition)) { m_Texture.Regenerate(m_Width, m_Height, m_Precition); }
	ImGui::Image((void*)(intptr_t)m_Texture.GetTexture()->GetRendererID(), ImVec2((float)m_Width, (float)m_Height));

	ImGui::End();
}

void MainLayer::OnEvent(RealEngine::Event& event) {
	m_Camera.OnEvent(event);

	RealEngine::EventDispatcher dispatcher(event);
	dispatcher.Dispatch<RealEngine::KeyPressedEvent>(RE_BIND_EVENT_FN(MainLayer::OnKeyPressed));
}

bool MainLayer::OnKeyPressed(RealEngine::KeyPressedEvent& e) {
	if (e.GetKeyCode() == RealEngine::Key::Escape && !e.GetRepeatCount()) {
		m_CursorEnabled = !m_CursorEnabled;
		RealEngine::Application::Get().GetWindow().SetCursorEnabled(m_CursorEnabled);
	}
	return false;
}
