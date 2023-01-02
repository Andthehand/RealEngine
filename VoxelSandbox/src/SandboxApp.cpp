#include <RealEngine.h>
#include <RealEngine/Core/EntryPoint.h>

#include "MainLayer.h"

class Sandbox : public RealEngine::Application {
public:
	Sandbox() 
		: Application("Voxel Engine") {
		PushLayer(new MainLayer());
	}

	~Sandbox() {
		
	}
};

RealEngine::Application* RealEngine::CreateApplication() {
	return new Sandbox();
}
