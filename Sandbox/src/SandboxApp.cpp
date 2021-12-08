#include <Hazel.h>
 
class Sandbox : public RealEngine::Application {
public:
	Sandbox() {

	}

	~Sandbox() {

	}
};

RealEngine::Application* RealEngine::CreateApplication() {
	return new Sandbox();
}