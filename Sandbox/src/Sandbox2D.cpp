#include "Sandbox2D.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui/imgui.h"

#include "Platform/OpenGL/OpenGLShader.h"

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f, true) { }

void Sandbox2D::OnAttach() {

}

void Sandbox2D::OnDetach() {

}

void Sandbox2D::OnImGuiRender() {
	ImGui::Begin("Settings");
	ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnUpdate(RealEngine::Timestep ts) {
	m_CameraController.OnUpdate(ts);

	RealEngine::RenderCommand::SetClearColor({ 0.1, 0.1, 0.1, 1 });
	RealEngine::RenderCommand::Clear();

	RealEngine::Renderer2D::BeginScene(m_CameraController.GetCamera());

	RealEngine::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, { 0.8f, 0.2f, 0.3f, 1.0f });
	RealEngine::Renderer2D::EndScene();
}

void Sandbox2D::OnEvent(RealEngine::Event& e) {
	m_CameraController.OnEvent(e);
}