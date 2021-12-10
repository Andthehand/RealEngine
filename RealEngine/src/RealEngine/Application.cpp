#include "repch.h"
#include "Application.h"

#include "RealEngine/Log.h"
#include "RealEngine/Events/ApplicationEvent.h"

namespace RealEngine {

	Application::Application() {
		m_Window = std::unique_ptr<Window>(Window::Create());
	}

	Application::~Application() {
	}

	void Application::Run() {
		
		while (true) {
			m_Window->OnUpdate();
		}
	}
}