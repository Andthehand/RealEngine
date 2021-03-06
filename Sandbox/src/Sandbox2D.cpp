#include "Sandbox2D.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui/imgui.h"

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f, true) { }

void Sandbox2D::OnAttach() {
	RE_PROFILE_FUNCTION();

	m_Texture = RealEngine::Texture2D::Create("assets/textures/Checkerboard.png");
	m_SpriteSheet = RealEngine::Texture2D::Create("assets/textures/Spritesheet.png");
	m_GrassTexture = RealEngine::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 0, 2 }, { 16, 16 });
}

void Sandbox2D::OnDetach() {
	RE_PROFILE_FUNCTION();
}

void Sandbox2D::OnImGuiRender() {
    ImGui::Begin("Settings");

    auto stats = RealEngine::Renderer2D::GetStats();
    ImGui::Text("Renderer2D Stats:");
    ImGui::Text("DrawCalls: %d", stats.DrawCalls);
    ImGui::Text("QuadCount: %d", stats.QuadCount);
    ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
    ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

    ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
    ImGui::Text("FPS: %f", fps);

	ImGui::End();
}

void Sandbox2D::OnUpdate(RealEngine::Timestep ts) {
	RE_PROFILE_FUNCTION();

	deltaTime += ts;
	deltaTime /= 2.0f;
	fps = 1.0f / deltaTime;

	//Update
	{
		RE_PROFILE_SCOPE("Camera Update");
		m_CameraController.OnUpdate(ts);
	}
	
	RealEngine::Renderer2D::ResetStats();
	//Render
	{
		RE_PROFILE_SCOPE("Render Prep");
        RealEngine::RenderCommand::SetClearColor({ 0.1, 0.1, 0.1, 1 });
		RealEngine::RenderCommand::Clear();
	}

	//Draw
	{
		RE_PROFILE_SCOPE("Render Draw");
		RealEngine::Renderer2D::BeginScene(m_CameraController.GetCamera());
		for (int x = -5; x < 5; x++) {
			for (int y = -5; y < 5; y++) {
				glm::vec2 position = { x + (x * 0.1f), y + (y * 0.1f) };
				RealEngine::Renderer2D::DrawQuad({ position.x + (x * 0.1f), position.y + (y * 0.1f), 0.0f }, { 1.0f, 1.0f }, {m_SquareColor, 1.0f});
			}
		}

		RealEngine::Renderer2D::DrawQuad({ 0.5f, 0.5f, 1.0f }, { 0.5f, 0.5f }, m_Texture);
		RealEngine::Renderer2D::DrawRotatedQuad({ -0.5f, 1.0f, 1.0f }, { 0.5f, 0.5f }, 45.0f, m_Texture);

		RealEngine::Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.1f }, { 1.0f, 1.0f }, m_GrassTexture);

		RealEngine::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnEvent(RealEngine::Event& e) {
	m_CameraController.OnEvent(e);
}