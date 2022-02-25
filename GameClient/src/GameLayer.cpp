#include "GameLayer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui/imgui.h"

GameLayer::GameLayer() : Layer("GameLayer"), m_CameraController(1280.0f / 720.0f) { }

void GameLayer::OnAttach() {
	m_SquareVA = VertexArray::Create();

	//I think this is fine??? MABYE
	float temp = width / height / 10;

	float squareVertices[5 * 4] = {
		 -temp,  -temp,  0.0f,  0.0f, 0.0f,
		  temp,  -temp,  0.0f,  1.0f, 0.0f,
		  temp,   temp,  0.0f,  1.0f, 1.0f,
		 -temp,   temp,  0.0f,  0.0f, 1.0f,
	};

	Ref<VertexBuffer> squareVB;
	squareVB = VertexBuffer::Create(squareVertices, sizeof(squareVertices));
	squareVB->SetLayout({
		{ ShaderDataType::Float3, "a_Position" },
		{ ShaderDataType::Float2, "a_TexCoord" }
	});
	m_SquareVA->AddVertexBuffer(squareVB);

	uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
	Ref<IndexBuffer> squareIB;
	squareIB = IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
	m_SquareVA->SetIndexBuffer(squareIB);

	m_SquareShader = Shader::Create("assets/shaders/Snake.glsl");
}

void GameLayer::OnDetach() {

}

void GameLayer::OnImGuiRender() {
	ImGui::Begin("Settings");
	ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void GameLayer::OnUpdate(Timestep ts) {
	RenderCommand::SetClearColor({ 0.1, 0.1, 0.1, 1 });
	RenderCommand::Clear();

	//No movement or rotation so why update camera
	//m_CameraController.OnUpdate(ts);
	Renderer::BeginScene(m_CameraController.GetCamera());

	//No idea what to call these
	float temp = width / height / 10;
	m_SquareShader->SetFloat4("u_Color", glm::vec4(m_SquareColor, 1.0f));
	for (int x = -(width / 2); x < width / 2; x++) {
		for (int y = -(height / 2); y < height / 2; y++) {
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), { temp * x, temp * y, 0.0f });
			Renderer::Submit(m_SquareShader, m_SquareVA, transform);
		}
	}
}

void GameLayer::OnEvent(Event& event) {
	m_CameraController.OnEvent(event);
}