#include <RealEngine.h>
#include <RealEngine/Core/EntryPoint.h>

#include "MainLayer.h"

class Sandbox : public RealEngine::Application {
public:
	Sandbox(RealEngine::ApplicationCommandLineArgs args)
		: Application("NES Emulator") {
		PushLayer(new MainLayer());
	}

	~Sandbox() {

	}
};

RealEngine::Application* RealEngine::CreateApplication(RealEngine::ApplicationCommandLineArgs args) {
	return new Sandbox(args);
}
