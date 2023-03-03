#include "repch.h"
#include "Application.h"

#include "RealEngine/Renderer/Renderer.h"

#include "RealEngine/Core/Log.h"

#include "Input.h"
#include <GLFW/glfw3.h>

namespace RealEngine {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application(const std::string& name, ApplicationCommandLineArgs args)
		: m_CommandLineArgs(args) {
		RE_CORE_ASSERT(!s_Instance, "Application already exists!")
		s_Instance = this;

		m_Window = Window::Create(WindowProps(name));
		//Sets the event callback of the window to the applictation.cpp OnEvent function
		//i.e. resize event, mouse input, and keyboard input
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		Renderer::Init();

		//Pushes the ImGuiLayer to the layer stack so that it's easier to deal with
		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	void Application::PushLayer(Layer* layer) {
		RE_PROFILE_FUNCTION();

		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay) {
		RE_PROFILE_FUNCTION();

		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void Application::Close() {
		m_Running = false;
	}

	void Application::OnEvent(Event& e) {
		RE_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));
		
		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it) {
			if (e.Handled)
				break;
			(*it)->OnEvent(e);
		}
	}

	void Application::Run() {
		RE_PROFILE_FUNCTION();

		while (m_Running) {
			RE_PROFILE_SCOPE("Run Loop");

			float time = (float)glfwGetTime(); //TODO: Move to Platform
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (!m_Minimized) {
				RE_PROFILE_SCOPE("LayerStack OnUpdate");

				//Update all layers/Overlays
				for (Layer* layer : m_LayerStack)
					layer->OnUpdate(timestep);
			}

			m_ImGuiLayer->Begin();
			{
				RE_PROFILE_SCOPE("LayerStack OnImGuiRender");

				for (Layer* layer : m_LayerStack)
					layer->OnImGuiRender();
			}
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e) {
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e) {
		RE_PROFILE_FUNCTION();

		if(e.GetWidth() == 0 || e.GetHeight() == 0) {
			m_Minimized = true;
			return false;
		}
		m_Minimized = false;

		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}
}
