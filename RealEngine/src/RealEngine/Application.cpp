#include "Application.h"

#include "RealEngine/Log.h"
#include "RealEngine/Events/ApplicationEvent.h"

namespace RealEngine {

	Application::Application() {
	}

	Application::~Application() {
	}

	void Application::Run() {
		WindowResizeEvent e(1280, 720);
		RE_TRACE(e);
		
		while (true);
	}
}