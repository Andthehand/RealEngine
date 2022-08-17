#include <RealEngine.h>
#include <RealEngine/Core/EntryPoint.h>

#include "SandboxLayer.h"

class Sandbox : public RealEngine::Application {
public:
	Sandbox() {
		PushLayer(new SandboxLayer);
	}

	~Sandbox() {

	}
};

RealEngine::Application* RealEngine::CreateApplication() {
	return new Sandbox();
}
