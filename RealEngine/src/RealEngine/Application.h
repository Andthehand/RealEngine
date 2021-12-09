#pragma once

#include "Core.h"
#include "Events/Event.h"

namespace RealEngine {

	class RealEngine_API Application {
	public:
		Application();
		virtual ~Application();

		void Run();
	};


	Application* CreateApplication();
}
