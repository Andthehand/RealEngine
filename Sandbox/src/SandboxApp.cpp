#include <RealEngine.h>

#include "imgui/imgui.h"

class ExampleLayer : public RealEngine::Layer {
public:
	ExampleLayer() : Layer("Example"), m_Camera(-1.0, 1.0f, -1.0f, 1.0f), m_CameraPosition(0.0f) {
		float vertices[3 * 7] = {
		-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
		 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
		 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		m_VertexArray.reset(RealEngine::VertexArray::Create());

		//Makes sure that m_VertexBuffer is deleted before storing another VertexBuffer
		//Also binds VBO to VAO
		std::shared_ptr<RealEngine::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(RealEngine::VertexBuffer::Create(vertices, sizeof(vertices)));

		RealEngine::BufferLayout layout = {
			{ RealEngine::ShaderDataType::Float3, "a_Position"},
			{ RealEngine::ShaderDataType::Float4, "a_Color"}
		};

		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		//Needed for glDrawElements in the game loop
		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<RealEngine::IndexBuffer> indexBuffer;
		indexBuffer.reset(RealEngine::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		/////////////////
		/// Square VAO //
		/////////////////

		m_SquareVA.reset(RealEngine::VertexArray::Create());

		float squareVertices[3 * 4] = {
			 -0.75f,  -0.75f,  0.0f,
			  0.75f,  -0.75f,  0.0f,
			  0.75f,   0.75f,  0.0f,
			 -0.75f,   0.75f,  0.0f
		};

		std::shared_ptr<RealEngine::VertexBuffer> squareVB;
		squareVB.reset(RealEngine::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({ { RealEngine::ShaderDataType::Float3, "a_Position" } });
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<RealEngine::IndexBuffer> squareIB;
		squareIB.reset(RealEngine::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;

			out vec3 v_Position;
			out vec4 v_Color;

			void main() {
				v_Position = a_Position;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);	
				v_Color = a_Color;
			})";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main() {
				color = v_Color;
			})";

		m_Shader.reset(new RealEngine::Shader(vertexSrc, fragmentSrc));

		std::string squareVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;

			out vec3 v_Position;

			void main() {
				v_Position = a_Position;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
			})";

		std::string squareFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			void main() {
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
			})";

		m_SquareShader.reset(new RealEngine::Shader(squareVertexSrc, squareFragmentSrc));
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

		RealEngine::Renderer::Submit(m_SquareShader, m_SquareVA);
		RealEngine::Renderer::Submit(m_Shader, m_VertexArray);

		RealEngine::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override {
	}

	void OnEvent(RealEngine::Event& event) override {
	}
private:
	std::shared_ptr<RealEngine::Shader> m_Shader;
	std::shared_ptr<RealEngine::VertexArray> m_VertexArray;

	std::shared_ptr<RealEngine::Shader> m_SquareShader;
	std::shared_ptr<RealEngine::VertexArray> m_SquareVA;

	RealEngine::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	const float m_CameraSpeed = 2.5f;
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