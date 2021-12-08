#pragma once
#include "core.h"

namespace RealEngine {

	class RealEngine_API Application {
	public:
		Application();
		virtual ~Application();

		void Run();
	};


	Application* CreateApplication();
}
