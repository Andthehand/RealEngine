#include "MainLayer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui/imgui.h"

MainLayer::MainLayer(Application* app, int width, int height)
	: Layer{ "Minecraft Main Layer" }, m_App(app), m_CameraController((float)width / (float)height) { }

void MainLayer::OnAttach() {
	m_App->GetWindow().SetInputMode(m_MouseEnabled);
	m_SpriteSheet = RealEngine::Texture2D::Create("assets/textures/Spritesheet.png");
	m_GrassTexture = RealEngine::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 0, 2 }, { 16, 16 });
}

void MainLayer::OnDetach() {
}

void MainLayer::OnUpdate(Timestep ts) {
	FPSCounter(ts);

	//Update
	m_CameraController.OnUpdate(ts);

	//Render Prep
	RealEngine::Renderer2D::ResetStats();
	RealEngine::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	RealEngine::RenderCommand::Clear();

	//Draw
	RealEngine::Renderer2D::BeginScene(m_CameraController.GetCamera());
	//for (int x = -5; x < 5; x++) {
	//	for (int y = -5; y < 5; y++) {
	//		glm::vec2 position = { x + (x * 0.1f), y + (y * 0.1f) };
	//		RealEngine::Renderer2D::DrawQuad({ position.x + (x * 0.1f), position.y + (y * 0.1f), -10.0f }, { 1.0f, 1.0f }, { m_Color, 1.0f });
	//	}
	//}

	RealEngine::Renderer2D::DrawQuad({ 0.0f, 0.0f, -10.0f }, { 9.0f, 3.0f }, m_SpriteSheet);

	RealEngine::Renderer2D::EndScene();
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
	ImGui::Text("Camera Zoom Level: %f", m_CameraController.GetZoomLevel());

	//ImGui::Text("Camera Chunk: %i, %i", ChunkManager::PositionToChunkPosition({ m_Camera.GetPosition().x, m_Camera.GetPosition().z }).x, ChunkManager::PositionToChunkPosition({ m_Camera.GetPosition().x, m_Camera.GetPosition().z }).z);
	//ImGui::Text("Camera Pos: %f, %f, %f", m_Camera.GetPosition().x, m_Camera.GetPosition().z, m_Camera.GetPosition().y);
	//ImGui::Text("Camera Direction Facing: %s", ToString(m_Camera.GetFacingDirection()));
	if (ImGui::Button("Set Wireframe")) {
		m_Wireframe = !m_Wireframe;
		RenderCommand::SetWireframe(m_Wireframe);
	}
		
	ImGui::End();
}

void MainLayer::OnEvent(Event& event) {
	m_CameraController.OnEvent(event);

	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<KeyPressedEvent>(RE_BIND_EVENT_FN(MainLayer::OnKeyPressedEvent));
}

bool MainLayer::OnKeyPressedEvent(KeyPressedEvent& e) {
	if (e.GetKeyCode() == KeyCode::F1) {
		m_MouseEnabled = !m_MouseEnabled;
		m_App->GetWindow().SetInputMode(m_MouseEnabled);
	}
	return false;
}