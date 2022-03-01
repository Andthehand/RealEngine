#include "Sandbox2D.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui/imgui.h"

#include "Platform/OpenGL/OpenGLShader.h"

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f, true) { }

void Sandbox2D::OnAttach() {
	m_Texture = RealEngine::Texture2D::Create("assets/textures/Checkerboard.png");
}

void Sandbox2D::OnDetach() {

}

void Sandbox2D::OnImGuiRender() {
	RE_PROFILE_FUNCTION();

	ImGui::Begin("Settings");
	ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));

	ImGui::End();
}

void Sandbox2D::OnUpdate(RealEngine::Timestep ts) {
	RE_PROFILE_FUNCTION();

	//Update
	{
		RE_PROFILE_SCOPE("Camera Update");
		m_CameraController.OnUpdate(ts);
	}
	
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
				RealEngine::Renderer2D::DrawQuad(position, { 1.0f, 1.0f }, m_Texture);
			}
		}

		RealEngine::Renderer2D::DrawQuad({ 0.5, 0.5, 1 }, { 0.5f, 0.5f }, m_SquareColor);
		RealEngine::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnEvent(RealEngine::Event& e) {
	m_CameraController.OnEvent(e);
}