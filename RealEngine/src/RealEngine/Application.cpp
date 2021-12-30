#include "repch.h"
#include "Application.h"

#include "RealEngine/Log.h"

#include <Glad/glad.h>

#include "Input.h"

namespace RealEngine {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	//TODO: Remove this
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
		switch (type) {
			case RealEngine::ShaderDataType::None:    return GL_FLOAT;
			case RealEngine::ShaderDataType::Float:	  return GL_FLOAT;
			case RealEngine::ShaderDataType::Float2:  return GL_FLOAT;
			case RealEngine::ShaderDataType::Float3:  return GL_FLOAT;
			case RealEngine::ShaderDataType::Float4:  return GL_FLOAT;
			case RealEngine::ShaderDataType::Mat3:	  return GL_FLOAT;
			case RealEngine::ShaderDataType::Mat4:	  return GL_FLOAT;
			case RealEngine::ShaderDataType::Int:	  return GL_INT;
			case RealEngine::ShaderDataType::Int2:	  return GL_INT;
			case RealEngine::ShaderDataType::Int3:	  return GL_INT;
			case RealEngine::ShaderDataType::Int4:	  return GL_INT;
			case RealEngine::ShaderDataType::Bool:    return GL_BOOL;
		}

		RE_CORE_ASSERT(false, "Unknown ShaderDataType");
		return 0;
	}

	Application::Application() {
		RE_CORE_ASSERT(!s_Instance, "Application already exists!")
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		//Sets the event callback of the window to the applictation.cpp OnEvent function
		//i.e. resize event, mouse input, and keyboard input
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		//Pushes the ImGuiLayer to the layer stack so that it's easier to deal with
		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		//-------------------//
		#pragma region TempCode
		//-------------------//

		//Creates and binds the VAO so that the VBO binds to it
		glGenVertexArrays(1, &m_VertexArray);
		glBindVertexArray(m_VertexArray);

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		//Makes sure that m_VertexBuffer is deleted before storing another VertexBuffer
		//Also binds VBO to VAO
		m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		//The brackets make it so that layout is created stored then deleted right away
		{
			BufferLayout layout = {
				{ ShaderDataType::Float3, "a_Position"},
				{ ShaderDataType::Float4, "a_Color"}
			};

			m_VertexBuffer->SetLayout(layout);
		}

		//Loops through the elements to add to the VBO such as a_Position and a_Color
		uint32_t index = 0;
		const auto& layout = m_VertexBuffer->GetLayout();
		for (const auto& element : layout) {
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, 
				element.GetComponentCount(), 
				ShaderDataTypeToOpenGLBaseType(element.Type), 
				element.Normalized ? GL_TRUE : GL_FALSE, 
				layout.GetStride(),
				(const void*)element.Offset);

			index++;
		}

		//I think it's needed for the ImGUI
		uint32_t indices[3] = { 0, 1, 2 };
		m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			out vec3 v_Position;
			out vec4 v_Color;

			void main() {
				v_Position = a_Position;
				gl_Position = vec4(a_Position, 1.0);	
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

		m_Shader.reset(new Shader(vertexSrc, fragmentSrc));
		#pragma endregion
	}

	Application::~Application() {
	}

	void Application::PushLayer(Layer* layer) {
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay) {
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void Application::OnEvent(Event& e) {
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
		
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::Run() {
		
		while (m_Running) {
			//Clear screen to color
			glClearColor(0.1, 0.1, 0.1, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			//Bind current shader and buffer
			m_Shader->Bind();
			glBindVertexArray(m_VertexArray);
			glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);

			//Update all layers/Overlays
			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e) {
		m_Running = false;
		return true;
	}
}