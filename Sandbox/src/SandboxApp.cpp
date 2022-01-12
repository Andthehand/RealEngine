#include <RealEngine.h>

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.h"

class ExampleLayer : public RealEngine::Layer {
public:
	ExampleLayer() : Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f) {
		m_SquareVA.reset(RealEngine::VertexArray::Create());

		float squareVertices[5 * 4] = {
			 -0.5f,  -0.5f,  0.0f,  0.0f, 0.0f,
			  0.5f,  -0.5f,  0.0f,  1.0f, 0.0f,
			  0.5f,   0.5f,  0.0f,  1.0f, 1.0f,
			 -0.5f,   0.5f,  0.0f,  0.0f, 1.0f,
		};

		RealEngine::Ref<RealEngine::VertexBuffer> squareVB;
		squareVB.reset(RealEngine::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({ 
			{ RealEngine::ShaderDataType::Float3, "a_Position" },
			{ RealEngine::ShaderDataType::Float2, "a_TexCoord" }
		});
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		RealEngine::Ref<RealEngine::IndexBuffer> squareIB;
		squareIB.reset(RealEngine::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);

		std::string squareVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			uniform mat4 u_Transform;
			uniform mat4 u_ViewProjection;
			out vec3 v_Position;
			void main() {
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			})";

		std::string squareFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			uniform vec3 u_Color;
			in vec3 v_Position;
			void main() {
				color = vec4(u_Color, 1.0f);
			})";

		//Square Color Shader
		m_SquareShader = RealEngine::Shader::Create("Square Shader", squareVertexSrc, squareFragmentSrc);

		//Texture Shader
		auto textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");
	
		m_Texture = RealEngine::Texture2D::Create("assets/textures/Checkerboard.png");
		m_LogoTexture = RealEngine::Texture2D::Create("assets/textures/RealEngine.png");

		std::dynamic_pointer_cast<RealEngine::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<RealEngine::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);
	}

	void OnUpdate(RealEngine::Timestep ts) override {
		if (RealEngine::Input::IsKeyPressed(RE_KEY_W)) {
			m_CameraPosition.y += m_CameraSpeed * ts;
		}
		if (RealEngine::Input::IsKeyPressed(RE_KEY_A)) {
			m_CameraPosition.x -= m_CameraSpeed * ts;
		}
		if (RealEngine::Input::IsKeyPressed(RE_KEY_S)) {
			m_CameraPosition.y -= m_CameraSpeed * ts;
		}
		if (RealEngine::Input::IsKeyPressed(RE_KEY_D)) {
			m_CameraPosition.x += m_CameraSpeed * ts;
		}

		RealEngine::RenderCommand::SetClearColor({ 0.1, 0.1, 0.1, 1 });
		RealEngine::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);

		RealEngine::Renderer::BeginScene(m_Camera);

		//static makes it not recalculated every update
		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		std::dynamic_pointer_cast<RealEngine::OpenGLShader>(m_SquareShader)->Bind();
		std::dynamic_pointer_cast<RealEngine::OpenGLShader>(m_SquareShader)->UploadUniformFloat3("u_Color", m_SquareColor);

		//Render flat color
		for (int x = 0; x < 20; x++) {
			for (int y = 0; y < 20; y++)
			{
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				RealEngine::Renderer::Submit(m_SquareShader, m_SquareVA, transform);
			}
		}

		auto textureShader = m_ShaderLibrary.Get("Texture");

		//Render with texture
		m_Texture->Bind();
		RealEngine::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		m_LogoTexture->Bind();
		RealEngine::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		//RealEngine::Renderer::Submit(m_Shader, m_VertexArray);

		RealEngine::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override {
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	void OnEvent(RealEngine::Event& event) override {
	}
private:
	RealEngine::ShaderLibrary m_ShaderLibrary;

	RealEngine::Ref<RealEngine::Shader> m_SquareShader;
	RealEngine::Ref<RealEngine::VertexArray> m_SquareVA;

	RealEngine::Ref<RealEngine::Texture2D> m_Texture;
	RealEngine::Ref<RealEngine::Texture2D> m_LogoTexture;

	RealEngine::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	const float m_CameraSpeed = 2.5f;

	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
};

class Sandbox : public RealEngine::Application {
public:
	Sandbox() {
		PushLayer(new ExampleLayer);
	}

	~Sandbox() {

	}
};

RealEngine::Application* RealEngine::CreateApplication() {
	return new Sandbox();
}