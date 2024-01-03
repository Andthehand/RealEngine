#include <RealEngine.h>
#include <RealEngine/Core/EntryPoint.h>

#include "MainLayer.h"

class Sandbox : public RealEngine::Application {
public:
	Sandbox(const RealEngine::ApplicationSpecification& specification)
		: Application(specification) {
		PushLayer(new RealEngine::MainLayer());
	}

	~Sandbox() {

	}
};

RealEngine::Application* RealEngine::CreateApplication(RealEngine::ApplicationCommandLineArgs args) {
	ApplicationSpecification spec;
	spec.Name = "Sandbox";
	spec.CommandLineArgs = args;

	return new Sandbox(spec);
}
