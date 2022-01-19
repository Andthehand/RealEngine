#include "Sandbox2D.h"

#include <glm/gtc/type_ptr.hpp>

#include "imgui/imgui.h"

#include "Platform/OpenGL/OpenGLShader.h"


Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f, true) { }

void Sandbox2D::OnAttach() {
	m_VertexArray = RealEngine::VertexArray::Create();

	float squareVertices[5 * 4] = {
		 -0.5f,  -0.5f,  0.0f,
		  0.5f,  -0.5f,  0.0f,
		  0.5f,   0.5f,  0.0f,
		 -0.5f,   0.5f,  0.0f,
	};

	RealEngine::Ref<RealEngine::VertexBuffer> squareVB;
	squareVB = RealEngine::VertexBuffer::Create(squareVertices, sizeof(squareVertices));
	squareVB->SetLayout({
		{ RealEngine::ShaderDataType::Float3, "a_Position" }
	});
	m_VertexArray->AddVertexBuffer(squareVB);

	uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
	RealEngine::Ref<RealEngine::IndexBuffer> squareIB;
	squareIB = RealEngine::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
	m_VertexArray->SetIndexBuffer(squareIB);

	m_SquareShader = RealEngine::Shader::Create("assets/shaders/Color.glsl");
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

	RealEngine::Renderer::BeginScene(m_CameraController.GetCamera());

	std::dynamic_pointer_cast<RealEngine::OpenGLShader>(m_SquareShader)->Bind();
	std::dynamic_pointer_cast<RealEngine::OpenGLShader>(m_SquareShader)->UploadUniformFloat3("u_Color", m_SquareColor);

	glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	RealEngine::Renderer::Submit(m_SquareShader, m_VertexArray, transform);

	RealEngine::Renderer::EndScene();
}

void Sandbox2D::OnEvent(RealEngine::Event& e) {
	m_CameraController.OnEvent(e);
}