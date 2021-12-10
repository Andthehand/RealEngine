#include <RealEngine.h>
 
class ExampleLayer : public RealEngine::Layer {
public:
	ExampleLayer() : Layer("Example") {
	}

	void OnUpdate() override {
		RE_INFO("ExampleLayer::Update");
	}

	void OnEvent(RealEngine::Event& event) override {
		RE_TRACE("{0}", event);
	}
};

class Sandbox : public RealEngine::Application {
public:
	Sandbox() {
		PushLayer(new ExampleLayer());
	}

	~Sandbox() {

	}
};

RealEngine::Application* RealEngine::CreateApplication() {
	return new Sandbox();
}