#include <RealEngine.h>
#include <RealEngine/Core/EntryPoint.h>

#include "Sandbox2D.h"
#include "ExampleLayer.h"

class Sandbox : public RealEngine::Application {
public:
	Sandbox() {
		//PushLayer(new ExampleLayer);
		PushLayer(new Sandbox2D);
	}

	~Sandbox() {

	}
};

RealEngine::Application* RealEngine::CreateApplication() {
	return new Sandbox();
}