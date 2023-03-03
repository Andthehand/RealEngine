#include <RealEngine.h>
#include <RealEngine/Core/EntryPoint.h>

#include "Core/MainLayer.h"

class Sandbox : public RealEngine::Application {
public:
	Sandbox() 
		: Application("Voxel Game") {
		PushLayer(new MainLayer());
	}

	~Sandbox() {
		
	}
};

RealEngine::Application* RealEngine::CreateApplication(ApplicationCommandLineArgs args) {
	return new Sandbox();
}
