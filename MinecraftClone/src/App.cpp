#include <RealEngine.h>
#include <RealEngine/Core/EntryPoint.h>

#include "MainLayer.h"

class App : public RealEngine::Application {
public:
	App() : Application { "Minecraft" } {
		PushLayer(new MainLayer(1280, 720));
	}

	~App() = default;
};

RealEngine::Application* RealEngine::CreateApplication() {
	return new App();
}