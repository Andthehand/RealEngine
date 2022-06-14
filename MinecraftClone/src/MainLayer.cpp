#include "MainLayer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui/imgui.h"

MainLayer::MainLayer(Application* app, int width, int height)
	: Layer{ "Minecraft Main Layer" }, m_App(app), m_Camera(1280.0f / 720.0f) {
}

void MainLayer::OnAttach() {
	//m_App->GetWindow().SetInputMode(m_MouseEnabled);
}

void MainLayer::OnDetach() {
}

void MainLayer::OnUpdate(Timestep ts) {
	m_Camera.OnUpdate(ts);
	//m_ChunkManager.OnUpdate(m_Camera.GetPosition());

	FPSCounter(ts);

	RenderCommand::SetClearColor({ 0.1, 0.1, 0.1, 1 });
	RenderCommand::Clear();

	Renderer2D::ResetStats();
	Renderer2D::BeginScene(m_Camera.GetCamera());
	//Renderer2D::SetColor(m_Color);

	//m_ChunkManager.RenderAll(m_Camera);

	Renderer::EndScene();
}

void MainLayer::OnImGuiRender(){

	//Renderer Stats
	ImGui::Begin("Stats");
	ImGui::Text("FPS: %f", fps);
	ImGui::Text("");

	auto stats = RealEngine::Renderer2D::GetStats();
	ImGui::Text("Renderer Stats:");
	ImGui::Text("DrawCalls: %d", stats.DrawCalls);
	ImGui::Text("QuadCount: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
	ImGui::End();

	//Settings
	ImGui::Begin("Settings");

	ImGui::ColorEdit3("Cube Colors", glm::value_ptr(m_Color));

	//ImGui::Text("Camera Chunk: %i, %i", ChunkManager::PositionToChunkPosition({ m_Camera.GetPosition().x, m_Camera.GetPosition().z }).x, ChunkManager::PositionToChunkPosition({ m_Camera.GetPosition().x, m_Camera.GetPosition().z }).z);
	//ImGui::Text("Camera Pos: %f, %f, %f", m_Camera.GetPosition().x, m_Camera.GetPosition().z, m_Camera.GetPosition().y);
	//ImGui::Text("Camera Direction Facing: %s", ToString(m_Camera.GetFacingDirection()));
	if (ImGui::Button("Set Wireframe")) {
		m_Wireframe = !m_Wireframe;
		//RenderCommand::SetWireframe(m_Wireframe);
	}
		
	ImGui::End();
}

void MainLayer::OnEvent(Event& event) {
	m_Camera.OnEvent(event);

	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<KeyPressedEvent>(RE_BIND_EVENT_FN(MainLayer::OnKeyPressedEvent));
}

bool MainLayer::OnKeyPressedEvent(KeyPressedEvent& e) {
	if (e.GetKeyCode() == KeyCode::F1) {
		m_MouseEnabled = !m_MouseEnabled;
		//m_App->GetWindow().SetInputMode(m_MouseEnabled);
	}
	return false;
}